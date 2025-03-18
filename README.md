# Problem Jedzących Filozofów - Dokumentacja

## Opis projektu

Ten projekt implementuje klasyczne zagadnienie synchronizacji znane jako "Problem jedzących filozofów" na potrzeby przedmiotu Systemy Operacyjne 2. Rozwiązanie wykorzystuje semafory binarne do koordynacji dostępu do współdzielonych zasobów (widelców) i zapobiega zakleszczeniom między wątkami reprezentującymi filozofów.

## Założenia problemu

- Przy okrągłym stole siedzi n filozofów.
- Każdy filozof na przemian myśli i je.
- Między każdą parą sąsiadujących filozofów znajduje się jeden widelec.
- Aby zjeść posiłek, filozof musi podnieść oba sąsiadujące z nim widelce.
- Po zakończeniu posiłku, filozof odkłada widelce i wraca do myślenia.

## Cechy implementacji

- Zastosowanie semaforów binarnych do synchronizacji dostępu do zasobów.
- Eliminacja zakleszczenia dzięki rozwiązaniu opartemu na algorytmie Dijkstry.
- Kontrola stanów filozofów (THINKING, HUNGRY, EATING).
- Bezpieczne zakończenie symulacji w dowolnym momencie.
- Synchronizacja wyświetlania komunikatów na konsoli.

## Kompilacja programu

Program można skompilować za pomocą kompilatora obsługującego standard C++11 lub nowszy, na przykład:

```bash
g++ main.cpp -o main
```

## Uruchomienie programu

Program można uruchomić z następującymi opcjonalnymi parametrami:

```bash
./main [liczba_filozofów] [czas_symulacji_w_sekundach]
```

Gdzie:
- `liczba_filozofów` - ilość filozofów biorących udział w symulacji (domyślnie 5)
- `czas_symulacji_w_sekundach` - czas trwania symulacji w sekundach (domyślnie 30)

Przykłady:
```bash
# Uruchomienie z domyślnymi parametrami (5 filozofów, 30 sekund)
./main

# Uruchomienie z 7 filozofami i domyślnym czasem
./main 7

# Uruchomienie z 10 filozofami i czasem 45 sekund
./main 10 45
```

## Struktura kodu

### Klasy i komponenty

1. `Semaphore` - implementacja semafora binarnego przy użyciu operacji atomowych.
2. `ConsoleLock` - klasa do synchronizacji wyświetlania komunikatów na konsoli.
3. `DiningTable` - główna klasa implementująca rozwiązanie problemu jedzących filozofów.
4. `PhilosopherState` - enum reprezentujący możliwe stany filozofa (THINKING, HUNGRY, EATING).

### Kluczowe funkcje

- `pickup_forks(int i)` - funkcja do bezpiecznego podnoszenia widelców przez filozofa.
- `return_forks(int i)` - funkcja do odkładania widelców i sprawdzania, czy sąsiedzi mogą jeść.
- `test(int i)` - funkcja sprawdzająca, czy filozof może zacząć jeść.
- `philosopher_function(int id)` - główna funkcja reprezentująca cykl życia filozofa.

## Mechanizm działania

1. Każdy filozof jest reprezentowany przez osobny wątek.
2. Filosfowie cyklicznie przechodzą przez fazy myślenia i jedzenia.
3. Przed jedzeniem filozof musi podnieść oba widelce (lewy i prawy).
4. Widelce są reprezentowane przez semafory binarne.
5. Stan każdego filozofa jest kontrolowany, aby zapobiec zakleszczeniom.
6. Implementacja opiera się na algorytmie Dijkstry, który gwarantuje brak zakleszczenia i zapobiega zagłodzeniu filozofów.

## Zapobieganie zakleszczeniom

Program implementuje strategię zapobiegania zakleszczeniom poprzez:
- Sprawdzanie dostępności obu widelców przed próbą ich podniesienia.
- Wykorzystanie semafora `state_mutex` do ochrony sekcji krytycznej podczas zmiany stanu filozofa.
- Sprawdzanie stanu sąsiednich filozofów po zwolnieniu widelców.

## Autor
Miłosz Lewandowski