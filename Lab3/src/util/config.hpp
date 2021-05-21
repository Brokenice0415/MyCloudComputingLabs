#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum MODE {
    COORDINATOR, PARTICIPANT
};

struct IP {
    string ip;
    int port;
    IP(){};
    IP(string s){
        int pos = s.find(":");
        ip = s.substr(0, pos);
        port = atoi(s.substr(pos+1, s.length() - pos-1).c_str());
    }
};

class Config {
    public:
        MODE mode;
        IP coor;
        vector<IP> part;
        
        Config (string path) {
            get_config(path);
        }

        void get_config (string path) {
            ifstream fin;
            fin.open(path, ios::in);
            if(!fin){
                cout<<"ERROR: NO READ!"<<endl;
                return;
            }
            string tmp;
            while(getline(fin, tmp)) {
                cout<<tmp<<endl;
                if(tmp[0] == '!') {
                    continue;
                }
                int pos = tmp.find(' ');
                string param = tmp.substr(0, pos);
                string value = tmp.substr(pos+1, tmp.length()-pos-1);
                if(param == "mode") {
                    if(value == "coordinator") {
                        mode = COORDINATOR;
                    } else {
                        mode = PARTICIPANT;
                    }
                }else if(param == "coordinator_info") {
                    coor = IP(value);
                }else if(param == "participant_info") {
                    part.push_back(IP(value));
                }
            }
        }
};


#endif