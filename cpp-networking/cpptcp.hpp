#pragma once
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <fstream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sstream>
#include <source_location>
#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <source_location>

#define DEBUG true

namespace cpptcp {
class TCPError : public std::runtime_error {
public:
  // no message
  explicit TCPError(
      const std::source_location& loc = std::source_location::current())
      : std::runtime_error(build_message("",loc)) {}
  // opt message
  explicit TCPError(
      const std::string& msg,
      const std::source_location& loc = std::source_location::current())
    : std::runtime_error(build_message(msg, loc)) {}
private:
  static std::string build_message(const std::string& msg,
      const std::source_location& loc){
    std::ostringstream oss;
    oss << loc.file_name() << ':' << loc.line()
      << "in [" << loc.function_name() << "]";
    if(!msg.empty())
      oss << " - " << msg;
    return oss.str();
  }
};
class TCPConfig{
public:
  bool TryBindRecursive(int i, int sock_int, sockaddr_in addr, size_t addr_sz){
    if(i < 0)
      throw TCPError("cast error i < 0?!");

    if(static_cast<size_t>(i) >= fallback_ports.size())
      throw TCPError("fallback ports out of range");
    
    int ret{};
    addr.sin_port = htons(fallback_ports[i]);
    ret = ::bind(sock_int, reinterpret_cast<sockaddr*>(&addr), addr_sz);

    if(ret == 0) {
      std::cout << std::format("Listening on port {}\n", fallback_ports[i]);
      return true;
    }
    if(errno != EADDRINUSE)
      throw TCPError();

    return TryBindRecursive(i + 1, sock_int, addr, addr_sz);
  }
private:
  std::vector<int> fallback_ports {
    8888,
    9999,
    7777,
    5050
  };
};
class Socket {
public:
    explicit Socket(int fd = -1) noexcept : fd_(fd) {}
    ~Socket() { if (fd_ >= 0) ::close(fd_); }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) ::close(fd_);
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept { return fd_; }
    [[nodiscard]] bool valid() const noexcept { return fd_ >= 0; }

private:
    int fd_;
};
  /*
   * Handles opened socket and all incoming connections
   * Forwards new connections to ClientHandler
   *
   */
  class TcpConnectionHandler {
  public:
    explicit TcpConnectionHandler(int port = 8080, int backlog = 128){
      // create config
      config_ = TCPConfig();
      
      sock_ = Socket(::socket(AF_INET, SOCK_STREAM, 0));
      if(!sock_.valid())
        throw TCPError("socket");

      int opt { 1 };
      ::setsockopt(sock_.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

      sockaddr_in addr {};
      addr.sin_family = AF_INET,
      addr.sin_port = htons(port),
      addr.sin_addr = { .s_addr = INADDR_ANY };

      // defining a return value for more readable code
      int ret {};
      size_t addr_sz = sizeof(addr);
      // port shared in trybindrecursive call to user
      ret = config_.TryBindRecursive(0, sock_.get(), addr, addr_sz);

      ret = ::listen(sock_.get(), backlog);
      if(ret < 0)
        throw TCPError("listen");
    }
    [[nodiscard]] Socket accept() const {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        int client_fd = ::accept(sock_.get(), reinterpret_cast<sockaddr*>(&client_addr), &len);
        if (client_fd < 0)
            throw TCPError();
        return Socket(client_fd);
    }

    [[nodiscard]] int fd() const noexcept { return sock_.get(); }  private:
private:
    Socket sock_;
    TCPConfig config_;
  };
  class ClientHandler {
  public:
#ifdef DEBUG
    void debug_display_traffic(){
      if(sbuff_.empty())
        return;
      std::cout << sbuff_ << "\n";
    }
#endif
    void handle_client(Socket client_,std::stop_token stop_token){
      char buf[1024];
      while(!stop_token.stop_requested()){
        ssize_t n = ::recv(client_.get(), buf, sizeof(buf), 0);
        if(n <= 0) break;

        sbuff_ = std::string(buf);
        
        // ensure null termination
        if(sbuff_[sbuff_.length()] != '\0')
          sbuff_ += '\0';  
        // copy to sbuff_
        //
#ifdef DEBUG
        debug_display_traffic();
#endif
      }
    }   
  private:
    std::string sbuff_{}; 
  };
}
