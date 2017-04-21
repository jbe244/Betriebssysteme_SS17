/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Marcel Obel
 *
 * Created on 21. April 2017, 15:42
 */

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <limits.h>

using namespace std;

void handler(int x) 
{
    cout << "Bye\n";
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) 
{
    int n;
    int pipefd[2];
    pid_t pid, pid2;
    char buffer[600] = "";
    
    signal(SIGINT, handler);
    
    //Befehl lesen
    
    if (pipe(pipefd) < 0)   //Pipe erstellen
    {
        perror ("pipe-error");
        exit (EXIT_FAILURE);
    }
    
    
    if((pid = fork()) < 0)  //forken
    {
        perror ("fork-error");
        exit (EXIT_FAILURE);
    }
    else if(pid > 0)    //Parent-Prozess
    {
        close(pipefd[0]);    //Leseseite schließen
     
        cout << "$ ";
        cin.getline(buffer,600,'\n');
        write(pipefd[1],buffer, sizeof(buffer) - 1);    //in die Pipe schreiben
        
        if((waitpid(pid,0,0)) < 0)   //Auf Child-Prozess warten
        {
            perror("waitpid-error");
            exit (EXIT_FAILURE);
        }
        
        exit (EXIT_SUCCESS);
    }
    else    //Child-Prozess erstellt die Reporter-Prozesse
    {
        close(pipefd[1]);    //Schreibseite schließen
        n = read(pipefd[0], buffer, sizeof(buffer) - 1);    //aus der Pipe lesen

        //umwandlung in string zur Überprüfung ob Befehl oder nicht
        string str(buffer);
        if(str == "EXIT")
        {
            cout << "Shell wird beendet" << endl;
            exit (EXIT_SUCCESS);
        }
        else if(str == "Report")
        {
            //Report-Prozess erstellen (weiteres fork()???)
        }
        else
        {
            write(STDOUT_FILENO, buffer, n);   //auf Konsole ausgeben
            exit (EXIT_SUCCESS);   
        }
    }

    return 0;
}

