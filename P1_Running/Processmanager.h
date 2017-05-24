/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Processmanager.h
 * Author: debian
 *
 * Created on May 24, 2017, 4:48 AM
 */

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "header.h"
#include "Reporter.h"
#include "Process.h"

class Processmanager{
public:
    Processmanager() = delete;
    Processmanager(int pipe_fd[2],string);
    static Processmanager* getTHIS();
    static void getCommand_SignalHandler(int signo);
    void handleCommand(string command);
    void run();
    void scheduleProcesses();
    int* getPipe();
    void createReport();
    void createProcess ( string dateiname,uint ppid );
    void stepProcess ( Process* process);
    void executeInstruction(string instruction, Process* process);
    string getInstructionFromFile(string filename,int pcounter);
    const vector<Process*>& getReady();
    const vector<Process*>& getBlocked();
    const vector<Process*>& getFinished();
    Process * getRunning();
    uint getCurrentTactNumber();
    void takeStep_roundRobin();
    void takeStep_simpleScheduling();
    
private:       
    int * kommandant_pipe; // damit im signal handler sichtbar
    int reporter_pid;    
    static Processmanager * THIS_OBJECT; //hier wird adresse unseres objectes gespeichert, damit static methode darauf zugreifen kann!!
    vector<Process*> blocked;
    vector<Process*> ready;
    vector<Process*> finished;
    int int_register;
    int pc;
    Process * running; 
    bool debug_mode;
    uint tact_counter;
    string alg_choice;
}; 

#endif /* PROCESSMANAGER_H */

