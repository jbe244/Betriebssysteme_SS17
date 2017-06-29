#ifndef Prozess_H
#define Prozess_H

#include "Seitentabelle.h"
#include "Festplatte.h"
#include "settings.h"
#include "Byte.h"

#include <random>
#include <vector>
;
class Prozess
{

private:

	Seitentabelle* PT;
	int data_startpoint = 0;
	int data_size = 0;

	Byte* std_byte;
	Byte* write_byte;

public:
	Prozess(Festplatte*);
	~Prozess();

	int get_size() { return data_size; }
	Seitentabelle* get_Seitentabelle() { return PT; }
	Byte* get_stdb() { return std_byte; }
	Byte* get_writeb() { return write_byte; }
	int get_data_start() { return data_startpoint; }

	std::vector<int> used_adr;
};

#endif