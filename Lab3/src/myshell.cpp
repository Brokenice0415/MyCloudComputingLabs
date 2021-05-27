#include <unistd.h>

#include "util/config.h"
#include "util/message.h"
#include "socket.h"

Message msg_hander;

vector<string> eval(string cmdline) {
	vector<string> param;
    if(cmdline.length() == 0) return param;
    size_t pos = cmdline.find(" ");
    if(pos == cmdline.npos) {cout<<"!!ERROR:\tNEED RARAM"<<endl;return param;}
    string cmd = cmdline.substr(0, pos);
    param.push_back(cmd);
    size_t lpos = pos;
    pos = cmdline.find(" ", pos+1);
    param.push_back(cmdline.substr(lpos+1, ((pos == cmdline.npos)?cmdline.length():pos)-lpos-1));

    if(cmd == "SET") {
        pos = cmdline.find("\"");
        if(pos == cmdline.npos) {cout<<"!!ERROR:\tERROR KV FORMAT"<<endl;return vector<string>(0);}
        lpos = pos;
        pos = cmdline.find("\"", pos+1);
        if(pos == cmdline.npos) {cout<<"!!ERROR:\tERROR KV FORMAT"<<endl;return vector<string>(0);}

        string value = cmdline.substr(lpos+1, pos-lpos-1);

        lpos = -1;
        pos = value.find(" ");
        while(pos != value.npos) {
            param.push_back(value.substr(lpos+1, pos-lpos-1));
            lpos = pos;
            pos = value.find(" ", pos+1);
        }
        param.push_back(value.substr(lpos+1, cmdline.length()-lpos-1));
		
		/*
        cout<<"key:\t"<<param[0]<<endl<<"value:\t";
        for(int i = 1; i < param.size(); i++){
            cout<<param[i]<<" ";
        }
        cout<<endl;
        */

    }else if(cmd == "GET") {
     //   cout<<"key:\t"<<param[0]<<endl;
    }else if(cmd == "DEL") {
    	pos = cmdline.find(" ", lpos+1);
        while(pos != cmdline.npos) {
            param.push_back(cmdline.substr(lpos+1, pos-lpos-1));
            lpos = pos;
            pos = cmdline.find(" ", pos+1);
        }
        param.push_back(cmdline.substr(lpos+1, cmdline.length()-lpos-1));
     //   cout<<"key:\t"<<param[0]<<endl;
    }else{
     //   cout<<"!!ERROR:\tNO COMMAND"<<endl;
     	param.clear();
    }
	
	return param;
}

int main(int argc, char* argv[]) {
    // char buffer[256]; 
    // char *val = getcwd(buffer, sizeof(buffer)); 
    // if (val) { 
    //     std::cout << buffer << std::endl; 
    // } 
    Config config;
    if(argc == 3) {
		config.get_config(string(argv[2]));
	}else return 0;

    string cmdline;
    vector<string> cmd;
    client_socket client;
    while(1) {
        getline(cin, cmdline);
        cmd = eval(cmdline);
        if(!cmd.empty()) {
        	if(!client.connect_remote(config.coor.ip, config.coor.port)){
        		cout<<"-ERROR\r\n";
        		continue;
        	}
        	client.send_rpc(msg_hander.set_array(cmd));
        	string msg = client.rcv_rpc();
        	string cmd = msg_hander.get_str(msg);
		 	if(msg == "nil") { /* coordinator breakdown */
			 	cout<<"-ERROR\r\n";
			 	continue;
			}else if (msg == "") { /* timeout or other error*/
				cout<<"-ERROR\r\n";
				continue;
			}
			cout<<msg;
			client.close_socket();
        }
    }
    return 0;
}
