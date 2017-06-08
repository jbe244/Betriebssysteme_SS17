/* 
 * File:   Prozess.h
 * Author: debian
 *
 * Created on 7. Juni 2017, 08:52
 */

#ifndef PROZESS_H
#define	PROZESS_H
#include "Seitentabelle.h"
#include "Hauptspeicher.h"

using namespace std;

class Prozess {
public:
    Prozess();

    void write(string);
    void read(string);
    
    string getAdresse() const {return Adresse;}
    Seitentabelle getTable() const {return seitentabelle;}
    
    Prozess(const Prozess& orig);
    virtual ~Prozess();
private:
    Seitentabelle seitentabelle;
    string Adresse;
    Hauptspeicher RAM;
};

#endif	/* PROZESS_H */

