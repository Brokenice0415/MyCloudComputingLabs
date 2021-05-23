#ifndef __SHELL_H__
#define __SHELL_H__

#include <iostream>
#include <vector>

using namespace std;

void do_shell();
void eval(string);


void do_shell() {
    string cmdline;
    while(1) {
        getline(cin, cmdline);
        eval(cmdline);
    }
}

void eval(string cmdline) {
    if(cmdline.length() == 0) return;
    size_t pos = cmdline.find(" ");
    if(pos == cmdline.npos) {cout<<"!!ERROR:\tNEED RARAM"<<endl;return;}
    string cmd = cmdline.substr(0, pos);
    vector<string> param;
    size_t lpos = pos;
    pos = cmdline.find(" ", pos+1);
    param.push_back(cmdline.substr(lpos+1, ((pos == cmdline.npos)?cmdline.length():pos)-lpos-1));

    if(cmd == "SET") {
        pos = cmdline.find("\"");
        if(pos == cmdline.npos) {cout<<"!!ERROR:\tERROR KV FORMAT"<<endl;return;}
        lpos = pos;
        pos = cmdline.find("\"", pos+1);
        if(pos == cmdline.npos) {cout<<"!!ERROR:\tERROR KV FORMAT"<<endl;return;}

        string value = cmdline.substr(lpos+1, pos-lpos-1);

        lpos = -1;
        pos = value.find(" ");
        while(pos != value.npos) {
            param.push_back(value.substr(lpos+1, pos-lpos-1));
            lpos = pos;
            pos = value.find(" ", pos+1);
        }
        param.push_back(value.substr(lpos+1, cmdline.length()-lpos-1));

        cout<<"key:\t"<<param[0]<<endl<<"value:\t";
        for(int i = 1; i < param.size(); i++){
            cout<<param[i]<<" ";
        }
        cout<<endl;

    }else if(cmd == "GET") {
        cout<<"key:\t"<<param[0]<<endl;
    }else if(cmd == "DEL") {
        cout<<"key:\t"<<param[0]<<endl;
    }else{
        cout<<"!!ERROR:\tNO COMMAND"<<endl;
    }

}


#endif
