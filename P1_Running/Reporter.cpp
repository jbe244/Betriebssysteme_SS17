/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "Reporter.h"

   Reporter::Reporter (Processmanager * manager)
        : processmanager {manager}
    {}
    void Reporter::printProcessDesc(){
        cout << "pid\t" << "ppid\t" << "value\t" << "clock created\t" << "CPU time\t"<< "clocks running\n";
    }
    
    void Reporter::printProcess(Process* toPrint){
        cout <<toPrint->getPid()<<"\t"
        <<toPrint->getPpid()<<"\t"
        <<toPrint->getIntRegister()<<"\t"
        <<toPrint->getCreationTime()<<"\t\t"
        <<toPrint->getTotalCpuTime()<<"\t\t";
        if ( !toPrint->isFinished() ){
            cout << ( processmanager->getCurrentTactNumber() - toPrint->getCreationTime() ) << "\n";
        }
        else cout << (toPrint->getFinishTime() - toPrint->getCreationTime() ) << "\n";
    }
    
    void Reporter::printReport(){
        cout << "##############################################################\n";
        cout << "Number Total-Clocks: "<< processmanager->getCurrentTactNumber()<<endl;
        
        cout << "\nRUNNING PROCESS: \n";
        printProcessDesc();
        Process * running {processmanager->getRunning() };
        if (running != nullptr)
            printProcess( running );
        
        cout << "\nREADY PROCESSES: \n";
        printProcessDesc();
        const vector<Process*> ready = processmanager->getReady();
        for (auto it = ready.begin(); it != ready.end(); ++it){
            printProcess (*it);
        }
        cout << "\nBLOCKED PROCESSES: \n";
        printProcessDesc();
        const vector<Process*> blocked = processmanager->getBlocked();        
        for (auto it = blocked.begin(); it != blocked.end(); ++it){
            printProcess (*it);
        }
        cout << "\nFINISHED PROCESSES: \n";
        printProcessDesc();
        const vector<Process*> finished = processmanager->getFinished();        
        for (auto it = finished.begin(); it != finished.end(); ++it){
            printProcess (*it);
        }
        cout << "##############################################################\n";        
        exit(0);         
    };