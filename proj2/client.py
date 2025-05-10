import socket
import sys

print("This is client")

host = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
port = int(sys.argv[2]) if len(sys.argv) > 2 else 9999

print(f"Client configured to connect to {host}:{port}")

