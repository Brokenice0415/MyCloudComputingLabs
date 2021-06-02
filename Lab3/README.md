# Lab 3: A Simple Distributed Key-Value Store 



We will try our best to satisfy as many requirements as we can...

- Basic Version OK
- Advanced Version OK



>  We assume that every user has known about every command of this system and would never type in any wrong parameter when using. 



## Environment

Ubuntu 20.04

>  In theory, we satisfy any OS with Linux-like core.



## Compile

Run `make` command in shell to compile our program.



## Server Configuration

### Start Server

Run the command below to setup coordinator and participant server

```shell
> ./kvstore2pcsystem --config "your configuration path"
```

For example, you have a coordinator configuration in "./config/coordinator.conf", and a participant configuration in "./config/participant.conf"

You should run these command to setup your coordinator and participant servers

```shell
#setup participant server
> ./kvstore2pcsystem --config ./config/participant.conf

# setup coordinator server
> ./kvstore2pcsystem --config ./config/coordinator.conf
```

### Stop Server

We set a process background to run servers when running `./kvstore2pcsystem`. If you want to kill any server, type these command below.

```shell
# show network status of process running on port *8* or pid = *8*
# we just focus on the process running on port *8* to find which server process runs background
> netstat -tunlp|grep 8
tcp        0      0 0.0.0.0:8001            0.0.0.0:*               LISTEN      4818/./kvstore2pcsystem 
tcp        0      0 0.0.0.0:8002            0.0.0.0:*               LISTEN      4811/./kvstore2pcsystem 

# then we know the pid of server on port 8001 is 4818, the pid of server on port 8002 is 4811
# use kill [pid] to stop server
> kill 4818
```





## How to Run

We provide `myshell` to function as a shell of this kv store distribute system.

In order to get to know which server the coordinator in on, `myshell` also runs with the coordinator configuration.

```shell
> ./myshell --config "your configuration path"
```

Then you can enjoy yourself in our kv store distribute system demo.

In the example below, we put a {1, Hello} into system, then get it and finally delete it twice.

```shell
# the servers of coordinator and participants are already on

> ./myshell --config ./config/coordinator.conf
SET 1 "Hello"
+OK
GET 1
*1
$5
Hello
DEL 1
:1
DEL 1
-ERROR
```

