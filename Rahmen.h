#ifndef Rahmen_H
#define	Rahmen_H

#include "Seite.h"

class Seite;

class Rahmen
{
public:
    Rahmen();
    ~Rahmen();

    Seite* Page = nullptr;
    int number;
};

#endif
