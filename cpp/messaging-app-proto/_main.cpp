#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <stop_token>
#include <sys/socket.h>
#include <vector>
#include <thread>
#include <unistd.h>
#include <string>
#include <format>
const int XOR_KEY = 9;

// RAII wrapper
class Socket {
public:
  explicit Socket(int fd) : fd_(fd){}
  ~Socket(){
    if(fd_ >= 0) ::close(fd_);
  }
  [[nodiscard]] int get() const noexcept { return fd_;}
  [[nodiscard]] bool valid() const noexcept { return fd_ >= 0;}
private:
  int fd_;
};

class Listener{
public:
  explicit Listener(int port, int backlog = 128){
    sock = Socket(::socket(AF_INET, SOCK_STREAM, 0));
    int opt = 1;
    ::setsockopt(sock.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {.s_addr = INADDR_ANY},
    };

    if(::bind(sock.get(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
      throw std::runtime_error("bind vailed");
    
    if(::listen(sock.get(), backlog) < 0)
      throw std::runtime_error("listen failed");

    std::cout << std::format("Started listening on port {}\n", port);
  }
  [[nodiscard]] Socket accept() const {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(sock.get(), reinterpret_cast<sockaddr*>(&client_addr), &len);
    if(client_fd < 0)
      throw std::runtime_error("client accept failed\n");
    return Socket(client_fd);
  }

  [[nodiscard]] int fd() const noexcept { return sock.get();}
private:
  Socket sock;
};

void handle_client(std::stop_token stoken, Socket client){
  char buf[1024];
  while(!stoken.stop_requested()){
    ssize_t n = ::recv(client.get(), buf, sizeof(buf), 0);
    if(n <=0) break;
    ::send(client.get(), buf, static_cast<size_t>(n), 0);
  }
}

std::string mxor(std::string& input){
  for(size_t i {0}; i < input.length(); i++){
    input[i] = input[i] ^ XOR_KEY;
  } 
  return input;
}

int main()
{
  try {
    Listener listener(8899);
    std::vector<std::jthread> workers;

    while(true){
      Socket client = listener.accept();
      workers.emplace_back(handle_client, std::move(client));
    }
  } catch (const std::exception& e) {
    std::cerr << std::format("Fatal: {}\n", e.what());
    return 1;
  }
}
