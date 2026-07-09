# FR

1) Aplikacja subskrybuje się na wszystkie tematy brokera, przy czym aplikacja ma prawa ADMIN do odczytu wiadomości

2) Aplikacja akumuluje otrzymane dane w bazie danych 

3) Aplikacja jest węzłem-konsumentem

4) Aplikacja na bieżąco wyświetla dane otrzymane od brokera w postaci wykresów czasowych

5) Aplikacja przy uruchomieniu odczytuje dane z bazy danych w celu wykreślenia ostatnich podbranych (od brokera) danych

6) Aplikacja buforuje otrzymane dane w przypadku braku połączenia z bazą danych 

7) Aplikacja dodaje wartości do wykresów, zatem  dodaje do bazy danych 






# NFR

1) GUI aplikacji musi mieć zakładkę z wykresami

2) Na jednym wykresie (dwuwymiarowym) na osi OX jest czas, na OY -- jedna wybrana jednostka (po jednym wykresie na wielkość)

3) Wykres można otworzyć w pełnym ekranie

4) Przy naprowadzeniu kursora myszki na próbkę wyświetla się wartość próbki (współrzędne punktu) 

5) GUI aplikacji musi mieć zakładkę z podstawową informacją o każdum węźle (temat, login węzła, samochód) w postaci tabeli

6) GUI aplikacji musi wyróżnić kolorem wiadomości o prioritecie różnym od DATA 

7) GUI aplikacji musi mieć przycisk do czyszczenia wykresów

8) Aplikacja działa na systemie Windows i Linux

9) Czas odświeżania wykresu jest nie dłuższy od 500ms



# Rozważania 
 
6 NFR) 8, 9 FR w wymaganiach węzła

7 FR) unikamy w ten sposób regularnych zapytań o odczyt do bazy danych 
