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

    bool try_acquire()
    {
        bool expected = false;
        return flag.compare_exchange_weak(expected, true);
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

// TODO: Expand with deadlock prevention mechanism
class DiningTable
{
private:
    int num_philosophers;
    std::vector<Semaphore> forks;
    std::vector<PhilosopherState> states;
    ConsoleLock console;

public:
    DiningTable(int n) : num_philosophers(n), forks(n), states(n, THINKING) {}

    // Attempt to pick up forks
    bool try_pickup_forks(int id)
    {
        int left_fork = id;
        int right_fork = (id + 1) % num_philosophers;

        // Try to pick up left fork
        if (!forks[left_fork].try_acquire())
        {
            return false;
        }

        // Try to pick up right fork
        if (!forks[right_fork].try_acquire())
        {
            // Failed, put down left fork
            forks[left_fork].release();
            return false;
        }

        return true;
    }

    // Put down forks
    void put_down_forks(int id)
    {
        int left_fork = id;
        int right_fork = (id + 1) % num_philosophers;

        forks[left_fork].release();
        forks[right_fork].release();
    }

    void set_state(int id, PhilosopherState new_state)
    {
        states[id] = new_state;
    }

    PhilosopherState get_state(int id) const
    {
        return states[id];
    }

    void log(const std::string &message)
    {
        console.print(message);
    }
};

// TODO: Implement simulation control mechanism and deadlock handling
void philosopher_function(int id, DiningTable &table)
{
    const int cycles = 10; // Number of thinking/eating cycles

    for (int i = 0; i < cycles; i++)
    {
        // Thinking
        table.set_state(id, THINKING);
        table.log("Philosopher " + std::to_string(id) + " is thinking");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));

        // Trying to pick up forks
        table.set_state(id, HUNGRY);
        table.log("Philosopher " + std::to_string(id) + " is hungry and trying to pick up forks");

        // Keep trying until both forks are picked up
        while (!table.try_pickup_forks(id))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Eating
        table.set_state(id, EATING);
        table.log("Philosopher " + std::to_string(id) + " is eating");
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 500));

        // Putting down forks
        table.put_down_forks(id);
        table.log("Philosopher " + std::to_string(id) + " puts down forks");
    }
}

// TODO: Add simulation time limit and safe stopping mechanism
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

    DiningTable table(num_philosophers);
    std::vector<std::thread> threads;

    // Create threads for philosophers
    for (int i = 0; i < num_philosophers; i++)
    {
        threads.push_back(std::thread(philosopher_function, i, std::ref(table)));
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}