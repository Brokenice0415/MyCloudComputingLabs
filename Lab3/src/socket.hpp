#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <vector>

using namespace std;

class client_socket {
    private:
        int sockfd;
        struct sockaddr_in addr;
        int ret;

    public:
        client_socket (string ip, int port) {
            while (true) {
                sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
                if (sockfd < 0) {
                    continue;
                }
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = inet_addr(ip);
                addr.sin_port = htons(port);
                ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
                if (ret < 0) {
                    continue;
                }
                break;
            }
            
        }

        void send_rpc (string msg) {
            while(1){
                ret = send(sockfd, msg.c_str(), msg.length(),0);
                if(ret < 0){
                    continue;
                }
            }
        }

        string rcv_rpc () {
            while (1) {
                char buf[1024] = {0};
                ret = recv(sockfd, buf, strlen(buf)-1,0);
                if(ret < 0){
                    continue;
                }
                else if(ret == 0)
                {
                    break;
                }
                break;
            }
            return buf;
        }
        

        void close_socket () {
            close(sockfd);
        }
};

class server_socket {
    private:
        int sockfd;
        uint32_t ip;
        uint16_t port;
        struct sockaddr_in addr;
        int ret;
        vector<int> new_socks;
    
    public:
        server_socket (string ip, int port) {
            sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            this->ip = inet_addr(ip);
            this->port = htons(port);
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = this->ip;
            addr.sin_port = this->port;
            ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
            ret = listren(sockfd, 1);
        }

        int rcv_rpc () {
            int new_sockfd = accept(sockfd, NULL, NULL);
            int pid = fork();
            if(pid == 0) {
                close(sockfd);
                char buf[1024] = { 0 };
                ret = recv(new_sockfd, buf, strlen(buf) - 1, 0);
                /*
                * do something

                */



                close(new_sockfd);
            }else{
                close(new_sockfd);
            }
        }

        void send_rpc (int new_sockfd, string msg) {
            ret = send(new_sockfd, msg.c_str(), msg.length(), 0);
        }

        void close_socket () {
            close(sockfd);
        }
};

#endif