# Lab 3: A Simple Distributed Key-Value Store 



We will try our best to satisfy as many requirements as we can...

- Basic Version OK



>  We assume that every user has known about every command of this system and would never type in any wrong parameter when using. 



## Compile

Run `make` command in Linux shell to compile our program.



## Server Configuration

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

