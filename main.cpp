#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// Semafor binarny
class Semaphore
{
private:
    std::atomic<bool> flag;

public:
    Semaphore() : flag(false) {}

    void acquire()
    {
        bool expected = false;
        while (!flag.compare_exchange_weak(expected, true))
        {
            expected = false;
            std::this_thread::yield();
        }
    }

    void release()
    {
        flag.store(false);
    }
};

// Stan filozofa
enum PhilosopherState
{
    THINKING,
    HUNGRY,
    EATING
};

// Klasa synchronizacji wyświetlania
class ConsoleLock
{
private:
    Semaphore lock;

public:
    void print(const std::string &message)
    {
        lock.acquire();
        std::cout << message << std::endl;
        lock.release();
    }
};

// Główna metoda filozofa
// TODO: Rozbudować o faktyczną logikę podnoszenia i odkładania widelców
void philosopher_function(int id, int total_philosophers, ConsoleLock &console)
{
    for (int i = 0; i < 5; i++)
    {
        // Myślenie
        console.print("Filozof " + std::to_string(id) + " myśli");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));

        // Próba zjedzenia
        console.print("Filozof " + std::to_string(id) + " chciałby zjeść");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));
    }
}

// TODO: Zaimplementować klasę zarządzającą stanem stołu i widelców
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Użycie: " << argv[0] << " <liczba filozofów>" << std::endl;
        return 1;
    }

    int num_philosophers;
    try
    {
        num_philosophers = std::stoi(argv[1]);
        if (num_philosophers <= 0)
        {
            throw std::invalid_argument("Liczba filozofów musi być dodatnia");
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Błąd: " << e.what() << std::endl;
        return 1;
    }

    ConsoleLock console;
    std::vector<std::thread> threads;

    // Tworzenie wątków dla filozofów
    for (int i = 0; i < num_philosophers; i++)
    {
        threads.push_back(std::thread(philosopher_function, i, num_philosophers, std::ref(console)));
    }

    // Czekanie na zakończenie wszystkich wątków
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}