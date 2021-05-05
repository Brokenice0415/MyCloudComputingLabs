#include "stdlib.h"
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <cstring>
using namespace std;

int SendSms(const char *cmd)
{
    int ret = 0;
    ret = system(cmd);
    if (ret != 0)
    {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int opt;

    static struct option long_options[] =
        {
            {"ip", required_argument, NULL, 'i'},
            {"port", required_argument, NULL, 'p'},
            {"threadnum", required_argument, NULL, 't'},
            {NULL, 0, NULL, 0},
        };

    string tip, tport, tnumber_thread;
    while ((opt = getopt_long_only(argc, argv, "a:b:c:d", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':
            tip = optarg;
            break;
        case 'p':
            tport = optarg;
            break;
        case 't': 
            tnumber_thread = optarg;
            break;
        default:
            break;
        }
    }

    string strSendSms = "./execute.sh " + tport + " " + tnumber_thread;

    return SendSms(strSendSms.c_str());
}
