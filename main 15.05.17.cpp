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
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <limits.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <list>
#include <iomanip>

using namespace std;

template <typename Typ>
void stringTo(string str, Typ &Wert)
{
    istringstream strin;
    
    strin.str(str);
    strin >> Wert;
}


int takt = 0;
int next_PID = 0;
bool mode = false;

struct Befehle
{
    vector<string> Process_Commands;
    vector<string> Process_Values;
};

int generate_PID()
{
    next_PID += 1;
    return next_PID - 1;
}

class Process
{
public:
    Process(string filename, pid_t parent_pid, int time) : ppid{parent_pid}, int_register{0}, programm_counter{0}, name{filename}, start_time{time}, total_time{0}, finish_time{0},
            finished{false}, blocked{false} 
            {
                pid = generate_PID();
            }
    bool IsBlocked() const { return blocked; }
    int GetFinish_time() const { return finish_time; }
    bool IsFinished() const { return finished; }
    int GetInt_register() const { return int_register; }
    string GetName() const { return name; }
    pid_t GetPid() const { return pid; }
    pid_t GetPpid() const { return ppid; }
    int GetProgramm_counter() const { return programm_counter; }
    int GetStart_time() const { return start_time; }
    int GetTotal_time() const { return total_time; }
    
    void SetBlocked(bool blocked) { this->blocked = blocked; }
    void SetFinish_time(int finish_time) { this->finish_time = finish_time; }
    void SetFinished(bool finished) { this->finished = finished; }
    void SetInt_register(int int_register) { this->int_register = int_register; }
    void SetName(int name) { this->name = name; }
    void SetPid(int pid) { this->pid = pid; }
    void SetPpid(int ppid) { this->ppid = ppid; }
    void SetProgramm_counter(int programm_counter) { this->programm_counter = programm_counter; }
    void SetStart_time(int start_time) { this->start_time = start_time; }
    void SetTotal_time(int total_time) { this->total_time = total_time; }
    
    void increase_time()
    {
        ++total_time;
    }
    
    string name;
    int int_register;    
    pid_t pid;
    pid_t ppid;    //parent_process ID
    int programm_counter;
    bool blocked, finished;
    int start_time, total_time, finish_time;
};

vector<Process*> processes_vector;
vector<Process*> ready_processes;
vector<Process*> blocked_processes;
vector<Process*> finished_processes;
Process * running_process;

class Process_Manager
{
public:
    Process_Manager() = delete;
    Process_Manager(int pipefd[2]) : pipe_to_parent{pipefd}
    {
        close(pipe_to_parent[1]);
        signal(SIGUSR1, Process_Manager::sig_usr);
    };
    static void sig_usr(int);
    void manage_Command();
    string getOutput(){return output;}
    void setOutput(string str) { this->output = str; }
    int * getPipe(){return pipe_to_parent;}
private:
    int * pipe_to_parent;
    string output;
};

void handler(int x) 
{
    cout << "Bye\n";
    exit(EXIT_SUCCESS);
}

string readFile(string filename, int programm_counter)
{
    string str1, str2;
    ifstream inFile(filename);
    istream_iterator<string> ii(inFile);
    istream_iterator<string> eos;
    int line_counter = 0;

    while(ii != eos)
    {
        str1 = *ii; ++ii;
        str2 = *ii; ++ii;
        
        if(line_counter == programm_counter)
        {
            return str1 + str2;
        }
        line_counter++;
    }
    return "";
}

void createProcess(string filename, pid_t ppid)
{
    ready_processes.push_back(new Process(filename, ppid, takt));
}

void exec_Instruction(string instruction, Process * process)
{
    int tmp_register;
    
    switch (instruction.at(0))
    {
        case 'S': 
        {
            int number; stringTo(instruction,number);                      //stoi( instruction.substr(2,string::npos) );
            tmp_register = number;
            break; 
        }
        case 'A':
        {
            int number; stringTo(instruction, number);                     //stoi( instruction.substr(2,string::npos) );
            tmp_register += number;
            break; 
        }
        case 'D': 
        {
            int number; stringTo(instruction, number);                     //stoi( instruction.substr(2,string::npos) );
            tmp_register -= number;               
            break; 
        }
        case 'B': 
        {
            blocked_processes.push_back(process);  
            process->SetBlocked(true);
            break;
        }
        case 'E': 
        {        
            finished_processes.push_back(process);
            process->SetFinish_time(takt);
            process->SetFinished(true);
            break;
        }                
        case 'R': 
        {
            createProcess ( instruction.substr(2,string::npos),process->GetPid() );            
            break;
        }
    }
    
    process->SetInt_register(tmp_register);
    process->SetProgramm_counter(process->programm_counter + 1);            
}

void take_Step(Process * process)
{
    int programm_counter;
    programm_counter = process->programm_counter;
    string filename = process->name;
    
    string instruction = readFile(filename, programm_counter);
    process->increase_time();
    
    exec_Instruction(instruction, process);
}

void FCFS_Scheduling()
{
    if(ready_processes.size() > 0)
    {
        Process * process{ready_processes.front()};
        ready_processes.erase( find(ready_processes.begin(), ready_processes.end(), process) );      
        running_process = process;
        
        take_Step(process);
        //running_process = nullptr;
        ++takt;
        
        //...
    }
}

void Scheduling()
{
    FCFS_Scheduling();
}

