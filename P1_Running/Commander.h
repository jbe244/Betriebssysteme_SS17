/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Commander.h
 * Author: debian
 *
 * Created on May 24, 2017, 4:38 AM
 */

#ifndef COMMANDER_H
#define COMMANDER_H

#include "header.h"

#include "Processmanager.h"

class Commander{
public:
    Commander()
    : processmanager_pipe{ new int[2] },pm_wait_status {}
    {
        if ( pipe(processmanager_pipe) != 0){
            perror("processmanager_pipe");
            exit(EXIT_FAILURE);
        };        
    }
    ~Commander(){
        delete [] processmanager_pipe;
    }
    void run(){
        
        string scheduling{};
        while(true){
            
            cout << "Choose your Scheduling:\n";
            cout << "1 = Simple\n";
            cout << "2 = Round Robin\n";
            string choice {};        
            cin >> choice;
            
            if (choice != "1" && choice != "2"){
                cout <<"Invalid Input. Please repeat.\n";
                continue;
            }
            else{ 
                scheduling=choice;
                break;
            }
        }
        
        int fork_pid { fork() };
        if(fork_pid < 0){
            throw std::runtime_error("fork_error");
            exit(EXIT_FAILURE);
        }
        if(fork_pid > 0){ //Commander
            while (true){
                string user_input {};
                cin >> user_input;
                write(processmanager_pipe[1], user_input.c_str() , user_input.length()+1);
                kill (fork_pid, SIGUSR1);
                
                if(user_input == "Q" || user_input == "q" || user_input == "Quit" || user_input == "quit"){
                    wait(&pm_wait_status);
                    exit(0);
                }            
            }
        } 
        if(fork_pid == 0){ //processmanager
            Processmanager processmanager {processmanager_pipe,scheduling};
            processmanager.run();
        }                      
    }
private:
    int * processmanager_pipe;
    int pm_wait_status;
};

#endif /* COMMANDER_H */

