/* 
 * File:   Hauptspeicher.h
 * Author: debian
 *
 * Created on 7. Juni 2017, 09:09
 */

#ifndef HAUPTSPEICHER_H
#define	HAUPTSPEICHER_H

using namespace std;
#include<string>

class Hauptspeicher {
public:
    Hauptspeicher()
    {
        //Array Initialisieren
    }
    
    string[] GetRAM() const {return RAM;}
    void SetRAM(string[] RAM) {this->RAM = RAM;}
    string GetAdresse() const {return adresse;}
    void SetAdresse(string adresse) {this->adresse = adresse;}
    int GetOption() const {return option;}
    void SetOption(int option) {this->option = option;}
    
private:
    string RAM[];
    string adresse;
    int option;
};

#endif	/* HAUPTSPEICHER_H */

