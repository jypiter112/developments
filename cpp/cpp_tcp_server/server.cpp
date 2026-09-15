#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class Server{
private:
    bool isOnline;
    SOCKET server;
    SOCKET lastClient;
public:
    ~Server();
    inline int CreateServer(int);
    inline int SendFile(string&);
    bool IsOnline();
    bool HasClient();
    void Listen();
    void Accept();
    SOCKET* GetServer();
    SOCKET* GetClient();
};

Server::~Server(){
    cout << "Destructor called\n";
    if(this->lastClient) closesocket(this->lastClient);
    if(this->server) closesocket(this->server);
    WSACleanup();
}
inline int Server::CreateServer(int port){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d", WSAGetLastError());
    }
    else {
        printf("WsaStartup done.\n");
    }
    this->server = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // bind to 0.0.0.0
    addr.sin_port = htons(port);
    if (bind(this->server, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(this->server);
        WSACleanup();
        return 1;
    }
    cout << "Server online, listening...\n";
    this->isOnline = true;
    return 0;
}
void Server::Listen(){
    if (listen(this->server, 10) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        delete this;
    }
}
void Server::Accept(){
    this->lastClient = accept(this->server, NULL, NULL);
    if (this->lastClient == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(this->server);
        delete this;
    }
}
bool Server::HasClient(){
    if(this->lastClient == INVALID_SOCKET){
        return false;
    }
    return true;
}
SOCKET* Server::GetServer(){
    return &this->server;
}
SOCKET* Server::GetClient(){
    return &this->lastClient;
}
bool Server::IsOnline(){
    return this->isOnline;
}
inline int SendFile(string&);

int main(int argc, char** argv){
    Server* host = new Server();
    host->CreateServer(8080);
    // Listen loop
    int recvResult;
    char raw_req[256] = { 0 };
    string request;
    while(host->IsOnline()) {
        cout << "\n ... Trying listen/accept ...\n";
        host->Listen();
        host->Accept();
        if(!host->HasClient()){
            cout << "Try again\n";
            continue;
        }
        while(host->HasClient()){
            recvResult = recv(*host->GetClient(), raw_req, sizeof(raw_req) - 1, 0);
            if(recvResult > 0) {
                raw_req[recvResult] = '\0';
                request = raw_req;
                send(*host->GetClient(), raw_req, sizeof(raw_req), 0);
                cout << request << "\n";
            } else {
                cout << "Conn closed\n";
                closesocket(*host->GetClient());
                break;
            }
        }
    }
    delete host;
}
