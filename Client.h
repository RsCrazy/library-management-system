#ifndef LIBRARY_MANAGEMENT_SYSTEM_CLIENT_H
#define LIBRARY_MANAGEMENT_SYSTEM_CLIENT_H

#endif  // LIBRARY_MANAGEMENT_SYSTEM_CLIENT_H

void Send_Message(const char* message, SOCKET ClientSocket)
{
    int ret = send(ClientSocket, message, (int)strlen(message), 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Send Information Failed! Error: %d\n", WSAGetLastError());
    }
    else
    {
        printf("Message sent: %s\n", message);
    }
}

void Recv_Message(char* recvBuffer, SOCKET ClientSocket)
{
    int ret;

    while (1) {
        memset(recvBuffer, 0, sizeof(&recvBuffer));
        ret = recv(ClientSocket, recvBuffer, 1024, 0);
        if(ret > 0) {
            printf("Received: %s\n", recvBuffer);
        }
        else if(ret == 0) {
            printf("Server closed connection.\n");
            break;
        }
        else {
            printf("Recv error: %d\n", WSAGetLastError());
            break;
        }
    }
}