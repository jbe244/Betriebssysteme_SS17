/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: debian
 *
 * Created on May 24, 2017, 4:45 AM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "header.h"
#include "pidgenerator.h"

using namespace std;

class Process{
public:
    Process(string file,uint parent_pid,uint time)
    : ppid{parent_pid},int_register{0},pc{0},filename{file},creation_time{time},
            cpu_time_total{0},cpu_time_running{0},finish_time{0},is_finished{false},is_blocked{0},recently_unblocked{0}
    {
        pid = pidgenerator::getPid();
    }
    int getIntRegister(){
        return int_register;
    }
    int getPC(){
        return pc;
    }
    string getFilename(){
        return filename;
    }
    int getPid(){
        return pid;
    }
    void setIntRegister(int newval){
        int_register = newval;
    }
    void setPC(int newval){
        pc=newval;
    }
    uint getCreationTime(){
        return creation_time;
    }
    uint getPpid(){
        return ppid;
    }
    void setBlocked(bool blocked){
        is_blocked= blocked;
        cpu_time_running = 0;        
    }
    void setCpuTimeRunning(uint time){
        cpu_time_running= time;
    }
    void setRecentlyUnblocked(bool var){
        recently_unblocked= var;
    }
    bool wasRecentlyUnblocked(){
        return recently_unblocked;
    }
    uint getTotalCpuTime(){
        return cpu_time_total;
    }
    void incrementCpuTime(){
        ++cpu_time_total;
        ++cpu_time_running;
    }
    uint getRunningCpuTime(){
        return cpu_time_running;
    }
    uint getFinishTime(){
        return finish_time;
    }
    bool isFinished(){
        return is_finished;
    }
    void setFinished(uint tact_counter){
        finish_time = tact_counter;
        cpu_time_running = 0;
        is_finished = true;
    }
    bool isBlocked(){
        return is_blocked;
    }
private:
    uint pid;
    uint ppid;
    int int_register;
    int pc;
    string filename;
    uint finish_time;
    bool is_finished;
    bool is_blocked;
    bool recently_unblocked;
    uint creation_time;
    uint cpu_time_total;
    uint cpu_time_running;
};

#endif /* PROCESS_H */

