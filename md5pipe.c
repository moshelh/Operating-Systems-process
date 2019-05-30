
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "md5.h"
#include <cstdio>
#include <iostream>


#define MSGSIZE 20
#define READSIZE 33

char rebuf[READSIZE];
char inbuf[MSGSIZE];
pid_t pid;

void ckill(int signal_number){
    exit(0);
}
 void signal_handler(int signal_number){    
    if(strlen(rebuf)==32){
    printf("%s%d%s","encrypted by process ",pid ," : ");
    printf("%s\n",rebuf);
    kill(pid, SIGQUIT);
 }

}
int main(int argc, char const *argv[]){
    int p[2],p1[2] ;
    signal(SIGHUP,signal_handler);
    signal(SIGQUIT,ckill);
    if(pipe(p)<0){
        exit(1);
    }
    if(pipe(p1)<0){
        exit(1);
    }
    printf("plain text: ");
    fgets(inbuf, MSGSIZE, stdin);
    pid=fork();
   
    if (pid<0) {
        exit(2);
    }
    else if(pid>0) {
        close(p[0]);
        close(p1[1]);
        write(p[1],inbuf,MSGSIZE);
        read(p1[0],rebuf,32);
        wait(NULL);
    }
    else { //child
        close(p[1]);
        close(p1[0]);
        char rbuf[MSGSIZE];
        pid_t ppid=getppid();
        read(p[0],rbuf,MSGSIZE);
        write(p1[1],md5(rbuf).c_str(),32);
        kill(ppid,SIGHUP);
    }
    return 0;
}
