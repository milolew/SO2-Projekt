import socket
import threading
import sys

class ChatClient:
    """
    Chat client implementation that connects to the multi-threaded server.
    Handles sending and receiving messages in separate threads.
    """
    
    def __init__(self, host='localhost', port=9999):
        """
        Initialize the chat client with server host and port.
        
        Args:
            host (str): Server host address
            port (int): Server port number
        """
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.username = None
        self.receive_thread = None
        
    def connect(self):
        """
        Connect to the chat server and start sending/receiving messages.
        """
        try:
            self.client_socket.connect((self.host, self.port))
            print(f"Connected to server at {self.host}:{self.port}")
            
            # Username validation loop
            while True:
                self.username = input("Enter your username: ")
                self.client_socket.sendall(self.username.encode('utf-8'))
                
                response = self.client_socket.recv(1024).decode('utf-8')
                
                if response == "OK":
                    break
                else:
                    print(response)
            
            print(f"Welcome to the chat, {self.username}!")
            
            # Start a separate thread for receiving messages
            self.receive_thread = threading.Thread(target=self.receive_messages)
            self.receive_thread.daemon = True
            self.receive_thread.start()
            
            # Main thread will handle sending messages
            self.send_messages()
            
        except Exception as e:
            print(f"Error connecting to server: {e}")
        finally:
            self.client_socket.close()
            
    def receive_messages(self):
        """
        Receive and display messages from the server.
        This method runs in a separate thread.
        """
        try:
            while True:
                message = self.client_socket.recv(1024).decode('utf-8')
                if not message:
                    print("Disconnected from server")
                    break
                print(message)
        except Exception as e:
            print(f"Error receiving messages: {e}")
            
    def send_messages(self):
        """
        Get input from the user and send messages to the server.
        This method runs in the main thread.
        """
        try:
            print("You can start sending messages. Type 'exit' to quit.")
            while True:
                message = input()
                if message.lower() == 'exit':
                    break
                    
                # Send the message to the server
                self.client_socket.sendall(message.encode('utf-8'))
                
                # Display the sent message locally with special formatting
                print(f"# {message}")
                
        except Exception as e:
            print(f"Error sending messages: {e}")
            
if __name__ == "__main__":
    # You can specify server address and port as command line arguments
    server_host = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
    server_port = int(sys.argv[2]) if len(sys.argv) > 2 else 9999
    
    client = ChatClient(server_host, server_port)
    client.connect()