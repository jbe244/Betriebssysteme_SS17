
#include "Reporter.hpp"

   Reporter::Reporter (Prozessmanager * manager)
        : processmanager {manager}
    {}
    void Reporter::printProcessDesc(){
        cout << "pid\t" << "ppid\t" << "value\t" << "tact created\t" << "CPU time\t"<< "tacts running\n";
    }
    
    void Reporter::printProcess(Prozess* toPrint){
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
        cout << "Anzahl Gesamt-Takte: "<< processmanager->getCurrentTactNumber()<<endl;
        
        cout << "\nRUNNING PROCESS: \n";
        printProcessDesc();
        Prozess * running {processmanager->getRunning() };
        if (running != nullptr)
            printProcess( running );
        
        cout << "\nREADY PROCESSES: \n";
        printProcessDesc();
        const vector<Prozess*> ready = processmanager->getReady();
        for (auto it = ready.begin(); it != ready.end(); ++it){
            printProcess (*it);
        }
        cout << "\nBLOCKED PROCESSES: \n";
        printProcessDesc();
        const vector<Prozess*> blocked = processmanager->getBlocked();        
        for (auto it = blocked.begin(); it != blocked.end(); ++it){
            printProcess (*it);
        }
        cout << "\nFINISHED PROCESSES: \n";
        printProcessDesc();
        const vector<Prozess*> finished = processmanager->getFinished();        
        for (auto it = finished.begin(); it != finished.end(); ++it){
            printProcess (*it);
        }
        cout << "##############################################################\n";        
        exit(0);         
    };
