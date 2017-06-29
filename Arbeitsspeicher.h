#ifndef Arbeitsspeicher_H
#define	Arbeitsspeicher_H

#include "Byte.h"
#include "Rahmen.h"
#include "settings.h"

#include <vector>

;using namespace std;

class Arbeitsspeicher
{
public:
    Arbeitsspeicher();
    ~Arbeitsspeicher();

    vector<Byte*> bytes;
    vector<Rahmen*> rahmen;
};


#endif