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
#include <ctype.h>
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
#include <fstream>
#include <iterator>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <list>

using namespace std;

int status; //empfängt den Wert von exit() und gibt diesen an wait() weiter
int pid_index = 0;
int takt = -1;
bool mode=false;

struct Befehle {
    vector<string> aCommands;
    vector<string> aValues;
};

class Process {
public:
    
 int getPPID(){return ppid;}
    int getPID(){return pid;}
    int getPC(){return pc;}
    bool getSTATUS(){return blocked;}
    void setSTATUS(bool s){blocked=s;};
    int getSTART(){return start;}
    int getTOTAL(){return total;}
    int getVALUE(){return value;}
    void setValue(int val){value=val;};
    Process(int v, int w, int x, Befehle b)
    : pid(v), pc(w), ppid(x), befehle(b) {
        blocked = false;
        start = takt;
    }
   
    int pid;
    int ppid; //process ID
    int pc; // program counter
    bool blocked;
    int start;
    int total;
    Befehle befehle;
    int value;
};

vector<Process> processes_vector;
vector<Process> ready_processes;
vector<Process> blocked_processes;
vector<Process> child_vector;
void handler(int x)
 {

    cout << "Bye\n";

    exit(EXIT_SUCCESS);

}

Befehle readFile(string filename) {
    string str1, str2;
    Befehle befehle;
    ifstream inFile(filename);
    istream_iterator<string> ii(inFile);
    istream_iterator<string> eos;

    while (ii != eos) {
        str1 = *ii;
        ++ii;
        str2 = *ii;
        ++ii;

        befehle.aCommands.push_back(str1);
        befehle.aValues.push_back(str2);
    }
    return befehle;
}

