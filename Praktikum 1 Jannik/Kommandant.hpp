
#ifndef KOMMANDANT_HPP
#define	KOMMANDANT_HPP

#include "headers.h"

#include "Prozessmanager.hpp"

class Kommandant{
public:
    Kommandant()
    : pipe_to_processmanager{ new int[2] },processmanager_wait_status {}
    {
        if ( pipe(pipe_to_processmanager) != 0){
            perror("pipe");
            exit(EXIT_FAILURE);
        };        
    }
    ~Kommandant(){
        delete [] pipe_to_processmanager;
    }
    void run(){
        
        string scheduling_choice{};
        while(true){
            
            cout << "1 = Simple Scheduling\n";
            cout << "2 = Round Robin Scheduling\n";
            string choice {};        
            cin >> choice;
            
            if (choice != "1" && choice != "2"){
                cout <<"Ungeultige Eingabe. Eingabe wiederholen.\n";
                continue;
            }
            else{ 
                scheduling_choice=choice;
                break;
            }
        }
        
        int fork_pid { fork() };
        if(fork_pid < 0){
            throw std::runtime_error("fork_error");
            exit(EXIT_FAILURE);
        }
        if(fork_pid == 0){ //prozessmanager
            Prozessmanager manager {pipe_to_processmanager,scheduling_choice};
            manager.run();
        }               
        if(fork_pid > 0){ //kommandant
            while (true){
                string user_input {};
                cin >> user_input;
                write(pipe_to_processmanager[1], user_input.c_str() , user_input.length()+1);
                kill (fork_pid, SIGUSR1);
                
                if( user_input == "Quit" || user_input == "quit"){
                    wait(&processmanager_wait_status);
                    exit(0);
                }            
            }
        } 
    }
private:
    int * pipe_to_processmanager;
    int processmanager_wait_status;
};

#endif	/* KOMMANDANT_HPP */

