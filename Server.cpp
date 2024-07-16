#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define SERVER_PORT 5000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define EPOLL_SIZE 10

int main() // (int argc, char* argv[])
{
    struct sockaddr_in server_addr;
    int server_socket;
    int opt = 1;

    bzero(&server_addr, sizeof(server_addr)); // 置字节字符串前n个字节为0，包括'\0'
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); // 转小端,INADDR_ANY就是指定地址为0.0.0.0的地址
    server_addr.sin_port = htons(SERVER_PORT);

    // 创建一个Socket
    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }


    // bind a socket
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        printf("Server Bind Port: %d Failed!\n", SERVER_PORT);
        exit(1);
    }

    // 监听Socket
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed!\n");
        exit(1);
    }

    int epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0){
        perror("Epoll Create");
        exit(-1);
    }

    struct epoll_event ev;
    struct epoll_event events[EPOLL_SIZE];
    ev.data.fd = server_socket;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,server_socket,&ev);
    fcntl(server_socket, F_SETFL, fcntl(server_socket,F_GETFD,0)| O_NONBLOCK);

    while(1)
    {
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (epoll_events_count < 0)
        {
            perror("epoll failed");
            break;
        }
        printf("epoll_events_count = %d\n", epoll_events_count);

        for (int i = 0;i < epoll_events_count;i++)
        {
            if (events[i].data.fd==server_socket&&(events[i].events & EPOLLIN))
            {
                struct sockaddr_in remote_addr;

                socklen_t socklen = sizeof(struct sockaddr_in);
                int client_socket = accept(server_socket, (struct sockaddr *)&remote_addr, &socklen);
                if (client_socket < 0)
                {
                    perror("accept error");
                }
                printf("client connection from: %s : % d(IP : port), "
                       "client_socket = %d \n",
                       inet_ntoa(remote_addr.sin_addr),
                       ntohs(remote_addr.sin_port),
                       client_socket);

                ev.data.fd = client_socket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev);

                fcntl(client_socket, F_SETFL,
                      fcntl(client_socket, F_GETFD, 0)| O_NONBLOCK);
            }else{
                char buffer[BUFFER_SIZE];
                int len=recv(events[i].data.fd,buffer,sizeof(buffer), 0);
                if(buffer[0] == 'S'){
                    close(server_socket);
                    return 0;
                }



                if (len > 0)
                {
                    //服务端主程序区
                    buffer[len] = '\0';
                    printf("Received message: %s, from fd: %d \n", buffer, events[i].data.fd);

                }
                else if (len == 0)
                {
                    printf("Client disconnected, fd: %d \n", events[i].data.fd);
                    close(events[i].data.fd);
                }
                else
                {
                    perror("Receive error");
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}