# FR
1) Broker pozwala węzłowi zasubskrybować się na temat 
2) Broker przesyła węzłowi-konsumentowi wiadomości na temat(y), na który(e) węzeł jest zasubskrybowany
3) Broker oczekuje wiadomości do węzłów produkujących
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

