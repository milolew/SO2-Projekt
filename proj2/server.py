import socket
import sys

print("This is server")

port = int(sys.argv[1]) if len(sys.argv) > 1 else 9999
host = 'localhost'

print(f"Server configured to run on {host}:{port}")

