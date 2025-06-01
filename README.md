# Dining Philosophers Problem - Project 1

## Project Description
This project implements the classic synchronization problem known as the "Dining Philosophers Problem" for the Operating Systems 2 course. The solution uses binary semaphores to coordinate access to shared resources (forks) and prevents deadlocks between threads representing philosophers.

## Problem Assumptions
- n philosophers sit around a circular table.
- Each philosopher alternately thinks and eats.
- Between each pair of adjacent philosophers, there is one fork.
- To eat a meal, a philosopher must pick up both forks adjacent to them.
- After finishing the meal, the philosopher puts down the forks and returns to thinking.

## Implementation Features
- Use of binary semaphores for synchronizing access to resources.
- Deadlock elimination through a solution based on Dijkstra's algorithm.
- Control of philosopher states (THINKING, HUNGRY, EATING).
- Safe termination of simulation at any time.
- Synchronized display of console messages.

## Program Compilation
The program can be compiled using a compiler that supports C++11 standard or newer, for example:
```bash
g++ main.cpp -o main
```

## Running the Program
The program can be run with the following optional parameters:
```bash
./main [number_of_philosophers] [simulation_time_in_seconds]
```
Where:
- `number_of_philosophers` - number of philosophers participating in the simulation (default 5)
- `simulation_time_in_seconds` - simulation duration in seconds (default 30)

Examples:
```bash
# Run with default parameters (5 philosophers, 30 seconds)
./main

# Run with 7 philosophers and default time
./main 7

# Run with 10 philosophers and 45 seconds
./main 10 45
```

## Operating Mechanism
1. Each philosopher is represented by a separate thread.
2. Philosophers cyclically go through thinking and eating phases.
3. Before eating, a philosopher must pick up both forks (left and right).
4. Forks are represented by binary semaphores.
5. Each philosopher's state is controlled to prevent deadlocks.
6. The implementation is based on Dijkstra's algorithm, which guarantees no deadlock and prevents philosopher starvation.

## Deadlock Prevention
The program implements a deadlock prevention strategy through:
- Checking availability of both forks before attempting to pick them up.
- Using a `state_mutex` semaphore to protect the critical section during philosopher state changes.
- Checking the state of neighboring philosophers after releasing forks.


# Multi-threaded Chat Application - Project 2

A simple multi-threaded chat server and client implementation in Python using sockets and threading.

## Features

- Multi-threaded server supporting multiple concurrent clients
- Username validation and duplicate prevention
- Real-time message broadcasting to all connected clients
- Join/leave notifications
- Thread-safe client management
- Simple command-line interface

## Requirements

- Python 3.x
- Virtual environment (recommended)

## Setup

1. Clone or download the project files
2. Create a virtual environment:
   ```bash
   python -m venv venv
   ```
3. Activate the virtual environment:
   ```bash
   source venv/bin/activate  # On Linux/Mac
   # or
   venv\Scripts\activate     # On Windows
   ```

## Files

- `server.py` - Multi-threaded chat server
- `client.py` - Chat client implementation
- `run_chat.sh` - Bash script to launch server and 3 clients automatically

## Usage

### Manual Start

1. Activate virtual environment:
   ```bash
   source venv/bin/activate
   ```

2. Start the server:
   ```bash
   python server.py
   ```

3. In separate terminals, start clients:
   ```bash
   source venv/bin/activate
   python client.py
   ```

### Automatic Start (Linux/Mac)

1. Make the script executable:
   ```bash
   chmod +x run.sh
   ```

2. Run the launcher script:
   ```bash
   ./run.sh
   ```

This will automatically start the server and open 3 client terminals.

## How to Use

1. When starting a client, enter a unique username
2. Start typing messages and press Enter to send
3. Type `exit` to disconnect from the chat
4. Use Ctrl+C to stop the server

## Server Configuration

Default configuration:
- Host: `localhost`
- Port: `9999`
- Max connections: 5

You can modify these settings in the `ChatServer` class constructor.

## Client Configuration

Clients can connect to different servers by modifying the host and port in `client.py` or by passing command line arguments:

```bash
python client.py <server_host> <server_port>
```

# Author
Miłosz Lewandowski