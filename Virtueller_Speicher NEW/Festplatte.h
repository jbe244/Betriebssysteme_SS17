#ifndef Festplatte_H
#define	Festplatte_H

#include "Byte.h"
#include <vector>

class Festplatte
{
public:
	Festplatte();
	~Festplatte();
        
	std::vector<Byte*> bytes;

};

#endif