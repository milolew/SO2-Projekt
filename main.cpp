#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <string>
#include <functional>

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

// Console output synchronization class
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

// Class implementing the dining philosophers problem
class DiningTable
{
private:
    int n;                               // number of philosophers
    std::vector<PhilosopherState> state; // array of philosopher states
    std::vector<Semaphore> forks;        // semaphores for forks
    std::vector<Semaphore> self;         // semaphore for each philosopher
    Semaphore state_mutex;               // semaphore for accessing the state array
    std::atomic<bool> running;           // flag to stop the simulation
    ConsoleLock console;                 // object for display synchronization

    // Checks if a philosopher can eat
    void test(int i)
    {
        if (state[i] == HUNGRY &&
            state[(i + n - 1) % n] != EATING &&
            state[(i + 1) % n] != EATING)
        {

            state[i] = EATING;
            self[i].release();
        }
    }

public:
    DiningTable(int num_philosophers)
        : n(num_philosophers),
          state(num_philosophers, THINKING),
          forks(num_philosophers),
          self(num_philosophers),
          running(true)
    {

        // Initialize semaphores for forks
        for (int i = 0; i < n; i++)
        {
            self[i].acquire();
        }
    }

    void log(const std::string &message)
    {
        console.print(message);
    }

    // Safe fork pickup with deadlock elimination
    void pickup_forks(int i)
    {
        state_mutex.acquire();
        state[i] = HUNGRY;
        log("Philosopher " + std::to_string(i) + " is hungry and trying to pick up forks");
        test(i);
        state_mutex.release();

        self[i].acquire();
        log("Philosopher " + std::to_string(i) + " picked up forks");
    }

    // Putting down forks and checking if neighbors can eat
    void return_forks(int i)
    {
        state_mutex.acquire();
        state[i] = THINKING;
        log("Philosopher " + std::to_string(i) + " puts down forks and returns to thinking");

        test((i + n - 1) % n);
        test((i + 1) % n);
        state_mutex.release();
    }

    // Main philosopher function
    void philosopher_function(int id)
    {
        const int cycles = running.load() ? 10 : 3; // Number of thinking/eating cycles

        for (int i = 0; i < cycles && running.load(); i++)
        {
            // Thinking
            log("Philosopher " + std::to_string(id) + " is thinking");
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 700));

            if (!running.load())
                break;

            // Trying to pick up forks
            pickup_forks(id);

            if (!running.load())
            {
                return_forks(id);
                break;
            }

            // Eating
            log("Philosopher " + std::to_string(id) + " is eating");
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 700));

            // Putting down forks
            return_forks(id);
        }
    }

    // Safe simulation stop
    void stop()
    {
        running.store(false);
    }

    bool is_running() const
    {
        return running.load();
    }
};

int main(int argc, char *argv[])
{
    // Default values
    int num_philosophers = 5;
    int simulation_time = 30;

    if (argc == 1)
    {
        std::cout << "No arguments provided. Using default values: number of philosophers = 5, simulation time = 30s" << std::endl;
    }
    else if (argc == 2)
    {
        try
        {
            num_philosophers = std::stoi(argv[1]);
            if (num_philosophers <= 0)
            {
                throw std::invalid_argument("Number of philosophers must be positive");
            }
            std::cout << "Set number of philosophers = " << num_philosophers << ", simulation time = 30s (default)" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Usage: " << argv[0] << " [number of philosophers] [simulation time in seconds]" << std::endl;
            return 1;
        }
    }
    else if (argc >= 3)
    {
        try
        {
            num_philosophers = std::stoi(argv[1]);
            simulation_time = std::stoi(argv[2]);

            if (num_philosophers <= 0)
            {
                throw std::invalid_argument("Number of philosophers must be positive");
            }

            if (simulation_time <= 0)
            {
                throw std::invalid_argument("Simulation time must be positive");
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Usage: " << argv[0] << " [number of philosophers] [simulation time in seconds]" << std::endl;
            return 1;
        }
    }

    DiningTable table(num_philosophers);
    std::vector<std::thread> threads;

    // Create threads for philosophers
    for (int i = 0; i < num_philosophers; i++)
    {
        threads.push_back(std::thread([i, &table]()
                                      { table.philosopher_function(i); }));
    }

    // Run the simulation for the specified time
    std::cout << "Started simulation for " << simulation_time << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(simulation_time));

    // Stop the simulation
    table.stop();

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    std::cout << "Simulation completed." << std::endl;
    return 0;
}