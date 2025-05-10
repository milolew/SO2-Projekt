#!/bin/bash

# Script to launch chat server and clients in separate GNOME terminals
# Usage: ./run.sh [-p port]

# Default port
PORT=9999

# Path to virtual environment
VENV_PATH="venv"

# Parse command line arguments
while getopts "p:" opt; do
  case $opt in
    p)
      PORT=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      echo "Usage: $0 [-p port]" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      echo "Usage: $0 [-p port]" >&2
      exit 1
      ;;
  esac
done

# Check if virtual environment exists
if [ ! -d "$VENV_PATH" ]; then
    echo "Error: Virtual environment directory '$VENV_PATH' not found"
    echo "Make sure you have created a virtual environment in the project directory"
    exit 1
fi

# Check if server.py and client.py exist
if [ ! -f "server.py" ]; then
    echo "Error: server.py not found in current directory"
    exit 1
fi

if [ ! -f "client.py" ]; then
    echo "Error: client.py not found in current directory"
    exit 1
fi

# Get current directory (for absolute paths)
CURRENT_DIR=$(pwd)

# Function to check if a port is available
check_port() {
    if netstat -tuln | grep -q ":$PORT "; then
        echo "Warning: Port $PORT is already in use!"
        echo "The server might fail to start. Consider using a different port."
        sleep 2
    fi
}

# Check port availability
check_port

# Launch server in a new terminal with virtual environment activated
echo "Starting server on port $PORT..."
gnome-terminal --title="Chat Server (Port $PORT)" -- bash -c "cd '$CURRENT_DIR'; source $VENV_PATH/bin/activate; python server.py $PORT; exec bash"

# Wait a bit for the server to start
echo "Waiting for server to initialize..."
sleep 2

# Launch a single client in a separate terminal for testing
echo "Starting a chat client..."
gnome-terminal --title="Chat Client (Port $PORT)" -- bash -c "cd '$CURRENT_DIR'; source $VENV_PATH/bin/activate; python client.py localhost $PORT; exec bash"

echo "Components launched. Server and client are now running."
echo "Server port: $PORT"