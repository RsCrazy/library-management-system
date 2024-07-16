#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <winsock.h>
#include <winsock2.h>
#include <string.h>
#include "Client.h"

#pragma comment(lib, "ws2_32.lib")
#define PORT 5000
#define IP_ADDRESS "122.112.236.182"
#define BUFFER_SIZE 1024

SOCKET ClientSocket;
struct sockaddr_in ClientAddr;

void client()
{
    WSADATA wsaData;
    int ret;

    // 初始化Windows Socket
    WSAStartup(MAKEWORD(2, 2), &wsaData) != 0;

    // 创建一个套接口
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("Create Socket Failed! Error: %d\n", GetLastError());
        WSACleanup();
        return;
    }

    ClientAddr.sin_family = AF_INET;
    ClientAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    ClientAddr.sin_port = htons(PORT);
    memset(ClientAddr.sin_zero, 0, sizeof(ClientAddr.sin_zero));

    // 连接Socket
    ret = connect(ClientSocket, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr));
    if (ret == SOCKET_ERROR)
    {
        printf("Socket Connect Failed! Error: %d\n", GetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }
    else
    {
        printf("Socket Connect Succeed!\n");
    }

    while(1) {

        // UI 线程发送消息
        char sendBuffer[BUFFER_SIZE];
        while(1) {
            printf("Input Data: ");
            gets(sendBuffer);

            if(strcmp(sendBuffer, "q") == 0) {
                printf("Quit!\n");
                break;
            }

            Send_Message(sendBuffer, ClientSocket);
        }

        if(strcmp(sendBuffer, "q") == 0) {
            printf("Quit!\n");
            break;
        }
    }
    // 关闭socket
    closesocket(ClientSocket);
    WSACleanup();
    return;
}