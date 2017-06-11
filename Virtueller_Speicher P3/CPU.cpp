/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPU.cpp
 * Author: Marcel Obel
 * 
 * Created on 11. Juni 2017, 18:20
 */

#include "CPU.h"
#include "MMU.h"

void CPU::choseOption() {
    srand(time(NULL));
    int n = (int) rand() * (100 - 0) + 0;
    int index = 0;
    int option = 0;
    
    
    if(n > 40)
    {
        //processes.at(index).read(Adresse);
        Process P1 = processes.at(index);
        option = 0;
        P1.SetVirt_adress("1101000000000101");
        MMU M1(P1);
        M1.virt_to_phys(option);
    }
    else if(n < 40 && n > 10)
    {
        //processes.at(index).write(Adresse);
        Process P2 = processes.at(index);
        option = 1;
        P2.SetVirt_adress("1101000000000101");
        MMU M2(P2);
        M2.virt_to_phys(option);
    }
    else if(n < 10)
    {
        int new_index = (int) rand() * (2, 0) + 0;  //Prozesswechsel -> 3 Prozesse - 0,1,2
        index = new_index;
    }

}