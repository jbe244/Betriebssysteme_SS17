#ifndef Seitentabelle_H
#define	Seitentabelle_H

#include "Seite.h"

#include <vector>

class Seitentabelle
{
public:
    Seitentabelle(int);
    ~Seitentabelle();

    std::vector<Seite*> seiten;
};

#endif