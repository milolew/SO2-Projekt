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

## Thread Synchronization

### Server Threading Model
- **One thread per client**: Each client connection runs in a separate daemon thread
- **Main thread**: Handles incoming connections and creates client threads
- **Shared resource**: Client list (`self.clients`) containing all connected users

### Synchronization Mechanism
- **Threading Lock** (`self.lock`): Protects access to shared client list
- **Critical sections**: Username validation, client addition/removal, message broadcasting
- **Thread safety**: All operations on shared data use `with self.lock:` context manager

### Client Threading Model
- **Receive thread**: Dedicated thread for receiving messages from server
- **Main thread**: Handles user input and sending messages

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