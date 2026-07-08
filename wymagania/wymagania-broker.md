# FR
1) Broker pozwala węzłowi zasubskrybować się na temat

2) Broker przesyła węzłowi-konsumentowi wiadomości na temat(y), na który(e) węzeł jest zasubskrybowany

3) Broker oczekuje wiadomości do węzłów (producentów i konsumentów)

4) Broker wspiera mechanizm handshake. W ramach handshake'a:
    4.1) Broker otrzymuje wiadomość określonego formatu od węzła 
    4.2) Sprawdza spójność otrzymanej wiadomości
    4.3) W przypadku spójności wiadomości przesyła listę tematów 
    4.4) W przypadku błędnego formatu przesyła wiadomość o tym węzłowi

5) Broker otrzymuje wiadomość określonego formatu od węzła z prośbą o dodanie go do listy węzłów, przypisanych do wybranego przez węzeł tematu  

6) Broker może (w zależności od zawartości nagłówka) rozpoznać typ wiadomości

7) Broker może rozpoznać typy wiadomości od węzłów:
    7.1) Handshake
    7.2) Wiadomość od węzła-producenta
    7.3) Wiadomość od węzła-konsumenta
    7.4) Wiadomości w ramach QoS1
    7.5) Wiadomości w ramach QoS2
    
8) Broker nadaje węzłom prawa do publikacji/odczytu wiadomości na konkretne tematy (gradacja praw węzłów na tematy)

9) Broker przechowuje listę tematów w bazie danych 

10) Broker na początku swojej pracy uruchamia FailoverBroker

11) Failover broker ma statyczny IP 

12) Główny proker regularnie wysyła do failover brokera wiadomości informujące, że on żyje

13) Broker usuwa tematy z bazy danych, na które nie było publikacji dłużej niż dwa miesiące

14) Broker dodaje do bazy danych każde nowe konto

15) Broker po dodaniu węzła do tematu wysyła potwierdzenie do węzła 

16) Broker wspiera odłączanie węzła z konkretnego tematu //

17) Broker wspiera ponowne połączenie węzła (reconnect) – po utracie połączenia węzeł może wznowić sesję bez ponownej pełnej rejestracji.


# NFR 
1) Broker wspiera protokół TCP/IP

2) Broker wspiera protokół TCP/lwIP

3) Broker wspiera szyfrowanie 

4) Czas obsługi jednej wiadomości musi być krótszy od 100ms

5) Broker wspiera systemy operacyjny Linux i OpenBSD

6) Broker utrzymuje do 500 węzłów przypisanych do jednego tematu

7) Maksymalna długość wiadomości, którą może przyjąć/wysłać broker wynosi 1MB

8) Broker wspiera logowanie zdarzeń (np. do pliku lub syslog) z konfigurowalnym poziomem szczegółowości // 

9) Broker wspiera konfigurację w pliku (JSON) oraz możliwość przeładowania konfiguracji bez restartu.

10) Broker wspiera TLS 1.2 dla szyfrowania komunikacji 

11) Czas przełączania failover (detekcja śmierci + podniesienie VIP + uruchomienie brokera) jest krótszy niż 5 sekund.


# Rozważania 

4, 5 FR) Czy na końcu handshake'a broker wywołuje `close()`? Tzn. co robimy, gdy węzeł nie znalazł tematu, na który chciał zasubskrybować się? I czy ogólnie w systemie chcemy odróżniać nieporządane przerwanie połączenia od porządnego (oczekujemy jakiejś bye-bye wiadomości?)? 


4 NFR) Można to sprawdzić za pomocą time stampów w nagłówku wiadomości (węzeł publikujący wpisuje czas wysłania do nagłówka, a aplikacja przy odczycie wiadomości odczytuje czas otrzymania. Stąd różnica między czasem wysłania i odczytu jest w przybliżeniu równa czasowi obróbki wiadomości)

5 FR) Typ odrębny typ wiadomości -- zapytanie o subskrypcję

7 FR) QoSn w tym kontekście -- kopia tego, co jest w MQTT


4 NFR) Próbowałem testować broker mosquitto, żeby ocenić ile on czasu traci na obróbkę wiadomości, ale były to czasy rzędu kilku milisekund. Zakładam, w naszym przypadku zapytanie do bazy danych będzie wprowadzać opóźnienie. Więc 100ms -- czas, który wziąłem z sufitu, nie jest to wymagane do czegokolwiek, i nie wiem, jak trudne będzie osiągnięcie tego czasu.


