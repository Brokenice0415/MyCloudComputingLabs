#include "util/config.h"
#include "util/message.h"
#include "mystore.h"
#include "socket.h"

#include <stdlib.h>

/*
 * two-phase waiting time limit /ms
 */
int TIMEOUT = 300;


using namespace std;

Config config;
int part_n;
Message msg_hander;


void server_work(int i){
	IP part = config.part[i];
	MyStore *myStore = new MyStore();
	vector<string> cmd;
	server_socket server(part.port); // start listen
	/*
	 *  new thread to :
	 *	periodically send heartbeat
	 *  if no connect with client -> ignore 
	 */
	while(1){
		int fd = server.accept_remote();
		//cout<<"receive new connect\n";
		
		/*
		 *  the code logic of receive needs to be polished
		 *	- if timeout in the period of response to "PRE"
		 *	- the state would be in a wrong place
		 */
		
		bool phase2 = false;
		
		while(1){
			
			string msg = server.rcv_rpc(fd);
			//cout<<msg<<endl;
			/*
			 * recieve error
			 */
			if(msg == "nil"){
				close(fd);
				break;
			}
			else if (msg == "") {
				break;
			}
			cmd = msg_hander.get_array(msg);
			
			/*
			for(vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++) {
				cout<<*it<<endl;
			}
			*/
			
			/*
			cout<<"fall asleep\n";
			sleep(rand()%2);
			cout<<"recover\n";
			*/
			
			if (phase2 == false) {
				/*
				 * do two-phase commit 
				 */
				if (cmd[0] == "PRE") {
					server.send_rpc(fd, msg_hander.set_str("ACK"));
					phase2 = true;
				}
				else if(cmd[0] == "ABORT") {
					server.send_rpc(fd, msg_hander.set_str("ACK"));
					continue;
				}
				else {
					//cout<<"no command\n";
					continue;
				}
				/**/
			}
			else {
				
				/*
				 * handle cmd
				 */
				
				/*
				cout<<"fall asleep\n";
				sleep(rand()%3);
				cout<<"recover\n";
				*/
				
				/*
				msg = server.rcv_rpc(fd);
				cmd = msg_hander.get_array(msg);
				*/
				//cout<<msg<<endl;
				
				 
				if(cmd[0] == "SET"){
					phase2 = false;
					string value = *(cmd.begin() + 2);
					for(vector<string>::iterator it = cmd.begin() + 3; it != cmd.end(); it ++) {
						value += " " + *it;
					}
					myStore->set(cmd[1], value);
					server.send_rpc(fd, msg_hander.set_success("OK"));
				}
				else if(cmd[0] == "GET"){
					phase2 = false;
					vector<string> value = myStore->get(cmd[1]);
					server.send_rpc(fd, msg_hander.set_array(value));
				}
				else if (cmd[0] == "DEL"){
					phase2 = false;
					int del_count = 0;
					for(vector<string>::iterator it = cmd.begin()+1; it != cmd.end(); it ++) {
						if(myStore->del(*it)) {
							del_count ++;
						}
					}
					if(del_count == 0){
						server.send_rpc(fd, msg_hander.set_error("ERROR"));
					}else{
						server.send_rpc(fd, msg_hander.set_int(del_count));
					}
				}
				else if(cmd[0] == "ABORT") {
					phase2 = false;
					server.send_rpc(fd, msg_hander.set_str("ACK"));
					//break;
				}
				else {
					/*
					 * if receive another phase1 command
					 * the coordinator may be restarted
					 * drop the work before and back to phase1
					 */
					phase2 = false;
					server.send_rpc(fd, msg_hander.set_error("ERROR"));
				}
				/**/
				
				//cout<<"done\n";
				/*
				cmd = myStore->get("asd");
				for(vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++) {
					cout<<*it<<endl;
				}
				*/
			}
		}
	}

}

