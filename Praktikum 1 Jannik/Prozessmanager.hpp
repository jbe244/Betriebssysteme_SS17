
#ifndef PROZESSMANAGER_HPP
#define	PROZESSMANAGER_HPP

#include "headers.h"
#include "Reporter.hpp"
#include "Prozess.hpp"

class Prozessmanager{
public:
    Prozessmanager() = delete;
    Prozessmanager(int pipe_fd[2],string);
    static Prozessmanager* getTHIS();
    static void getCommand_SignalHandler(int signo);
    void handleCommand(string command);
    void run();
    void scheduleProcesses();
    int* getPipe();
    void createReport();
    void createProcess ( string dateiname,uint ppid );
    void stepProcess ( Prozess* process);
    void executeInstruction(string instruction, Prozess* process);
    string getInstructionFromFile(string filename,int pcounter);
    const vector<Prozess*>& getReady();
    const vector<Prozess*>& getBlocked();
    const vector<Prozess*>& getFinished();
    Prozess * getRunning();
    uint getCurrentTactNumber();
    void takeStep_roundRobin();
    void takeStep_simpleScheduling();
    
private:       
    int * pipe_to_kommandant; // damit im signal handler sichtbar
    int reporter_pid;    
    static Prozessmanager * THIS_OBJECT; //hier wird adresse unseres objectes gespeichert, damit static methode darauf zugreifen kann!!
    vector<Prozess*> blocked;
    vector<Prozess*> ready;
    vector<Prozess*> finished;
    int int_register;
    int program_counter;
    Prozess * running; 
    bool debug_mode;
    uint tact_counter;
    string alg_choice;
}; 

#endif	/* PROZESSMANAGER_HPP */

