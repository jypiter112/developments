import socket

HOST = "localhost"
PORT = 8080
MESSAGE = "ng</username>testuser</username></msg>Lorem ipsum ...</msg>"

def send_message():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((HOST, PORT))
        sock.sendall(MESSAGE.encode("utf-8"))
        print(f"Sent {len(MESSAGE)} bytes to {HOST}:{PORT}")

        # Optional: read a response if the server sends one
        try:
            sock.settimeout(2)
            data = sock.recv(4096)
            if data:
                print("Received:", data.decode("utf-8", errors="replace"))
        except socket.timeout:
            print("No response received (timed out).")

if __name__ == "__main__":
    send_message()
