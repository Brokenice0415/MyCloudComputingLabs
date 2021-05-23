#include "util/config.h"
#include "util/message.h"
#include "MyMap.h"
#include "socket.h"


using namespace std;

Config config;
int part_n;
Message msg_hander;


void server_work(int i){
	IP part = config.part[i];
	MyMap *myMap = new MyMap();
	vector<string> cmd;
	server_socket server(part.port); // start listen
	while(1){
		int fd = server.accept_remote();
		string msg = server.rcv_rpc(fd);
		cmd = msg_hander.get_array(msg);
		/*
		for(vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++) {
			cout<<"= "<<*it;
		}
		cout<<endl;
		*/
		//if(cmd[0] = "SET"){
		
		//}
	}

}

int main(int argc, char* argv[]) {
	if(argc == 3) {
		config.get_config(string(argv[1]));
	}else return 0;
	part_n = config.part.size();
	if(config.mode == PARTICIPANT){
		
		for(int i = 0; i < part_n; i ++){
			if(fork() == 0) {
				server_work(i);
				break;
			}
		}

	}else{
		client_socket client;
		client.connect_remote(config.part[0].ip, config.part[0].port);
		client.send_rpc(msg_hander.set_array(vector<string>(1, "SET A A")));
	}

	return 0;
}
