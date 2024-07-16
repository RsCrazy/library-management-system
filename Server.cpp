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
#include "interface.h"//这里包了interface的头文件
#define SERVER_PORT 5000
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define EPOLL_SIZE 10
Connection con;//创建接口数据结构
void Login(const char* buffer,int cur,string& _buffer) {
    _buffer+="login ";
    while(buffer[cur] == ' ') {
        cur++;
    }
    string usrname,usrpwd;
    while(buffer[cur] != ' ') {
        usrname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        usrpwd+=buffer[cur];
        cur++;
    }//get username and passwd from agreement
    Users* pusr = con.usrrigister(usrname,usrpwd);
    Manager* pma = con.Marigister(usrname,usrpwd);
    if(pusr == nullptr && pma == nullptr) _buffer+="fail pass-err";
    else if(pusr != nullptr) _buffer+="success user";
    else _buffer+="success admin";
}
void Reg(const char* buffer,int cur,string& _buffer) {
    _buffer+="reg ";
    while(buffer[cur] == ' ') {
        cur++;
    }
    string usrname,usrpwd;
    while(buffer[cur] != ' ') {
        usrname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        usrpwd+=buffer[cur];
        cur++;
    }//get username and passwd from agreement
    con.usrrigister(usrname,usrpwd);
    _buffer+="success";
}
void Borrow(const char* buffer,int cur,string& _buffer) {
    _buffer+="borrow ";
    while(buffer[cur] == ' ') {
        cur++;
    }
    string usrname,usrpwd,Bookname,type;
    while(buffer[cur] != ' ') {
        usrname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ') {
        usrpwd+=buffer[cur];
        cur++;
    }//get username and passwd from agreement
    while(buffer[cur] == ' '){
        cur++;
    }
    while(buffer[cur] != ' ') {
        Bookname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' '){
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        type+=buffer[cur];
        cur++;
    }
    Users* pusr = con.usrrigister(usrname,usrpwd);
    bool ret = con.usrborrow(*pusr,type,Bookname);
    if(ret) _buffer+="success";
    else _buffer+="fail";
}
void Check(const char* buffer,int cur,string& _buffer) {
    _buffer+="check ";
    while(buffer[cur] == ' ') {
        cur++;
    }
    string bookname,type;
    while(buffer[cur] != ' ') {
        bookname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        type+=buffer[cur];
        cur++;
    }
    const BookInfor* pbi = con.search_by_usr(con.getRoot(),type,bookname);
    if(pbi == nullptr) _buffer+="fail";
    else {
        _buffer += "success ";
        _buffer += pbi->_name;
        _buffer += ' ';
        _buffer += pbi->_type;
        _buffer += ' ';
        if(pbi->_system == ::in) _buffer+="in";
        else _buffer+="leave";
        _buffer += " ";
        _buffer += pbi->_writer;
    }
}
void Return(const char* buffer,int cur,string& _buffer) {
    _buffer += "return ";
    while(buffer[cur] == ' ') {
        cur++;
    }
    string usrname,usrpwd;
    while(buffer[cur] != ' ') {
        usrname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        usrpwd+=buffer[cur];
        cur++;
    }//get username and passwd from agreement
    Users* pusr = con.usrrigister(usrname,usrpwd);
    bool ret = con.returnBook(*pusr);
    if(ret) _buffer += "success";
    else _buffer += "fail";
}
void Overview(const char* buffer,int cur,string& _buffer) {
    _buffer += "overview ";
    vector<BookInfor> vbi = con.getAllBookInfor(vbi);
    int size = vbi.size();
    stack<char> sk;
    while(size > 0) {
        sk.push(size%10 + '0');
        size/=0;
    }
    while(!sk.empty()) {
        _buffer.push_back(sk.top());
        sk.pop();
    }
    _buffer.push_back('\n');
    for(auto& e : vbi) {
        _buffer+=e._name;
        _buffer+=' ';
        _buffer+=e._type;
        if(e._system == ::in) _buffer+="in";
        else _buffer+="leave";
        _buffer+=' ';
        _buffer+=e._writer;
        _buffer+='\n';
    }
}
void Add(const char* buffer,int cur,string& _buffer) {
    _buffer+="add ";
    string name,type,writer;
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ') {
        name+=buffer[cur];
        cur++;
    }

    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ') {
        writer += buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' ') {
        cur++;
    }
    while(buffer[cur] != ' ' && buffer[cur] != '!') {
        type += buffer[cur];
        cur++;
    }
    bool ret = con.push_new_book(con.getRoot(),{name,type,writer});
    if(ret) _buffer+="success";
    else _buffer+="fail";
}
void Del(const char* buffer,int cur,string& _buffer) {
    _buffer+="del ";
    string bookname,type;
    while(buffer[cur] == ' '){cur++;}
    while(buffer[cur] != ' ') {
        bookname+=buffer[cur];
        cur++;
    }
    while(buffer[cur] == ' '){cur++;}
    while(buffer[cur] != '!' && buffer[cur] != ' ') {
        type+=buffer[cur];
        cur++;
    }
    bool ret = con.pop_old_book(con.getRoot(),type,bookname);
    if(ret) _buffer+="success";
    else _buffer+="fail";
}

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
                    string content;
                    int cur = 1;//buff[0]存储是服务器的配置的信息，从pos = 1开始执行协议
                    while(buffer[cur] != ' ') {
                        content+=buffer[cur];
                        cur++;
                    }//通过这里可以获得协议的事件内容
                    string _buffer;//返回的buffer
                    switch(content) {
                        case "login": Login(buffer,cur,_buffer);break;
                        case "reg": Reg(buffer,cur,_buffer);break;
                        case "borrow": Borrow(buffer,cur,_buffer);break;
                        case "check":Check(buffer,cur,_buffer);
                        case "return":Return(buffer,cur,_buffer);break;
                        case "overview":Overview(buffer,cur,_buffer);break;
                        case "add":Add(buffer,cur,_buffer);break;
                        case "del":Del(buffer,cur,_buffer);break;
                       //没有图书修改这个功能
                        //Usr和Manager的图书的总览一致，因此只用overview即可
                    default:_buffer+="cin error";
                    }//不同的登录的情况只需要看_buffer就行了
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