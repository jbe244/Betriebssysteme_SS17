#ifndef MMU_H
#define	MMU_H


#include "Betriebssystem.h"
#include "Arbeitsspeicher.h"
#include "Seitentabelle.h"

class MMU
{
private:

	Betriebssystem* OS;
	Arbeitsspeicher* RAM;
	Seitentabelle* PT;

	

public:
	MMU(Arbeitsspeicher*);
	~MMU();

	void set_Seitentabelle(Seitentabelle*);
	Betriebssystem* get_os() { return OS; }

	int translate(int);
	
};

#endif