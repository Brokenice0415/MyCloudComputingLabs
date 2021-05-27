#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory.h>

#include <vector>

using namespace std;

class client_socket {
    private:
        int sockfd;
        struct sockaddr_in addr;
        struct in_addr server_addr;
        char buf[BUFSIZ];
        fd_set fds;
        timeval timeout;

    public:
    	client_socket (){
    		memset(&addr, 0, sizeof(addr));
    		timeout = {5, 0};
    	}
    	
    	client_socket (string ip, int port){
    		timeout = {5, 0};
    		connect_remote(ip, port);
    	}
    	
        bool connect_remote(string ip, int port) {
            memset(&addr, 0, sizeof(addr));
			inet_aton(ip.c_str(), &server_addr);
            addr.sin_family = AF_INET;
            addr.sin_addr = server_addr;
            addr.sin_port = htons(port);
           
            
            if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
            	//printf("socket create error\n");
            	return false;
            }
            
            if(connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr))<0){
            	//printf("connect error\n");
            	return false;
            }
            //printf("connect to server %s\n", inet_ntoa(addr.sin_addr));
            return true;
        }

        void send_rpc (string msg) {
            int l = msg.length(); 
       		int ret;
            if((ret = send(sockfd, msg.c_str(), l, 0)) < l) {
            	msg = msg.substr(ret, l-ret);
            	l -= ret;
            	//printf("ret: %d\n", ret);
            }
        }

        string rcv_rpc () {
        	string ret = "";
            int rest;
            
            FD_ZERO(&fds);
            FD_SET(sockfd, &fds);
            
            int res = select(sockfd+1, &fds, NULL, NULL, &timeout);
            if(res > 0){
            	if(FD_ISSET(sockfd, &fds)){
            		if((rest = recv(sockfd, buf, BUFSIZ, 0)) > 0){
				    	ret += string(buf).substr(0, rest);
				    }
            	}
            }
            else if (res < 0) {
            	ret = "nil";
            }
            return ret;
        }
        

        void close_socket () {
            close(sockfd);
        }
};

class server_socket {
    private:
        int sockfd;
        uint16_t port;
        struct sockaddr_in addr;
        char buf[BUFSIZ];
        fd_set fds;
        timeval timeout;
    
    public:
    	server_socket () {
    		memset(&addr, 0, sizeof(addr));
    		timeout = {0, 0};
    	}
    	
    	server_socket (int port) {
    		timeout = {0, 0};
    		listen_remote (port);
    	}
    	
        void listen_remote (int port) {
        	memset(&addr, 0, sizeof(addr));
            
            this->port = htons(port);
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port = htons(port);
            
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            
            int on = 1;
            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
            	//printf("set error\n");
            	return;
            }
            if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            	//printf("bind error\n");
            	return;
            }
            if(listen(sockfd, 1)<0){
            	//printf("listen error\n");
            	return;
            }
            //printf("listen at %s:%d ...\n", inet_ntoa(addr.sin_addr), port);
        }
        
        int accept_remote() {
        	int sin_size = sizeof(struct sockaddr_in);
        	struct sockaddr_in remote_addr;
            int new_sockfd = accept(sockfd, (struct sockaddr*)&remote_addr, (socklen_t*)&sin_size);
            return new_sockfd;
        }

        string rcv_rpc (int new_sockfd) {
            string ret = "";
            int rest;

    		if((rest = recv(new_sockfd, buf, BUFSIZ, 0)) > 0){
		    	ret += string(buf).substr(0, rest);
		    }

            return ret;
        }

        void send_rpc (int new_sockfd, string msg) {
       		int l = msg.length(); 
       		int ret;
       		
       		
            if((ret = send(new_sockfd, msg.c_str(), l, 0)) < l) {
            	msg = msg.substr(ret, l-ret);
            	l -= ret;
            	//printf("ret: %d\n", ret);
            }
        }

        void close_socket (int fd) {
            close(fd);
        }
};

#endif

