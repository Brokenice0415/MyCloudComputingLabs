#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Message {
    public:
        static string set_success(string msg) {
            return "+" + msg + "\r\n";
        }

        static string set_error(string msg) {
            return "-" + msg + "\r\n";
        }

        static string set_int(int msg) {
            return ":" + to_string(msg) + "\r\n";
        }

        static string set_str(string msg) {
            string l = to_string(msg.length());
            return "$" + l + "\r\n" + msg + "\r\n";
        }

        static string set_array(vector<string> msgs) {
            string n = to_string(msgs.size());
            string ret = "*" + n + "\r\n";
            for(vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++) {
                ret += set_str(*it);
            }
            return ret;
        }

        static bool is_success(string msg) {
            return msg[0] == '+';
        }

        static bool is_error(string msg) {
            return msg[0] == '-';
        }

        static int get_int(string msg) {
            if(msg[0] != ':') return -1;
            size_t pos = msg.find('\r');
            return atoi(msg.substr(1, pos-1).c_str());
        }

        static string get_str(string msg) {
            if(msg[0] != '$') return "";
            size_t pos = msg.find('\n');
            return msg.substr(pos+1, msg.find('\r', pos)-pos-1);
        }

        static vector<string> get_array(string msg) {
            vector<string> ret;
            if(msg[0] != '*') return ret;
            size_t lpos = msg.find('\n');
            size_t pos = msg.find('\n', lpos+1);
            pos = msg.find('\n', pos+1);
            while(pos != msg.npos) {
                ret.push_back(get_str(msg.substr(lpos+1, pos-lpos-1)));
                lpos = pos;
                pos = msg.find('\n', lpos+1);
                if(pos == msg.npos)break;
                pos = msg.find('\n', pos+1);
            }
            return ret;
        }
};

#endif
