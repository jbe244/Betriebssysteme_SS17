/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "Processmanager.h"
#include <iterator>


Processmanager* Processmanager::THIS_OBJECT {nullptr};

Processmanager::Processmanager(int pipe_fd[2],string scheduling)
: kommandant_pipe {pipe_fd}, reporter_pid{}, running{nullptr},debug_mode{true},tact_counter{0},alg_choice{scheduling}
{
    THIS_OBJECT = this;
    close( kommandant_pipe[1] );
    signal(SIGUSR1,getCommand_SignalHandler);
}
Processmanager* Processmanager::getTHIS(){
    return THIS_OBJECT;
}   
void Processmanager::getCommand_SignalHandler(int signo) { //wird von kommandant aufgerufen, wenn neuer eintrag in der pipe
    uint buffsize=200;
    char output [buffsize];
    Processmanager * processmanager = getTHIS();
    int * kommandant_pipe = processmanager->getPipe();
    read (kommandant_pipe[0], output , buffsize); //eingaben werden aktuell gebuffert -> report mehrfach -> wird nach und nach ausgeführt      
    processmanager->handleCommand (output);     
} 

void Processmanager::createReport(){
    int reporter_status {};
    int result = waitpid(reporter_pid, &reporter_status, WNOHANG);
    if ( reporter_pid == 0 || (reporter_pid != 0 && result != 0 && result != -1) ){ 
        int fork_pid{ fork() };
        if (fork_pid > 0){
            reporter_pid = fork_pid;
        }
        else if (fork_pid == 0){ //reporter process
            Reporter reporter (this);
            reporter.printReport();
        }             
    }    
}

void Processmanager::handleCommand(string command) {
    if (command == "quit" || command == "Quit" || command == "q" || command == "Q"){
        int status {};
        wait(&status);
        
        cout << "Average Runtime: ";
        if (finished.size() == 0){
            cout << "Too few data\n";
        }
        else {
            double durchschnittliche_durchlaufzeit {};
            for (auto it = finished.begin();it != finished.end(); ++it ){
                durchschnittliche_durchlaufzeit += ( (*it)->getFinishTime() - (*it)->getCreationTime() );
            }
            cout << ( durchschnittliche_durchlaufzeit /= finished.size() ) << " (n = "<<finished.size() << ")" << endl;
        }
        exit(0);  //beendet den process ( nicht nur diese methode )
    }
    
    else if (command == "report" || command == "Report" || command == "P" || command == "p" || command == "print" || command == "Print"){
        createReport();
    }
    
    else if(command == "M" || command == "m" || command == "mode" || command == "Mode"){
        if (debug_mode)
            debug_mode = false;
        else 
            debug_mode = true;
    }
    
    else if (command == "s" || command == "S" || command == "step" || command == "Step"){
        if (alg_choice == "1"){
            takeStep_simpleScheduling();
        }
        else if (alg_choice == "2"){
            takeStep_roundRobin();
        }
    }
    
    else if (command == "u" || command == "U" || command == "unblock" || command == "Unblock"){
        blocked.front()->setRecentlyUnblocked(true);
        blocked.front()->setBlocked(false);
        
        if (ready.front()->wasRecentlyUnblocked() )
            ready.front()->setRecentlyUnblocked(false); //nur 1 process darf recently unblocked status haben (sonst funzt es halt nicht wie gewollt :D)
        
        ready.insert ( ready.begin(), blocked.front() );
        blocked.erase( blocked.begin() );
        
    }   
}  
void Processmanager::run(){
    createProcess ("init.txt",0);
    scheduleProcesses();
}

void Processmanager::takeStep_roundRobin() {
    
    uint quantum = 2;
    if(ready.size() > 0){
        Process * toStep { ready.front() };
        ready.erase( find(ready.begin(),ready.end(),toStep) );      
        running= toStep;        
        
        stepProcess( toStep );
        running = nullptr;
        
        if (!toStep->isBlocked() && !toStep->isFinished()) {
            
            if (toStep->getRunningCpuTime() >= quantum){
                ready.push_back( toStep );
                toStep->setCpuTimeRunning( 0 );
            }
            else{
                auto it = ready.begin();
                if( (*it)->wasRecentlyUnblocked() ){ //wenn es einen recently unblocked process gibt -> diesen hier nicht an 1. stelle schieben
                    ++it;
                    toStep->setCpuTimeRunning(0);
                }
                ready.insert(it,toStep);  
            }
        }
        
        ++tact_counter;
    }
}

void Processmanager::takeStep_simpleScheduling() {
    if(ready.size() > 0){
        Process * toStep { ready.front() };
        ready.erase( find(ready.begin(),ready.end(),toStep) );      
        running= toStep;        
        
        stepProcess( toStep );
        running = nullptr;
        
        if (!toStep->isBlocked() && !toStep->isFinished()) {
            auto it = ready.begin();
            if( (*it)->wasRecentlyUnblocked() ){ //wenn es einen recently unblocked process gibt -> diesen hier nicht an 1. stelle schieben
                ++it;
            }
            ready.insert(it,toStep);            
        }
        ++tact_counter;
    }
}

void Processmanager::scheduleProcesses(){ //getestet
    while(true){
        if (!debug_mode){
            if (alg_choice == "1"){
                takeStep_simpleScheduling();
            }
            else if (alg_choice == "2"){
                takeStep_roundRobin();
            }
        }
    }
}
int* Processmanager::getPipe(){
    return kommandant_pipe;
}
void Processmanager::createProcess ( string dateiname,uint ppid){
    ready.push_back( new Process(dateiname,ppid,tact_counter) );
}
void Processmanager::stepProcess ( Process* process){

    sleep(1); //step soll 1 sekunde dauern      
    int_register = process->getIntRegister();
    pc = process->getPC();
    string file = process->getFilename();
    
    string order = getInstructionFromFile(file,pc);
    process->incrementCpuTime();
    executeInstruction(order,process);
    process->setRecentlyUnblocked(false);
    
}

void Processmanager::executeInstruction(string instruction, Process* process){
    switch (instruction.at(0)){
        case 'S': {
            int number = stoi( instruction.substr(2,string::npos) );
            int_register = number;
            break; 
        }
        case 'A': {
            int number = stoi( instruction.substr(2,string::npos) );
            int_register += number;
            break; 
        }
        case 'D': {
            int number = stoi( instruction.substr(2,string::npos) );
            int_register -= number;               
            break; 
        }
        case 'B': {
            blocked.push_back(process);  
            process->setBlocked(true);
            break;
        }
        case 'E': {        
            finished.push_back(process);
            process->setFinished( tact_counter );
            break;
        }                
        case 'R': {
            createProcess ( instruction.substr(2,string::npos),process->getPid() );            
            break;
        }
    }
    process->setIntRegister(int_register);
    process->setPC(pc+1);            
}
string Processmanager::getInstructionFromFile(string filename,int pcounter){
    ifstream file (filename);
    string line;
    int line_counter {}; 
    if (file){
        while(getline(file,line)){
            if (line_counter == pcounter){
                file.close();
                return line;
            }
            else {
                ++line_counter;
                continue;
            }
        }
        file.close();
    }
    return "";
}
const vector<Process*>& Processmanager::getReady(){
    return ready;
}
const vector<Process*>& Processmanager::getBlocked(){
    return blocked;
}
const vector<Process*>& Processmanager::getFinished(){
    return finished;
}
Process * Processmanager::getRunning(){
    return running;
}

uint Processmanager::getCurrentTactNumber(){
    return tact_counter;
}