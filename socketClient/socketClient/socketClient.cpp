#include <iostream>
#pragma once
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
SOCKET connection;

void ClientHandler()
{
    char msg[256];
    while (true)
    {
        recv(connection, msg, sizeof(msg), NULL);
        std::cout << msg <<std::endl;
    }
}
int main() {

    // Запрашиваем библиотеку ws 
    WSADATA wsaData;
    WORD DLLver = MAKEWORD(2, 1);
    if (WSAStartup(DLLver, &wsaData) != 0) {
        std::cout << "Library not installed";
        exit(1);
    }

    // Бам бам сокет
    SOCKADDR_IN address;
    int size = sizeof(address);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(1234);
    address.sin_family = AF_INET;

    connection = socket(AF_INET, SOCK_STREAM, NULL);
    connect(connection, (SOCKADDR*)&address, size);
    if (!connect) {
        std::cout << "Error connection\n";
    }
    std::cout << "Connected\n";
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
    char msgC[256];
    while (true)
    {
        std::cin.getline(msgC, sizeof(msgC));
        send(connection, msgC, sizeof(msgC), NULL);
        Sleep(10);
    }
    system("pause");
    return 0;

}