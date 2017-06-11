/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MMU.h
 * Author: Marcel Obel
 *
 * Created on 11. Juni 2017, 18:20
 */

#ifndef MMU_H
#define MMU_H

#include "Process.h"

class MMU {
public:
    MMU(Process P) : curr_process{P} {}
    void virt_to_phys(int);
    
    Process getCurr_process() const {
        return curr_process;
    }

    void setCurr_process(Process curr_process) {
        this->curr_process = curr_process;
    }

private:
    Process curr_process;
};

#endif /* MMU_H */

