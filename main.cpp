#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// Binary semaphore
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

// Philosopher state
enum PhilosopherState
{
    THINKING,
    HUNGRY,
    EATING
};

// Console synchronization class
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

// Main philosopher method
// TODO: Expand with actual logic for picking up and putting down forks
void philosopher_function(int id, int total_philosophers, ConsoleLock &console)
{
    for (int i = 0; i < 5; i++)
    {
        // Thinking
        console.print("Philosopher " + std::to_string(id) + " is thinking");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));

        // Trying to eat
        console.print("Philosopher " + std::to_string(id) + " would like to eat");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));
    }
}

// TODO: Implement a class to manage the table state and forks
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <number of philosophers>" << std::endl;
        return 1;
    }

    int num_philosophers;
    try
    {
        num_philosophers = std::stoi(argv[1]);
        if (num_philosophers <= 0)
        {
            throw std::invalid_argument("Number of philosophers must be positive");
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    ConsoleLock console;
    std::vector<std::thread> threads;

    // Creating threads for philosophers
    for (int i = 0; i < num_philosophers; i++)
    {
        threads.push_back(std::thread(philosopher_function, i, num_philosophers, std::ref(console)));
    }

    // Waiting for all threads to complete
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}