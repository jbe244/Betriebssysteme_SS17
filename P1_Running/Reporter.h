/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Reporter.h
 * Author: debian
 *
 * Created on May 24, 2017, 4:50 AM
 */

#ifndef REPORTER_H
#define REPORTER_H

#include "Processmanager.h"
#include "header.h"

class Processmanager;
class Process;

class Reporter {
public:
    Reporter() = delete;
    Reporter (Processmanager * manager);
    void printReport();
    void printProcess(Process*);
    void printProcessDesc();
private:
    Processmanager * processmanager;
};

#endif /* REPORTER_H */

