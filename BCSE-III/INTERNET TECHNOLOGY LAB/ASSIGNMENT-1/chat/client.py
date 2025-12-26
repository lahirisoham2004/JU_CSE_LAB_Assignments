import socket
import threading
from tkinter import Tk, Toplevel, Text, Entry, Button, Scrollbar, messagebox, Label, END, filedialog
from cryptography.fernet import Fernet
import os

# Server Configuration
HOST = '127.0.0.1'
PORT = 12346


class ChatClient:
    def __init__(self, root, client_socket, cipher, username):
        self.root = root
        self.root.title("Chat Client - {}".format(username))  # Display username in title

        self.chat_display = Text(root, state="disabled", height=20, width=50)
        self.chat_display.pack()

        scrollbar = Scrollbar(root, command=self.chat_display.yview)
        scrollbar.pack(side="right", fill="y")
        self.chat_display["yscrollcommand"] = scrollbar.set

        self.message_entry = Entry(root)
        self.message_entry.pack()

        self.send_button = Button(root, text="Send", command=self.send_message)
        self.send_button.pack()

        self.send_file_button = Button(root, text="Send File", command=self.send_file)
        self.send_file_button.pack()

        self.client_socket = client_socket
        self.cipher = cipher
        self.username = username  # Store the username
        self.recipient = None  # Add recipient attribute

        threading.Thread(target=self.receive_messages, daemon=True).start()


    def send_message(self):
        """Send a message."""
        message = self.message_entry.get().strip()
        if message:
            # Check if the message is a direct message
            if ":" in message:
                self.recipient = message.split(":")[0]  # Extract recipient from message
            try:
                self.client_socket.send(self.cipher.encrypt(message.encode()))
                self.message_entry.delete(0, END)
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send message: {e}")  # More specific error
        else:
            messagebox.showwarning("Warning", "Message cannot be empty!")

    def send_file(self):
        """Send a file."""
        filename = filedialog.askopenfilename(initialdir=".", title="Select a File")
        if filename:
            try:
                filesize = os.path.getsize(filename)

                if filesize > 1024 * 1024:
                    messagebox.showerror("Error", "File size exceeds 1MB limit.")
                    return


                with open(filename, "rb") as file:
                    file_data = file.read()

                # Get recipient
                recipient = self.recipient if self.recipient else "all" # Default to all if no recipient

                # Send file information: recipient:filename:filesize
                file_info = f"{recipient}:{os.path.basename(filename)}:{filesize}"
                self.client_socket.send(self.cipher.encrypt(f"FILE:{file_info}".encode()))  # Prefix with "FILE:"

                # Send file data
                self.client_socket.send(file_data)

                messagebox.showinfo("Info", f"File '{os.path.basename(filename)}' sent successfully.")

            except Exception as e:
                messagebox.showerror("Error", f"Failed to send file: {e}")



    def receive_messages(self):
        """Receive messages."""
        while True:
            try:
                encrypted_message = self.client_socket.recv(1024)
                if not encrypted_message:  # Handle disconnection
                    break
                message = self.cipher.decrypt(encrypted_message).decode()
                self.chat_display.config(state="normal")
                self.chat_display.insert(END, f"{message}\n")
                self.chat_display.config(state="disabled")
                self.chat_display.see(END)
            except Exception as e:
                print(f"Error receiving message: {e}")  # Print error for debugging
                break  # Exit loop on error
        self.close_connection()  # Call close_connection on error or disconnection

    def close_connection(self):
        """Close the connection to the server."""
        try:
            self.client_socket.close()
        except Exception as e:
            print(f"Error closing connection: {e}")
        messagebox.showinfo("Info", "Disconnected from server.")
        self.root.destroy()  # Close the chat window


class LoginWindow:
    def __init__(self, root):
        self.root = root
        self.root.title("Login")

        Label(root, text="Username:").pack()
        self.username_entry = Entry(root)
        self.username_entry.pack()

        Label(root, text="Password:").pack()
        self.password_entry = Entry(root, show="*")
        self.password_entry.pack()

        self.login_button = Button(root, text="Login", command=self.authenticate)
        self.login_button.pack()

        self.client_socket = None
        self.cipher = None
        self.username = None  # Store the username

    def connect_to_server(self):
        """Connect to the server."""
        try:
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.connect((HOST, PORT))

            # Receive encryption key
            key = self.client_socket.recv(1024)
            self.cipher = Fernet(key)
        except Exception as e:
            messagebox.showerror("Error", f"Connection failed: {e}")
            self.root.quit()

    def authenticate(self):
        """Authenticate the user."""
        username = self.username_entry.get().strip()
        password = self.password_entry.get().strip()
        self.username = username  # Store username

        if not username or not password:
            messagebox.showwarning("Warning", "Both fields are required!")
            return

        try:
            self.client_socket.send(f"{username}:{password}".encode())
            response = self.client_socket.recv(1024).decode()

            if response == "INVALID":
                messagebox.showerror("Error", "Invalid credentials!")
            else:
                messagebox.showinfo("Success", "Login successful!")
                self.root.destroy()

                main_window = Tk()
                chat_client = ChatClient(main_window, self.client_socket, self.cipher, username)
                main_window.protocol("WM_DELETE_WINDOW", chat_client.close_connection)  # Handle window close
                main_window.mainloop()
        except Exception as e:
            messagebox.showerror("Error", f"Authentication failed: {e}")


if __name__ == "__main__":
    root = Tk()
    login = LoginWindow(root)
    login.connect_to_server()
    root.mainloop()