void print_Process_description() 
{
    cout << "pid\t" << "ppid\t" << "value\t" << "tact created\t" << "CPU time\t"<< "tacts running\n";
}

void print_Process(Process * process) 
{
    cout << process->GetPid() << "\t" << process->GetPpid() << "\t" << process->GetInt_register() << "\t" << process->GetStart_time() << "\t\t" << process->GetTotal_time() << "\t\t";
    
    if ( !process->finished )
    {
        cout << ( takt - process->GetStart_time() ) << "\n";
    }
    else 
    {
        cout << (process->GetFinish_time() - process->GetStart_time() ) << "\n";
    }
}


void print_Report() 
{
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "Anzahl Gesamt-Takte: "<< takt << endl;

    cout << endl << "RUNNING PROCESS: " << endl;
    print_Process_description();
    if (running_process != nullptr)
        print_Process(running_process);

    cout << endl << "READY PROCESSES: " << endl;
    print_Process_description();
    
//    const vector<Prozess*> ready = processmanager->getReady();
    for (auto it = ready_processes.begin(); it != ready_processes.end(); ++it)
    {
        print_Process(*it);
    }
    
    cout << endl << "BLOCKED PROCESSES: " << endl;
    print_Process_description();
    
//    const vector<Prozess*> blocked = processmanager->getBlocked();        
    for (auto it = blocked_processes.begin(); it != blocked_processes.end(); ++it)
    {
        print_Process(*it);
    }
    
    cout << endl << "FINISHED PROCESSES: " << endl;
    print_Process_description();
    
//    const vector<Prozess*> finished = processmanager->getFinished();        
    for (auto it = finished_processes.begin(); it != finished_processes.end(); ++it)
    {
        print_Process(*it);
    } 
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    exit(0);         
}

void create_Report()
{
    pid_t pid2;
    
    if((pid2 = fork()) < 0)
    {
        cout << "fork-error-child2" << endl;
        exit (EXIT_FAILURE);
    }
    else if(pid2 == 0)  //Reporter-Prozess
    {
        print_Report();
    }
    else 
    {
        if((waitpid(pid2,0,0)) < 0)   //Auf Reporter-Prozess warten
        {
            perror("waitpid-error2");
            exit (EXIT_FAILURE);
        }
    }
}

void Process_Manager::sig_usr(int signo)
{
    if(signo == SIGUSR1)
        cout << "Signal received!" << endl;
    int buffer = 200;
    char output [buffer];
    
    Process_Manager * manager;
    int * pipe_to_parent = manager->getPipe();
    read(pipe_to_parent[0], output, buffer);
    manager->setOutput(output);
    manager->manage_Command();
}

void Process_Manager::manage_Command()
{
    //    if(signo == SIGUSR1)
//        cout << "Signal caught!" << endl;

//        createProcess("init.txt", 0);
//        FCFS_Scheduling();

    Process_Manager * manager;
    string str = manager->getOutput();
    
    if(str == "Quit" || str == "quit" || str == "Q" || str == "q")
    {

    }
    else if (str == "report" || str == "Report" || str == "P" || str == "p")
    {
        create_Report();
    }
    else if(str == "M" || str == "m" || str == "mode" || str == "Mode")
    {
//                if (debug_mode)
//                    debug_mode = false;
//                else 
//                    debug_mode = true;
    }
    else if (str == "s" || str == "S" || str == "step" || str == "Step")
    {
        cout << "Taken 1 Step" << endl;
        FCFS_Scheduling();
//                if (alg_choice == "1")
//                {
//                    takeStep_simpleScheduling();
//                }
//                else if (alg_choice == "2")
//                {
//                    takeStep_roundRobin();
//                }
    }
    else if (str == "u" || str == "U" || str == "unblock" || str == "Unblock")
    {
//                blocked_processes.front().setRecentlyUnblocked(true);
        blocked_processes.front()->SetBlocked(false);

//                if (ready_processes.front().wasRecentlyUnblocked() )
//                    ready.front()->setRecentlyUnblocked(false); //nur 1 prozess darf recently unblocked status haben (sonst funzt es halt nicht wie gewollt :D)

        ready_processes.insert( ready_processes.begin(), blocked_processes.front() );
        blocked_processes.erase( blocked_processes.begin() );

    }         
    else
    {
        write(STDOUT_FILENO, str.c_str(), str.length());   //auf Konsole ausgeben
        cout << endl;
    }
}

int main(int argc, char** argv) 
{
    int n;
    int pipefd[2];
    int bytes_read{0};
    int status{};
    pid_t pid;
    char buffer[600] = "";
    
    signal(SIGINT, handler);
    
    createProcess("init.txt", 0);
    Scheduling();
    
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
    else if (pid > 0)    //Kommandant (Parent-Prozess)
    {
        close(pipefd[0]);    //Leseseite schließen
        
        while(true)
        {            
            cout << "$ " << flush;
            cin.getline(buffer,600,'\n');
            if(write(pipefd[1], buffer, sizeof(buffer) - 1) != (sizeof(buffer) - 1))
            {
                cerr << "Parent: short write to child" << endl;
                exit(EXIT_FAILURE);
            }
            memset(buffer,'\0',600);    //Array leeren
            kill(pid,SIGUSR1);
        }
    }
    else if(pid == 0)   //Prozessmanager
    {
        Process_Manager manager(pipefd[2]);
    }

    return 0;
}