
#ifndef PROZESS_HPP
#define	PROZESS_HPP

#include "headers.h"
#include "PidGenerator.hpp"

using namespace std;

class Prozess{
public:
    Prozess(string file,uint parent_pid,uint time)
    : ppid{parent_pid},int_register{0},program_counter{0},dateiname{file},creation_time{time},
            cpu_time_total{0},cpu_time_running{0},finish_time{0},is_finished{false},is_blocked{0},recently_unblocked{0}
    {
        pid = PidGenerator::getId();
    }
    int getIntRegister(){
        return int_register;
    }
    int getProgramCounter(){
        return program_counter;
    }
    string getFilename(){
        return dateiname;
    }
    int getPid(){
        return pid;
    }
    void setIntRegister(int newval){
        int_register = newval;
    }
    void setProgramCounter(int newval){
        program_counter=newval;
    }
    uint getCreationTime(){
        return creation_time;
    }
    uint getPpid(){
        return ppid;
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
private:
    uint pid;
    uint ppid;
    int int_register;
    int program_counter;
    string dateiname;
    uint creation_time; //takt wo dieser prozess erstellt wurde
    uint cpu_time_total; // wieviele takte hat cpu diesem prozess gewidmet?
    uint cpu_time_running;
    uint finish_time;
    bool is_finished;
    bool is_blocked;
    bool recently_unblocked;
};


#endif	/* PROZESS_HPP */

