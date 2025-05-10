# Wielowątkowa Aplikacja Czatu - Dokumentacja

## Opis projektu
Ten projekt implementuje wielowątkową aplikację czatu typu klient-serwer w języku Python. Serwer może obsługiwać wiele równoczesnych połączeń klientów i rozsyłać wiadomości pomiędzy nimi. Rozwiązanie wykorzystuje gniazda sieciowe (sockets) i mechanizmy synchronizacji wątków do zapewnienia bezpiecznej i wydajnej komunikacji.

## Założenia problemu
- Serwer czatu musi obsługiwać wiele równoczesnych połączeń klientów.
- Każdy klient musi mieć unikalną nazwę użytkownika w systemie.
- Wiadomości wysyłane przez klienta powinny być rozsyłane do wszystkich pozostałych klientów.
- System powinien być odporny na rozłączenia klientów.
- Klienci powinni być informowani o dołączaniu i opuszczaniu czatu przez innych użytkowników.

## Cechy implementacji
- Wykorzystanie wielowątkowości do obsługi wielu połączeń jednocześnie.
- Zastosowanie blokad (locks) do synchronizacji dostępu do współdzielonych zasobów.
- Walidacja nazw użytkowników zapobiegająca duplikatom.
- Obsługa powiadomień o dołączeniu i opuszczeniu czatu.
- Bezpieczne zarządzanie połączeniami i rozłączeniami.
- Skrypt automatyzujący uruchomienie serwera i klientów.

## Wymagania systemowe
- Python 3.x
- System Linux z GNOME Terminal (dla skryptu uruchamiającego)
- Pakiety standardowe Python (socket, threading)

## Uruchomienie programu
Program można uruchomić za pomocą załączonego skryptu:
```bash
./run.sh [opcje]
```

Dostępne opcje:
- `-p PORT` - określenie niestandardowego portu (domyślnie 9999)

Przykłady:
```bash
# Uruchomienie z domyślnymi parametrami (port 9999)
./run.sh

# Uruchomienie z niestandardowym portem 8080
./run.sh -p 8080
```

Można również uruchomić serwer i klientów ręcznie:
```bash
# Uruchomienie serwera
python server.py [port]

# Uruchomienie klienta
python client.py [host] [port]
```

## Struktura kodu
### Klasy i komponenty
1. `ChatServer` - główna klasa serwera implementująca obsługę wielu klientów.
2. `ChatClient` - klasa klienta odpowiedzialna za łączenie i komunikację z serwerem.

### Kluczowe funkcje
#### Serwer
- `start()` - uruchamia serwer i obsługuje przychodzące połączenia.
- `handle_client(client_socket, address)` - obsługuje indywidualne połączenie klienta.
- `is_username_taken(username)` - sprawdza, czy nazwa użytkownika jest już zajęta.
- `broadcast(message, sender_socket)` - rozsyła wiadomość do wszystkich klientów oprócz nadawcy.

#### Klient
- `connect()` - łączy z serwerem i inicjuje proces komunikacji.
- `receive_messages()` - odbiera i wyświetla wiadomości z serwera (działa w osobnym wątku).
- `send_messages()` - pobiera i wysyła wiadomości do serwera (działa w głównym wątku).

## Mechanizm działania
1. Serwer uruchamia się i nasłuchuje na określonym porcie.
2. Dla każdego nowego połączenia klienta tworzony jest osobny wątek.
3. Klient musi podać unikalną nazwę użytkownika, która jest weryfikowana przez serwer.
4. Po zaakceptowaniu nazwy użytkownika, klient dołącza do czatu, a pozostali użytkownicy otrzymują powiadomienie.
5. Wiadomości wysyłane przez klienta są rozsyłane do wszystkich pozostałych klientów.
6. Gdy klient się rozłącza, jego wątek jest zamykany, a pozostali użytkownicy są informowani o opuszczeniu czatu.

## Synchronizacja i zapobieganie wyścigom
Program implementuje mechanizmy synchronizacji, aby zapobiec problemom współbieżności:
- Blokada (`threading.Lock`) jest używana do ochrony współdzielonych zasobów (lista klientów).
- Dostęp do listy klientów jest synchronizowany w sekcjach krytycznych.
- Operacje dodawania/usuwania klientów i rozsyłania wiadomości są atomowe.
- Wykorzystanie wątków daemon zapewnia, że aplikacja może zostać bezpiecznie zakończona.

## Szczegóły implementacji serwera
Serwer implementuje następujące mechanizmy:
- Nasłuchiwanie na określonym porcie za pomocą gniazda TCP/IP
- Tworzenie nowego wątku dla każdego połączonego klienta
- Weryfikacja unikalności nazw użytkownika
- Synchronizacja dostępu do listy klientów za pomocą blokady
- Powiadamianie wszystkich klientów o zmianach w czacie
- Obsługa rozłączeń klientów i czyszczenie zasobów

## Szczegóły implementacji klienta
Klient implementuje następujące mechanizmy:
- Nawiązywanie połączenia z serwerem
- Przesyłanie nazwy użytkownika i weryfikacja jej dostępności
- Utworzenie osobnego wątku do odbierania wiadomości
- Wysyłanie wiadomości do serwera z głównego wątku
- Obsługa rozłączenia i zamknięcia połączenia

## Obsługa błędów
Aplikacja zawiera mechanizmy obsługi błędów:
- Wykrywanie i obsługa nieoczekiwanych rozłączeń klientów
- Bezpieczne zamykanie gniazd i wątków w przypadku błędów
- Informowanie użytkownika o problemach z połączeniem
- Weryfikacja dostępności portu przed uruchomieniem serwera

## Autor
Miłosz Lewandowski