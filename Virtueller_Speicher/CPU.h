/* 
 * File:   CPU.h
 * Author: debian
 *
 * Created on 7. Juni 2017, 09:00
 */

#ifndef CPU_H
#define	CPU_H

using namespace std;
#include<random>
#include"Prozess.h"

class CPU {
public:
    CPU(vector<Prozess> prozesse) : processes{prozesse} {}    
    void choseOption();
    
    void setAdresse(string Adresse) {
        this->Adresse = Adresse;
    }

    
    CPU(const CPU& orig);
    virtual ~CPU();
private:
    vector<Prozess> processes;
    string Adresse;

};

#endif	/* CPU_H */

