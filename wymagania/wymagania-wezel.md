# FR
1) Węzeł może występować jako producent lub konsument danych

2) Węzeł może występować jako producent i konsument jednocześnie

3) Węzeł produkujący przesyła wiadomości (w ramach publikacji na temat) o określonym formacie brokerowi 

4) Węzeł wspiera meachanizm handshake

5) Węzeł w celu podłączenia się do tematu wysyła wiadomość do brokera o określonym formacie

6) Węzeł wspiera czujniki analogowe

7) Węzeł wspiera czujniki cyfrowe

8) Węzeł w ramach publikacji może wysłać typy wiadomości: DATA, WARNING, ERROR 

9) Węzeł-konsument może rozpoznać typy wiadomości: DATA, WARNING, ERROR

10) Węzeł-konsument próbujący zasubskrybować się na nieistniejący temat może jawnie go stworzyć

11) Węzeł przy pierwszym podłączeniu do brokera tworzy konto przesyłając login i hasło

12) Węzeł może zasubskrybować się na wszystkie tematy jednocześnie (wildcard) 

13) Węzeł przy połączeniu do tematu oczekuje potwierdzenia od brokera

14) Węzeł wspiera połączenie z brokerem poprzez TLS/SSL (szyfrowanie). //

15) Węzeł może odczytać ostatnią wiadomość na temat (retained message) przy subskrypcji

16) Węzeł buforuje wiadomości (kolejkuje) w przypadku chwilowej niedostępności brokera i wysyła je po przywróceniu połączenia.




# NFR
1) Węzeł wspiera protokół TCP/IP
2) Węzeł wspiera protokół TCP/lwIP
3) Węzeł wspiera do 2 czujników analogowych
4) Węzeł wspiera do 255 czujników cyfrowych
5) Hasło węzła powinno zawierać przynajmniej 8 symboli
6) Login węzła nie powinien zawierać symboli specjalnych
7) Węzeł nie może stworzyć tematu, który wymaga praw do publikacji/subskrypcji wyższych, niż jego własne


# Rozważania 

8, 9 FR) Data -- dane z czujników, które węzeł wysyła brokerowi/otrzymuje od brokera, WARNING -- wiadomości, które informują o niekrytycznych rzeczach, np. o niskiej temperaturze, ciśnieniu, itd. ERROR -- informacja o błędach (softwearowych lub hardwearowych, np. o nieudanej inicjalizacji i2c). Broker nie interesuje się typem wiadomości. Można też zmapować tą gradację na QoS
    
5 FR) Chodzi o format nagłówka, który różni się od nagłówka wiadomości z danymi z czujników. Podczasu podłączenia węzeł wysyła tematY na które chciałby zasubskrybować się, a login i hasło do autoryzacji(i ewentualnie sekret) w ramach handshake'a. Można też dodać do nagłówka bit, który informuje czy konkretne konto jest "zarejestrowane". Jeśli nie, to login i hasło dodają sie do bazy danych (stąd 11FR i 13FR w brokerze).
