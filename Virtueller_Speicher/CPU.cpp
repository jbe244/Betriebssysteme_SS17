/* 
 * File:   CPU.cpp
 * Author: debian
 * 
 * Created on 7. Juni 2017, 09:00
 */

#include "CPU.h"
#include "Prozess.h"
#include "MMU.h"

void CPU::choseOption() {
    srand(time(NULL));
    int n = (int) rand() * (100 - 0) + 0;
    int index = 0;
    
    
    if(n > 40)
    {
        //processes.at(index).read(Adresse);
        Prozess P1 = processes.at(index);
        MMU M1(P1);
    }
    else if(n < 40 && n > 10)
    {
        //processes.at(index).write(Adresse);
        Prozess P2 = processes.at(index);
        MMU M2(P2);
    }
    else if(n < 10)
    {
        int new_index = (int) rand() * (2, 0) + 0;  //Prozesswechsel -> 3 Prozesse - 0,1,2
        index = new_index;
    }

}

