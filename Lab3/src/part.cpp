#include "util/config.h"
#include "util/message.h"
#include "MyMap.h"
#include "socket.h"


/*
 * two-phase waiting time limit /ms
 */
#define TIMEOUT 300


using namespace std;

Config config;
int part_n;
Message msg_hander;


void server_work(int i){
	IP part = config.part[i];
	MyMap *myMap = new MyMap();
	vector<string> cmd;
	server_socket server(part.port); // start listen
	/*
	 *  new thread to :
	 *	periodically send heartbeat
	 *  if no connect with client -> ignore 
	 */
	while(1){
		int fd = server.accept_remote();
		cout<<"receive new connect\n";
		while(1){
		
			
			
			string msg = server.rcv_rpc(fd);
			if(msg == ""){
				close(fd);
				break;
			}
			cmd = msg_hander.get_array(msg);
			
			/*
			for(vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++) {
				cout<<*it<<endl;
			}
			*/
			

			/*
			 * do two-phase commit 
			 */
			if (cmd[0] == "PRE") {
				server.send_rpc(fd, msg_hander.set_str("ACK"));
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

			/*
			 * handle cmd
			 */
			msg = server.rcv_rpc(fd);
			cmd = msg_hander.get_array(msg);
			 
			if(cmd[0] == "SET"){
				string value = *(cmd.begin() + 2);
				for(vector<string>::iterator it = cmd.begin() + 3; it != cmd.end(); it ++) {
					value += " " + *it;
				}
				myMap->set(cmd[1], value);
				server.send_rpc(fd, msg_hander.set_str("ACK"));
			}
			else if(cmd[0] == "GET"){
				vector<string> value = myMap->get(cmd[1]);
				server.send_rpc(fd, msg_hander.set_array(value));
			}
			else if (cmd[0] == "DEL"){
				int del_count = 0;
				for(vector<string>::iterator it = cmd.begin()+1; it != cmd.end(); it ++) {
					if(myMap->del(*it)) {
						del_count ++;
					}
				}
				server.send_rpc(fd, msg_hander.set_int(del_count));
			}
			/**/
			
		}
	}

}

int main(int argc, char* argv[]) {
	if(argc == 3) {
		config.get_config(string(argv[2]));
	}else return 0;
	part_n = config.part.size();
	if(config.mode == PARTICIPANT){
		server_work(0);
	}else{
		vector<client_socket> clients(part_n);
		/*
		* initial participant connect
		*/
		for(int i = 0; i < part_n; i ++) {
			if(!clients[i].connect_remote(config.part[i].ip, config.part[i].port)) {
				cout<<"connect to "<<config.part[i].ip<<":"<<config.part[i].port<<" failed\n";
				clients.erase(clients.begin() + i);
				config.part.erase(config.part.begin() + i);
				i--;
				part_n--;
			}else{
				clients[i].send_rpc(msg_hander.set_array(vector<string>(1, "Hello!")));
			}
		}
		
		/*
		 *  new thread to :
		 *	periodically receive heartbeat with TIMEOUT_1
		 *  otherwise remove participant
		 *  new participant's heartbeat leads to renew config
		 */

		/*
		 *	while(1):
		 *  do shell
		 *  1. send "PRE" to all participant
		 *  2. new thread to do timing
		 *  2. if receiving more than half of "ACK", send command to all participant
		 *  3. if TIMEOUT_2 semaphore, send "ABORT"
		 */
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
			 //cout<<"commit\n";
		}
		//cout<<"ack count: "<<ack_count<<endl;
		
		if (2*ack_count > part_n){
			/*
			 *  if "SET" cmd
			 */
			vector<string> tmp;
		 	tmp.push_back("SET");
		 	tmp.push_back("asd");
		 	tmp.push_back("1");
		 	
			for(int i = 0; i < part_n; i ++) {
				clients[i].send_rpc(msg_hander.set_array(tmp));
				string msg = clients[i].rcv_rpc();
				/*
				 * transmit msg to shell client with out any action
				 */
			}
		}
		else{
			for(int i = 0; i < part_n; i ++) {
				clients[i].send_rpc(msg_hander.set_array(vector<string>(1, "ABORT")));
			 	clients[i].rcv_rpc();
			}
		}
		
		/*
		 *  ! shell should on a local process
		 *  then send cmd to client to do two-phase commit
		 */
		 
	}

	return 0;
}

