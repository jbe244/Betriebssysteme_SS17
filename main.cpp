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

#include <fstream>

#include <iterator>

#include <vector>

#include <typeinfo>

using namespace std;

struct Befehle{
    vector<string> aCommands;
    vector<string> aValues;
};

void handler(int x) 

{

    cout << "Bye\n";

    exit(EXIT_SUCCESS);

}

Befehle readFile(string filename){
    string str1, str2;
    
    ifstream inFile(filename);
    istream_iterator<string> ii(inFile);
    istream_iterator<string> eos;

    while(ii != eos)
    {
        str1 = *ii; ++ii;
        str2 = *ii; ++ii;
                    
        Befehle.aCommands.push_back(str1);
        Befehle.aValues.push_back(str2);
    }
    return Befehle;
}

void simulateProcess(string filename)
{
    Befehle befehle;
    bool debug = true;
    bool blocked = false;
    int pc_counter = 0;
    int integer = 0;
    
    befehle = readFile(filename);
    
    for(int i = 0; i < befehle.aCommands.size(); ++i)
    {
        char B;
        int Zahl;
        B = befehle.aCommands.at(i);
        
        if(typeid(befehle.aValues.at(i)) == typeid(int))
        {
            Zahl = stoi(befehle.aValues.at(i));
        }
        
        switch(B)
        {
            case 'S':
                integer = Zahl;
                break;
            case 'A':
                integer = integer+Zahl;
                break;
            case 'D':
                integer = integer-Zahl;
                break;
            case 'B':
                blocked = true;
                break;
            case 'E':
                i = befehle.aCommands.size();
                break;
            case 'R':
                //Befehle befehleOld = befehle;
                //int oldPc = i;
                blocked = true;
                simulateProcess(befehle.aValues.at(i)); 
                blocked = false;
                break;
            default:
                printf("Wrong command");
                break;
        }
    }
    
    
}

int main(int argc, char** argv) 

{

    int n;

    int pipefd[2];

    pid_t pid, pid2;

    char buffer[600] = "";

  
    
    
    while(true)

    {

    

        signal(SIGINT, handler);



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

        else if(pid > 0)    //Kommandant (Parent-Prozess)

        {

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

        }

        else    //Prozessmanager (Child-Prozess) erstellt die Reporter-Prozesse

        {

            close(pipefd[1]);    //Schreibseite schließen

            n = read(pipefd[0], buffer, sizeof(buffer) - 1);    //aus der Pipe lesen



            //umwandlung in string zur Überprüfung ob Befehl oder nicht

            string str(buffer);

            if(str == "Quit") 

            {

                cout << "Shell wird beendet" << endl;   //Alle Prozesse killen?

                kill(getppid(),SIGKILL);  

                kill(getpid(),SIGKILL);

                exit (EXIT_SUCCESS);

            }

            else if(str == "Report")

            {

                if((pid2 = fork()) < 0)

                {

                    cout << "fork-error-child2" << endl;

                    exit (EXIT_FAILURE);

                }

                else if(pid2 == 0)  //Reporter-Prozess

                {

                    

                }

                else    //Prozessmanager

                {

                    if((waitpid(pid,0,0)) < 0)   //Auf Reporter-Prozess warten

                    {

                        perror("waitpid-error2");

                        exit (EXIT_FAILURE);

                    }

                }

            }

            else

            {

                write(STDOUT_FILENO, buffer, n);   //auf Konsole ausgeben

                n = 0;

                cout << endl;

                exit (EXIT_SUCCESS);   

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