void simulateProcess(Process p) {                                       //simulateProcess nochmal ohne Parameter "Process p" 
    Befehle b = p.befehle;                                        
    bool debug = true;
    int pc_counter = p.pc;
    int integer = 0;
    
    
    
   
    for (int i = 0; i < b.aCommands.size(); ++i) {
       if(mode==true){ 
        
            
        
        if (p.getSTATUS() == true) {                                     
            simulateProcess(ready_processes.front());
            continue;
        }
        takt = takt + 1;
        char B;
        int Zahl;
        B = b.aCommands.at(i).at(0);
        string temp;
        temp = b.aValues[i];
        
      
        
        if(isdigit(temp[0])){
            
         
            Zahl = stoi(temp);  //Wert direkt aus inputstream als int abspeichern
        }
       

        switch (B) {
        
            case 'S':
            {
                cout<<"Initiere "<<Zahl<<endl;
                integer = Zahl;
                cout << "S Process: " << p.pid << " gesetzte Zahl= " << integer;
                p.setValue(integer);
                break;
            }
            case 'A':
            {
                cout<<"Addiere "<<Zahl<<endl;
                integer = integer + Zahl;
                cout << "A Process: " << p.pid << " addierte Zahl= " << integer;
                 p.setValue(integer);
                break;
            }
            case 'D':
            {
                cout<<"Subtrahiere "<<Zahl;
                integer = integer - Zahl;
                cout << "D Process: " << p.pid << " subtrahierte Zahl= " << integer;
                 p.setValue(integer);
                break;
            }
            case 'B':
            {
                cout << "B";
                p.blocked = true;
                // push process to blocked process list
                blocked_processes.push_back(p);
                // remove process from ready process list

                for (int i = 0; i < ready_processes.size(); ++i) {
                    if (ready_processes.at(i).pid == p.pid) {
                        ready_processes.erase(ready_processes.begin() + i);
                    }
                };
                // Call first process in ready list
                simulateProcess(ready_processes.front());
                break;
            }
            case 'E':
            {
                cout << "E";
                i = b.aCommands.size();
                break;
            }
            case 'R':
            {
                cout << "R "<<temp;
                Befehle befehleNew;
                befehleNew = readFile(b.aValues.at(i));
                Process pNew(pid_index + 1, 0, p.pid, befehleNew);
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
        cout << endl;
        }
  
   else{
            cout<<"Bitte Eingabe machen"<<endl;
            i--;
            mode=true;
   }
    }
}

void init() {
    Befehle befehle;
    befehle = readFile("init.txt");
    Process p(pid_index, 0, 0, befehle);            //als globale variable? und werte mit settern und gettern machen?
    processes_vector.push_back(p);
    ready_processes.push_back(p);

    simulateProcess(p);
}

void print(Process p)
{
    
    cout<<"Processes_vector.size = "<<processes_vector.size()<<endl;
    for (int i = 0; i < processes_vector.size(); i++) {
        

    }
 cout<<"Startbereite Prozesse: "<<ready_processes.size()<<endl;
  cout<<"Insgesammte Prozesse: "<<processes_vector.size()<<endl;
   cout<<"Blockierte Prozesse: "<<blocked_processes.size()<<endl;
   
    cout<<"**********************************************"<<endl
           <<"The current System state is as follows:"<<endl
           <<"************************************************"<<endl
           <<"Current Time: "<<takt<<endl
           <<"Running Process: "<<endl
           <<"pid "<<setw(15)<<"ppid" <<setw(15) <<"priority"<<setw(15)<<"value"<<setw(15)<<"start time"<<setw(30)<<"CPU time used so far"<<endl
      

           <<p.getPID() <<setw(15)<<p.getPPID()<<setw(15)<< "0"<<setw(15)<< p.getVALUE()<<setw(15)<< p.getSTART()<<setw(30)<< p.getTOTAL()<<endl
           <<endl
           <<"Blocked Processes:"<<endl
           <<"pid "<<setw(15)<<"ppid" <<setw(15) <<"priority"<<setw(15)<<"value"<<setw(15)<<"start time"<<setw(30)<<"CPU time used so far"<<endl;
           for (int i = 0; i < blocked_processes.size(); i++) {
             cout<< blocked_processes[i].getPID()<<setw(15)<<blocked_processes[i].getPPID()<<setw(15)<<"0"<<setw(15)<<blocked_processes[i].getVALUE()<<setw(15)<<blocked_processes[i].getSTART()<<setw(30)<<blocked_processes[i].getTOTAL()<<endl;
  
             //hier müssen noch die values der blockierten Prozesse rein
  
           }

         
         cout  <<endl
           <<"Processes Ready to Execute:"<<endl
           <<"pid "<<setw(15)<<"ppid" <<setw(15) <<"priority"<<setw(15)<<"value"<<setw(15)<<"start time"<<setw(30)<<"CPU time used so far"<<endl;
        
         for (int i = 0; i < ready_processes.size(); i++) {
             cout<< ready_processes[i].getPID()<<setw(15)<<ready_processes[i].getPPID()<<setw(15)<<"0"<<setw(15)<<ready_processes[i].getVALUE()<<setw(15)<<ready_processes[i].getSTART()<<setw(30)<<ready_processes[i].getTOTAL()-1100<<endl;
//hier müssen noch die Values der starbereiten Prozesse rein
    }
            
         cout<<"************************************************"<<endl;

           
}

int main(int argc, char** argv)
 {
    int n;
    int pipefd[2];
    pid_t pid, pid2;   //pid_t ist ähnlich zu unsigned integer
    char buffer[600] = "";
    int zaehler=0;
    pid_t getpid(void); //returns the process id of the calling process
    pid_t getppid(void); // returns the process id of the parent of the calling process
   
    bool outer_loop=true;
    bool inner_loop=true;
    
    
    
    
    
     init();
     
    while(outer_loop==true){
     
    signal(SIGINT, handler);
    if (pipe(pipefd) < 0) //Pipe erstellen
    {
        perror("pipe-error");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) //forken
    {
       
        
        perror("fork-error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) //Kommandant (Parent-Prozess)
    {
        close(pipefd[0]); //Leseseite schließen


        while (inner_loop==true) {
            cout<<"Zähler: "<<zaehler<<endl;
               
            zaehler++;
            cout << "$ ";
            cin.getline(buffer, 600, '\n');
            write(pipefd[1], buffer, sizeof (buffer) - 1); //in die Pipe schreiben
            string str(buffer);
            memset(buffer, '\0', 600); //Array leeren

            if ((waitpid(pid, 0, 0)) < 0) //Auf Child-Prozess warten
            {
              
                if(ready_processes.size()==0){
                    cout<<"keine Prozesse mehr da"<<endl;
                }
                else {
                    cout<<"ready: "<<ready_processes.size()<<endl
                           << "bl: "<<blocked_processes.size()<<endl;
                }
                
              
                
             cout<< "Child ID: "<<getpid()<<endl<<"Parent ID: "<<getppid();
          
               perror("waitpid-error");
               inner_loop=false;
               simulateProcess(ready_processes.at(0));
               exit(EXIT_FAILURE);
            }

          
            if (str == "Quit"||str=="quit") {
                cout << "Shell wird beendet" << endl; //Alle Prozesse killen?
                kill(getppid(), SIGKILL);
                kill(getpid(), SIGKILL);
                wait(&status);
                exit(EXIT_SUCCESS);
            }
            
        }
    } else //Prozessmanager (Child-Prozess) erstellt die Reporter-Prozesse
    {
        close(pipefd[1]); //Schreibseite schließen

        n = read(pipefd[0], buffer, sizeof (buffer) - 1); //aus der Pipe lesen
        //umwandlung in string zur Überprüfung ob Befehl oder nicht
        string str(buffer);
        
        if (str=="Unblock"|| str=="unblock"|| str=="u" || str=="U"){
               cout<<"Unblock "<< endl;
                       for (int i = 0; i < blocked_processes.size(); i++) {
                          
                           bool stat=false;
                           blocked_processes.at(0).setSTATUS(stat);   //prozess ist nicht mehr blockiert
                           Process a=blocked_processes.at(0);           
                         
                           blocked_processes.erase(blocked_processes.begin()+1);        //blockierter prozess wird gelöscht und zu bereiten gepusht
                           ready_processes.push_back(a);
                           simulateProcess(ready_processes.at(0));
                }
               
                                                 // über funktion realisieren?
            }
        
        if (str == "Print" || str =="print"|| str =="P"|| str =="p") {
          
            if ((pid2 = fork()) < 0) {
                cout << "fork-error-child2" << endl;
                exit(EXIT_FAILURE);
            } else if (pid2 == 0) //Reporter-Prozess
            {
                cout << "Report-child" << endl;
            print(processes_vector.at(pid));
            } else //Prozessmanager
            {
                if ((waitpid(pid, 0, 0)) < 0) //Auf Reporter-Prozess warten
                {
                   
                    perror("waitpid-error 2");
                    exit(EXIT_FAILURE);
                }
            }
        } 
        if (str == "Mode" || str =="mode"|| str =="M"|| str =="m") {
            cout<<"Umschalten zwischen Debug- und Automatic Mode"<<endl;
            if(mode==true){
                mode=false;                     //true = automatisch, false =debug
                
            }
            else {
                mode=true;
            }
        }
        else {
            write(STDOUT_FILENO, buffer, n); //auf Konsole ausgeben
            n = 0;
            cout << endl;
            exit(EXIT_SUCCESS);
        }
        
    }
    }
    return 0;
}



/*
 * FCFS-Algorithmus
 * 
 * int n, j, i;
 * int bt[20], wt[20], tat[20], avwt = 0, avtat = 0;
    cout << "Enter total number of processes(maximum 20):" << endl;
    cin >> n;
 
    cout << "\nEnter Process Burst Time\n";
    for(i = 0; i < n; i++)
    {
        cout << "P[" << i+1 << "]:";
        cin >> bt[i];
    }
 
    wt[0] = 0;    //waiting time for first process is 0
 
    //calculating waiting time
    for(i = 1; i < n;i++)
    {
        wt[i] = 0;
        for(j = 0; j < i; j++)
            wt[i] += bt[j];
    }
 
    cout << "\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time";
 
    //calculating turnaround time
    for(i = 0; i < n; i++)
    {
        tat[i] = bt[i] + wt[i];
        avwt += wt[i];
        avtat += tat[i];
        cout << "\nP[" << i+1 << "]" << "\t\t" << bt[i] << "\t\t" << wt[i] << "\t\t" << tat[i];
    }
 
    avwt/ = i;
    avtat/ = i;
    cout << "\n\nAverage Waiting Time:" << avwt;
    cout << "\nAverage Turnaround Time:" << avtat;
 */