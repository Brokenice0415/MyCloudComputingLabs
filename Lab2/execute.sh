#!/bin/sh

JAR_PATH=./lab2server.jar
 
#    $1  port
#    $2  threadnum
 
java -jar -Dport=$1 -Dthreadnum=$2 $JAR_PATH 
 
exit $?
