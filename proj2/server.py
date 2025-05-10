import socket
import threading

class ChatServer:
    """
    Multi-threaded chat server implementation.
    Creates a separate thread for each client connection and
    ensures message synchronization between clients.
    """
    
    def __init__(self, host='localhost', port=9999):
        """
        Initialize the chat server with host and port.
        
        Args:
            host (str): Server host address
            port (int): Server port number
        """
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.clients = []  # List to store client connections
        self.lock = threading.Lock()  # Lock for synchronizing access to shared resources
        
    def start(self):
        """Start the server and listen for client connections."""
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        print(f"Server started on {self.host}:{self.port}")
        
        try:
            while True:
                client_socket, address = self.server_socket.accept()
                print(f"New connection from {address}")
                
                # Create a new thread for each client
                client_thread = threading.Thread(
                    target=self.handle_client,
                    args=(client_socket, address)
                )
                client_thread.daemon = True
                client_thread.start()
        except KeyboardInterrupt:
            print("Server is shutting down...")
        finally:
            self.server_socket.close()
    
    def is_username_taken(self, username):
        """
        Check if the username is already taken.
        
        Args:
            username (str): Username to check
            
        Returns:
            bool: True if username is taken, False otherwise
        """
        # This function is called within a lock context, so it's thread-safe
        return any(u == username for _, u in self.clients)
            
    def handle_client(self, client_socket, address):
        """
        Handle an individual client connection in a separate thread.
        
        Args:
            client_socket (socket): Socket object for client connection
            address (tuple): Client address information
        """
        username = None
        
        try:
            # Username validation loop
            while True:
                username = client_socket.recv(1024).decode('utf-8')
                if not username:
                    return  # Client disconnected
                
                # Check if username is taken (critical section)
                with self.lock:
                    if self.is_username_taken(username):
                        # Username is taken, ask for another one
                        client_socket.sendall("ERROR: Username already taken. Please choose another:".encode('utf-8'))
                    else:
                        # Username is available
                        client_socket.sendall("OK".encode('utf-8'))
                        break
            
            welcome_message = f"*** {username} has joined the chat ***"
            
            # Add client to the list (critical section)
            with self.lock:
                self.clients.append((client_socket, username))
                self.broadcast(welcome_message, client_socket)
            
            # Main loop for handling client messages
            while True:
                message = client_socket.recv(1024).decode('utf-8')
                if not message:
                    break
                
                # Broadcast the message (critical section)
                with self.lock:
                    formatted_message = f"{username}: {message}"
                    self.broadcast(formatted_message, client_socket)
                    
        except Exception as e:
            print(f"Error handling client {address}: {e}")
        finally:
            # Remove client from the list (critical section)
            with self.lock:
                if username:
                    self.clients = [(s, u) for s, u in self.clients if s != client_socket]
                    self.broadcast(f"*** {username} has left the chat ***", client_socket)
            
            client_socket.close()
            
    def broadcast(self, message, sender_socket):
        """
        Broadcast a message to all clients except the sender.
        
        Args:
            message (str): Message to broadcast
            sender_socket (socket): Socket of the message sender
        """
        # This method is called within a lock context from handle_client
        # so it's already synchronized
        for client_socket, _ in self.clients:
            # Don't send the message back to the sender
            if client_socket != sender_socket:
                try:
                    client_socket.sendall(message.encode('utf-8'))
                except Exception as e:
                    print(f"Error broadcasting message: {e}")
                    # Connection might be broken, but we'll handle it in handle_client
                    
if __name__ == "__main__":
    server = ChatServer()
    server.start()