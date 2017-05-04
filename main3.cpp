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
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <vector>
#include <typeinfo>
#include <list>

using namespace std;

int fd[2];
int pid_index = 0;
int takt = -1;
bool bInit = true;

struct Befehle{
    vector<string> aCommands;
    vector<string> aValues;
};

class Process{
public:
    Process(int v, int w, int x, Befehle b) 
        :pid(v), pc(w), ppid(x), befehle(b){
         blocked = false;
         start = takt;
    }

    int pid;
    int ppid;    //process ID
    int pc;     // program counter
    bool blocked;
    int start;
    int total;
    Befehle befehle;
};

vector<Process> processes_vector;
vector<Process> ready_processes;
vector<Process> blocked_processes;
vector<string> vCommands = {"M","S","U"};


Befehle readFile(string filename){
    string str1, str2;
    Befehle befehle;
    ifstream inFile(filename);
    istream_iterator<string> ii(inFile);
    istream_iterator<string> eos;

    while(ii != eos)
    {
        str1 = *ii; ++ii;
        str2 = *ii; ++ii;
                    
        befehle.aCommands.push_back(str1);
        befehle.aValues.push_back(str2);
    }
    return befehle;
}

void simulateProcess(Process p)
{
    Befehle b = p.befehle;
    bool debug = true;
    int pc_counter = p.pc;
    int integer=0;
    
    for(int i = 0; i < b.aCommands.size(); ++i)
    {
        if(p.blocked == true){
            return;
        }
        takt = takt +1;
        char B;
        int Zahl;
        B = b.aCommands.at(i).at(0);
        string temp;
        
        if(typeid(b.aValues.at(i)) == typeid(int))
        {
            temp = b.aValues[i];
            Zahl = stoi(temp);
        }
        
        switch(B)
        {
            case 'S':
            {
                integer = Zahl;
                cout << "Process: " << p.pid << "Zahl=" << integer;
                break;
            }
            case 'A':
            {
                integer = integer+Zahl;
                cout << "Process: " << p.pid << "Zahl=" << integer;
                break;
            }
            case 'D':
            {
                integer = integer-Zahl;
                cout << "Process: " << p.pid << "Zahl=" << integer;
                break;
            }
            case 'B':
            {
                p.blocked = true;
                // push process to blocked process list
                blocked_processes.push_back(p);
                // remove process from ready process list
                
                for(int i = 0; i < ready_processes.size(); ++i)
                {
                    if(ready_processes.at(i).pid == p.pid)
                    {
                        ready_processes.erase(ready_processes.begin() + i);
                    }
                };
                break;
            }
            case 'E':
            {
                i = b.aCommands.size();
                break;
            }
            case 'R':
            {
                Befehle befehleNew;
                befehleNew = readFile(b.aValues.at(i));
                Process pNew(pid_index+1, 0,p.pid, befehleNew);
                processes_vector.push_back(pNew);
                ready_processes.push_back(pNew);
                break;
            }
            default:
            {
                cout << "Wrong Command" << endl;
                break;
            }
        }
    }
}

void init()
{
    Befehle befehle;
    befehle = readFile("init.txt");
    Process p(pid_index, 0,0, befehle);
    processes_vector.push_back(p);
    ready_processes.push_back(p);

    bInit = false;
}

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
    bool bReadyProcesses = true;

    if(bInit){
        init();
    }

    while(bReadyProcesses){
        simulateProcess(ready_processes.front());
        if(ready_processes.size() == 0){
            bReadyProcesses = false;
            cout << "There are no ready processes in the queue" << endl;
        }
    }
}

bool in_array(const string &value, const vector<string> &array)
{
    return find(array.begin(), array.end(), value) != array.end();
}

void kommandant(){
    pid_t pid;
    int n;
    char buf[1024] = "";
    fstream log;
    pid_t pidReport;

    while(true) {
        signal(SIGINT, beenden);

        if (pipe(fd) < 0) {
            perror("pipe-error");
            exit(EXIT_FAILURE);
        }

        if ( (pid = fork()) < 0) {
            perror("fork-error");
            exit(EXIT_FAILURE);
        } else if (pid > 0){
            close(pipefd[0]);    //Leseseite schließen

            cout << "$ ";
            cin.getline(buffer,600,'\n');
            write(pipefd[1],buffer, sizeof(buffer) - 1);    //in die Pipe schreiben
            memset(buffer,'\0',600);    //Array leeren

            if((waitpid(pid,0,0)) < 0)   //Auf Child-Prozess warten
            {
                perror("waitpid-error");
                exit (EXIT_FAILURE);
            }
        } else {

            close(fd[1]);

            n= read(fd[0],buf,sizeof(buf) - 1);
            string cBuf(buf);

            if(cBuf == "Stop") {
                cout << "Alle Prozesse werden beendet" << endl;   //Alle Prozesse killen?
                kill(getppid(),SIGKILL);  
                kill(getpid(),SIGKILL);
                exit (EXIT_SUCCESS);
            } else if (cBuf == "Report"){
                if((pidReport = fork()) == 0){
                    report();
                }
            } else if (in_array(cBuf,vCommands){
                if((waitpid(pid,0,0)) < 0)   //Auf Reporter-Prozess warten
                {
                    perror("waitpid-error2");
                    exit (EXIT_FAILURE);
                } else {
                    processmanager();
                }   
            } else {
                write(STDOUT_FILENO, buf, n);
                n=0;
                cout << endl;
                exit(EXIT_SUCCESS);
            }
        }
    }
}

int main() {

    signal(SIGINT, beenden);

    printf("Beenden mit CTR_C\n");

    kommandant();
}
