#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace
{
    SOCKET g_ClientSocket;
}

int main()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed with error: " + std::to_string(result) + "\n");
    }

    addrinfo addrInfo = {};
    addrInfo.ai_family = AF_INET;
    addrInfo.ai_protocol = IPPROTO_TCP;
    addrInfo.ai_socktype = SOCK_STREAM;

    addrinfo* addrInfoResult = nullptr;
    result = getaddrinfo("frios2.fri.uniza.sk", "16691", &addrInfo, &addrInfoResult);
    if (result != 0) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(result) + "\n");
    }

    SOCKET connectSocket = socket(addrInfoResult->ai_family, addrInfoResult->ai_socktype, addrInfoResult->ai_protocol);
    if (connectSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("socket failed with error: " + std::to_string(WSAGetLastError()) + "\n");
    }

    result = connect(connectSocket, addrInfoResult->ai_addr, (int)addrInfoResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    if (connectSocket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server\n";
        WSACleanup();
        throw std::runtime_error("Unable to connect to server.\n");
    }
    freeaddrinfo(addrInfoResult);
    g_ClientSocket = connectSocket;

    char buffer[256];
    recv(g_ClientSocket, buffer, sizeof(buffer), 0);
    std::cout << buffer << std::endl;

    return 0;
}