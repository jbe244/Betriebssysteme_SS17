/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jonas Bernard
 *
 * Created on April 20, 2017, 10:55 AM
 */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int fd[2];

void beenden(int param) {
    printf("\nbye!\n");
    exit(0);
}

void report() {
    cout << "***************************************" << endl;
    cout << "The current system state is as follows:" << endl;
    cout << "***************************************" << endl;
    time_t  timev;
    cout << "CURRENT TIME: " << time(&timev) << endl;
    
    kill(getpid(), SIGKILL);
    exit(EXIT_SUCCESS); 
}

void processmanager(){
    int n;
    char buf[1024] = "";
    fstream log;
    pid_t pid;

    while(true) {
        signal(SIGINT, beenden);
        cout << "$ " << endl;
        cin.getline(buf,1024,'\n');

        write(fd[1],buf,sizeof(buf) - 1);
        memset(buf,'\0',1024);

        log.open("logfile", fstream::out | fstream::app);
        log << buf << endl;
          log.close();

        n= read(fd[0],buf,sizeof(buf) - 1);
        string cBuf(buf);

        if(cBuf == "Stop") {
            cout << "Alle Prozesse werden beendet" << endl;   //Alle Prozesse killen?
            kill(getppid(),SIGKILL);  
            kill(getpid(),SIGKILL);
            exit (EXIT_SUCCESS);
        } else if (cBuf == "Report"){
            if((pid = fork()) == 0){
                report();
            }
        } else {
            write(STDOUT_FILENO, buf, n);
            n=0;
            cout << endl;
            exit(EXIT_SUCCESS);
        }
    }
}

void kommandant(){
    pid_t pid;

    if (pipe(fd) < 0) {
        perror("pipe-error");
        exit(EXIT_FAILURE);
    }

    if ( (pid = fork()) < 0) {
        perror("fork-error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        processmanager();
    } else {
        waitpid(pid, NULL, 0);
        kill(getpid(), SIGKILL);
    }
}

int main() {

    signal(SIGINT, beenden);

    printf("Beenden mit CTR_C\n");

    kommandant();
}
