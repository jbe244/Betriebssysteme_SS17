
#ifndef Seite_H
#define	Seite_H

#include "Rahmen.h"

class Seite
{
public:
    Seite();
    ~Seite();

    bool mod = false;   //Modify Bit
    bool ref = false;   //Reference Bit
    bool prot = false;
    bool pres = false;  //Present Bit
    int frame_number = -1;
    int page_number = -1;
};

#endif