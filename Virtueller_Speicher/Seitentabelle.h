/* 
 * File:   Seitentabelle.h
 * Author: debian
 *
 * Created on 7. Juni 2017, 09:44
 */

#ifndef SEITENTABELLE_H
#define	SEITENTABELLE_H

using namespace std;

class Seitentabelle {
public:
    Seitentabelle();
    Seitentabelle(const Seitentabelle& orig);
    
    struct Entry
    {
        string Adresse;
        int modify_bit;
    };
    
    virtual ~Seitentabelle();
private:
    Entry Table[5];
    //string Adresse;
    //int modify_bit;

};

#endif	/* SEITENTABELLE_H */

    