#include "include/nlohmann/json.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

class JsonHandler {
public:
  void read_json(const nlohmann::json &j) {
    j.at("port").get_to(app_config.port);
    j.at("username").get_to(app_config.username);
    j.at("debug").get_to(app_config.debug);
  }
  // make getters and setters
  struct AppConfig {
    int port;
    bool debug;
    std::string username;
  } app_config;
};

class MessageHandler;
// RAII wrapper — closes the fd no matter how we exit scope.
class Socket {
public:
  explicit Socket(int fd = -1) noexcept : fd_(fd) {}
  ~Socket() {
    if (fd_ >= 0)
      ::close(fd_);
  }

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  Socket(Socket &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
  Socket &operator=(Socket &&other) noexcept {
    if (this != &other) {
      if (fd_ >= 0)
        ::close(fd_);
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

class TcpListener {
public:
  explicit TcpListener(int port, int backlog = 128) {
    // load config
    sock_ = Socket(::socket(AF_INET, SOCK_STREAM, 0));
    if (!sock_.valid())
      throw std::runtime_error(
          std::format("socket() failed: {}", std::strerror(errno)));

    int opt = 1;
    ::setsockopt(sock_.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {.s_addr = INADDR_ANY},
    };

    if (::bind(sock_.get(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) <
        0)
      throw std::runtime_error(
          std::format("bind() failed: {}", std::strerror(errno)));

    if (::listen(sock_.get(), backlog) < 0)
      throw std::runtime_error(
          std::format("listen() failed: {}", std::strerror(errno)));

    std::cout << std::format("Listening on port {}\n", port);
  }

  [[nodiscard]] Socket accept() const {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd =
        ::accept(sock_.get(), reinterpret_cast<sockaddr *>(&client_addr), &len);
    if (client_fd < 0)
      throw std::runtime_error(
          std::format("accept() failed: {}", std::strerror(errno)));
    return Socket(client_fd);
  }

  [[nodiscard]] int fd() const noexcept { return sock_.get(); }

private:
  void LoadConfig() {
    std::ifstream file("config.json");
    if (!file.is_open())
      throw std::runtime_error("Could not load config.");

    nlohmann::json config{};
    try {
      file >> config;
    } catch (const nlohmann::json::parse_error &e) {
      throw std::runtime_error(std::format("Json parse error, {}\n", e.what()));
    }
    std::cout << "[info] Config loaded ok!\n";
    file.close(); // unsure if necessary with ifstream
  }
  JsonHandler json_handler;
  Socket sock_;
};

class MessageHandler {
public:
  bool verify_msg(std::string &magic, std::string &uname, std::string &msg) {
    if (magic != magic_ident)
      return false;
    if (uname.empty() || msg.empty())
      return false;
    return true;
  }
  void send_msg(Socket &client) {
    std::string formatted_msg =
        std::format("{}{}{}{}{}{}{}", magic_ident, uname_ident, my_username,
                    uname_ident, msg_ident, my_msg, msg_ident);
    if ((::send(client.get(), formatted_msg.c_str(), formatted_msg.length() + 1,
                0)) < 0)
      std::cout << "[Error] Message couldnt be sent\n";
    // on success print on our screen too
    std::cout << formatted_msg << "\n";
  }
  bool send_message_recieved(int client_int, bool ok = true) {
    if (!ok)
      return false;
    const size_t msg_ok_len = (message_recieved_ok.length() + 1);
    if ((::send(client_int, message_recieved_ok.c_str(), msg_ok_len, 0)) < 0) {
      std::cout << "[info] Msg ok, send failed. Connection lost?\n";
      return false;
    }
    return true;
  }
  void print_msg(int client_int, std::string &buff) {
    std::vector<std::string> tokens;
    for (auto &piece : split(buff, uname_ident))
      for (auto &piece2 : split(piece, msg_ident))
        if (!piece2.empty())
          tokens.push_back(piece2);
    // verify
    if (tokens.size() < 3)
      return;

    // verify msg
    last_msg = tokens[2];
    if (!verify_msg(tokens[0], tokens[1], last_msg))
      return;
    // send ok back
    send_message_recieved(client_int);
    // print message
    std::cout << std::format("[{}] ", tokens[1]);
    std::cout << last_msg << "\n";
  }

private:
  std::string xxor(std::string msg) {
    for (int i{0}; i < msg.length(); i++) {
      msg[i] = msg[i] ^ key;
    }
    return msg;
  }

  std::vector<std::string> split(std::string_view input,
                                 std::string_view delim) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t pos;

    while ((pos = input.find(delim, start)) != std::string_view::npos) {
      result.emplace_back(input.substr(start, pos - start));
      start = pos + delim.size();
    }
    result.emplace_back(input.substr(start)); // remainder after last delimiter

    return result;
  }
  std::string my_msg{"Hello world"};
  std::string my_username = {"admin"};
  std::string last_msg{};
  const int key{7};
  const std::string uname_ident = "</username>";
  const std::string msg_ident = "</msg>";
  const std::string magic_ident{"ng"};
  const std::string message_recieved_ok{"ng</info>MSG OK</info>"};
};

void handle_client(std::stop_token stoken, Socket client) {
  MessageHandler handler{};
  std::string sbuff{};
  char buf[1024];
  while (!stoken.stop_requested()) {
    ssize_t n = ::recv(client.get(), buf, sizeof(buf), 0);
    if (n <= 0)
      break; // closed or error

    sbuff = std::string(buf);
    handler.print_msg(client.get(), sbuff);
  }
}

int main() {
  try {
    TcpListener listener(8080);
    std::vector<std::jthread> workers;

    while (true) {
      Socket client = listener.accept();
      // jthread: auto-joins on destruction, supports cooperative
      // cancellation via std::stop_token — no manual thread bookkeeping.
      workers.emplace_back(handle_client, std::move(client));
    }
  } catch (const std::exception &e) {
    std::cerr << std::format("Fatal: {}\n", e.what());
    return 1;
  }
}
