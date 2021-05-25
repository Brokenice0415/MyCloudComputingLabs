#ifndef __MYMAP_H__
#define __MYMAP_H__

#include <iostream>
#include <vector>
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
        
        vector<string> get(string key) {
            string v = "nil";
            if(m.count(key) != 0) {
                v = m[key];
            }
            vector<string> ret;
            size_t pos = v.find(' ');
            size_t lpos = -1;
            
            while(pos != v.npos) {
            	ret.push_back(v.substr(lpos+1, pos-lpos-1));
            	lpos = pos;
            	pos = v.find(' ', lpos+1);
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