int main(int argc, char* argv[]) {
	if(argc == 3) {
		config.get_config(string(argv[2]));
	}else return 0;
	
	int pid = fork();
	if(pid != 0) exit(0);
	
	part_n = config.part.size();
	if(config.mode == PARTICIPANT){
		server_work(0);
	}else{
		vector<client_socket> clients(part_n);
		
		
		/*
		 *  new thread to :
		 *	periodically receive heartbeat with TIMEOUT
		 *  otherwise remove participant
		 *  new participant's heartbeat leads to renew config
		 */

		/*
		 *	while(1):
		 *  hander cmd from shell
		 *  1. send "PRE" to all participant
		 *  2. new thread to do timing
		 *  2. if receiving more than half of "ACK", send command to all participant
		 *  3. if TIMEOUT occur to receiving, send "ABORT"
		 */
		 server_socket server(config.coor.port);
		 while(1){
		 	 int shell_fd = server.accept_remote();
		 	 string shell_msg = server.rcv_rpc(shell_fd);
		 	 //cout<<"shell: "<<shell_msg<<endl;
		 	 
		 	 /*
			  * initial participant connect
			  */
			  for(int i = 0; i < part_n; i ++) {
			  	  if(!clients[i].connect_remote(config.part[i].ip, config.part[i].port)) {
					  //cout<<"connect to "<<config.part[i].ip<<":"<<config.part[i].port<<" failed\n";
					  clients.erase(clients.begin() + i);
					  config.part.erase(config.part.begin() + i);
					  i--;
					  part_n--;
				  }
				  /*
				  else{				
					  clients[i].send_rpc(msg_hander.set_array(vector<string>(1, "Hello!")));
				  }
				  */
			  }
		 
		 	 /*
		 	  * do execute shell cmd while not done
		 	  */
		 	 //while (1) {
				 int ack_count = 0;
				 for(int i = 0; i < part_n; i ++) {
					 clients[i].send_rpc(msg_hander.set_array(vector<string>(1, "PRE")));
					 //cout<<"pre\n";
					 string msg = clients[i].rcv_rpc();
					 //cout<<"rcv\n";
					 string cmd = msg_hander.get_str(msg);
					 
					 if(cmd == "ACK") {
					 	ack_count ++;
					 }
					 else if(msg == "nil") { /* participant breakdown */
					 	//cout<<"disconnect\n";
					 	clients.erase(clients.begin() + i);
						config.part.erase(config.part.begin() + i);
						//i--;
						part_n--;
						continue;
					 }
					 /*
					  * else msg == "" means recv timeout 
					  */
				}
				//cout<<"ack count: "<<ack_count<<" / "<<part_n<<endl;
				
				
				if (ack_count == part_n){
					//cout<<"commit\n";
					
				 	string shell_ret;
					for(int i = 0; i < part_n; i ++) {
						clients[i].send_rpc(shell_msg);
						//cout<<i<<" have sent\n";
						string msg = clients[i].rcv_rpc();
						string cmd = msg_hander.get_str(msg);
						if(msg == "nil") { /* participant breakdown */
						 	clients.erase(clients.begin() + i);
							config.part.erase(config.part.begin() + i);
							part_n--;
							continue;
						}else if (msg == "" || cmd[0] == '-') {
							/*
							 * "-ERROR"
							 * resend
							 */
							continue;
						}
						if(msg.length() > shell_ret.length()) {
							shell_ret = msg;
						}
						//cout<<i<<" have received "<<msg<<endl;
						
					}
					/*
					 * transmit the newest msg to shell client
					 */
					server.send_rpc(shell_fd, shell_ret);
					//cout<<"send to shell\n";
					
					/*
					 * end execute while
					 */
					close(shell_fd);
					//break;
				}
				else{
					//cout<<"abort\n";
					for(int i = 0; i < part_n; i ++) {
						clients[i].send_rpc(msg_hander.set_array(vector<string>(1, "ABORT")));
						string msg = clients[i].rcv_rpc();
						string cmd = msg_hander.get_str(msg);
					 	if(msg == "nil") { /* participant breakdown */
						 	clients.erase(clients.begin() + i);
							config.part.erase(config.part.begin() + i);
							i--;
							part_n--;
						}else if (msg == "" || cmd[0] == '-') {
							/*
							 * "-ERROR"
							 * resend
							 */
							
						}
					}
				}
				
			//}
			for(int i = 0; i < part_n; i ++) {
				clients[i].close_socket();
			}

		}
	}

	return 0;
}

