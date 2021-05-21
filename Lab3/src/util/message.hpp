#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

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
};

#endif