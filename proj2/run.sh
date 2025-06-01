#!/bin/bash

# Activate virtual environment
source venv/bin/activate

# Start server in background
python server.py &

# Wait for server to start
sleep 2

# Start 3 clients in new terminals
gnome-terminal -- bash -c "source venv/bin/activate; python client.py" &
gnome-terminal -- bash -c "source venv/bin/activate; python client.py" &
gnome-terminal -- bash -c "source venv/bin/activate; python client.py" &

echo "Chat system started"
wait