#ifndef __MYMAP_HPP__
#define __MYMAP_HPP__

#include <iostream>
#include <map>
#include <utility>

using namespace std;

class MyMap {
    public:
        map<string, string> m;

        void set(string key, string value) {
            if (m.count(key) == 0) {
                m.insert(make_pair(key, value));
            } else {
                m[key] = value;
            }
        }
        string get(string key) {
            string ret = "nnil";
            if(m.count(key) != 0) {
                ret = m[key];
            }
            return ret;
        }
        bool del(string key) {
            if(m.count(key) == 0) {
                return false;
            } else{
                m.erase(key);
                return true;
            }
        }
};

#endif