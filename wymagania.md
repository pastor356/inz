# Wymagania projektowe: Broker
    1) Wymagania FR:
        1.1) System daje możliwość utworzenia tematu przez producenta
        1.2) System daje możliwość zasubskrybowania się na konkrenty temat producentowi lub konsumentowi
        1.3) Każdy węzeł (konsument lub producent zasubskrybowany/publikujący) posiada prawa do zapisu lub odczytu danych
        1.4) Każdy węzeł musi być autoryzowany w systemie (obecność praw na tematy wymusza) 
        1.5) Broker przechowuje w kontenerze inf-cję o temacie: nazwa, zbiór konsumentów, zbiór producentów, ich prawa na odczyt/publikacje,  ... (mutex do ochrony wspólnych danych)
        1.6) Broker przechowuje w kontenerze inf-cję o podłączonym węźle: nazwa (lub imię), hasło, rola (konsument/producent), ...
        1.7) System powinien nadawać do każdego tematu poziom dostępu (gradacja: USER, ADMIN, SYSTEM (logi brokera))
        1.8) Po połączeniu nowego węzła broker wysyła inf-cję o dostępnych tematach, ilościach producentów, konsumentów (handshake)

    2) Wymagania NFR:
        p = 500, k = 1000, t = 500 // te liczby, szczerze, wziąłem z sufitu
        2.1) System powinien wspierać do t tematów
        2.2) Do p producentów na każdy temat
        2.3) Do k konsumentów na każdy temat
        2.4) Broker przechowuje dane z czujników do 1 tygodnia 
        2.5) System musi być dostępny dla węzłów, które wspierają protokół TCP/IP lub TCP/lwIP (low weight, wtedy węzłem może występować ESP32 (lub dowolny węzeł z modułem COM)) => skalowalność
        2.6) Komunikacja zaszyfrowana dla tematów, które mają prawa odczytu i zapisu ADMIN

# Wymagania projektowe: aplikacja konsumenta
    // skoro mamy klientów wirtualnych, to aplikację można potraktować jako system monitorowania np. parkingu inteligentnego

    3) Wymagania FR:
        3.1) Aplikacja może zasubskrybować się na wiele tematów (jako konsument)
        3.2) Aplikacja może wyprowadzać otrzymane dane (w czasie rzeczywistym) w postaci wykresów (wielkości od czasu)
        3.3) Aplikacja może subskrybować się i odsubskrybować się od tematów w dowolnym momencie
        3.4) Aplikacja uczestniczy w handshake z brokerem (otrzymuje listę dostępnych tematów)

    4) Wymagania NFR
        4.1) Aplikacja przechowuje w bazie danych otrzymane dane w ciągu 1 roku 
        4.2) Aplikacja może być zasubskrybowana na 500 tematów jednocześnie 
