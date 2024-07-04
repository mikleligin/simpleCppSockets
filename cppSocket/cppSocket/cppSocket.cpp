#include <iostream>
#pragma once
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#define CONNECTIONS 100


SOCKET connections[CONNECTIONS];
int connector = 0;

void MsgHandler(int index)
{
    char msg[256];
    printf("Count of clients %d\n", index);
    //std::cout << index << std::endl;
    while (true)
    {
        int res = recv(connections[index], msg, sizeof(msg), NULL);
        if (res <= 0) {
            // Connection closed or error
            closesocket(connections[index]);
            return;
        }
        for (size_t i = 0; i < connector; i++)
        {
            if (i == index)
            {
                continue;
            }
            else
            {
                std::cout << msg;
                send(connections[i], msg, sizeof(msg), NULL);
            }
        }
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
    address.sin_port = htons(1233);
    address.sin_family = AF_INET;

    SOCKET listener = socket(AF_INET, SOCK_STREAM, NULL);
    // Связка сокета + лисенера
    //bind(listener, (SOCKADDR*)&address, size);

    if (bind(listener, (SOCKADDR*)&address, size) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(listener);
        return 1;
    }

    listen(listener, SOMAXCONN);
    SOCKET newConnection;
    for (size_t i = 0; i < CONNECTIONS; i++)
    {
        int c = 0;
        newConnection = accept(listener, (SOCKADDR*)&address, &size);
        if (newConnection == INVALID_SOCKET) {
            std::cerr << "Failed to accept connection. Error: " << WSAGetLastError() << std::endl;
            continue;
        }
        else {
            char msg[256] = "Welcome to my server";
            send(newConnection, msg, sizeof(msg), NULL);
            std::cout << "Client connected!, here is IP:";
            std::cout << inet_ntoa(address.sin_addr) << std::endl;
            connections[i] = newConnection;
            connector++;

            std::thread(MsgHandler, i).detach();
        }
    }
    WSACleanup();
    system("pause");
    return 0;
}
