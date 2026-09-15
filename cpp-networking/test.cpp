#include "./cpptcp.hpp"
#include <thread>

int main (int argc, char *argv[]) {
  try {
    cpptcp::TcpConnectionHandler listener();
    std::vector<std::jthread> active_connections;

    while(true) {
      cpptcp::Socket client = listener.accept();
    }
  }
  return 0;
}
