/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPU.h
 * Author: Marcel Obel
 *
 * Created on 11. Juni 2017, 18:20
 */

#ifndef CPU_H
#define CPU_H

#include<vector>
#include<string>
#include "Process.h"

class CPU {
public:
    CPU(std::vector<Process> Processes) : processes{Processes} {}
    void choseOption();

    std::string getAdresse() const {
        return Adresse;
    }

    void setAdresse(std::string Adresse) {
        this->Adresse = Adresse;
    }

    std::vector<Process> getProcesses() const {
        return processes;
    }

    void setProcesses(std::vector<Process> processes) {
        this->processes = processes;
    }

private:
    std::vector<Process> processes;
    std::string Adresse;
};

#endif /* CPU_H */

