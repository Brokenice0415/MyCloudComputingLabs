#include "util/config.h"
#include "util/message.h"
#include "MyMap.h"
#include "socket.h"

/*
 * two-phase waiting time limit
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
		string msg = server.rcv_rpc(fd);
		cmd = msg_hander.get_array(msg);
		/*
		for(vector<string>::iterator it = cmd.begin(); it != cmd.end(); it++) {
			cout<<"= "<<*it;
		}
		cout<<endl;
		*/

		/*
		 * do two-phase commit 
		 *
		if (cmd[0] == "PRE") {
			server.send_rpc(fd, "ACK");
		}
		else if(cmd[0] == "ABORT") {
			server.send_rpc(fd, "ACK");
			continue;
		}
		else continue;
		*/

		/*
		 * handle cmd
		 *
		if(cmd[0] == "SET"){
			string value = *(cmd.begin() + 2);
			for(vector<string>::iterator it = cmd.begin() + 3; it != cmd.end(); it ++) {
				value += " " + *it;
			}
			myMap->set(cmd[1], value);
		}
		*/
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
				config.part.erase(config.part.begin() + 1);
				i--;
				part_n--;
			}else{
				client.send_rpc(msg_hander.set_array(vector<string>(1, "Hello!")));
			}
		}
		
		/*
		 *  new thread to :
		 *	periodically receive heartbeat
		 *  otherwise remove participant
		 *  new participant's heartbeat leads to renew config
		 */

		/*
		 *	while(1):
		 *  do shell
		 *  1. send "PRE" to all participant
		 *  2. new thread to do timing
		 *  2. if receiving more than half of "ACK", send command to all participant
		 *  3. if TIMEOUT semaphore, send "ABORT"
		 */


		/*
		 *  ! shell should on a local process
		 *  then send cmd to client to do two-phase commit
		 */
	}

	return 0;
}
