#ifndef CPU_H
#define	CPU_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <unistd.h>

#include "MMU.h"
#include "Prozess.h"
#include "settings.h"
#include "Arbeitsspeicher.h"


using namespace std;

class CPU
{

private:

	Arbeitsspeicher * RAM;
	MMU * mmu;
	vector<Prozess*> Prozesse;
	Prozess* aktiver_Prozess;

	bool write(int);
	bool read(int);
	bool switch_process();
	int Adresse_generieren();
        int Adresse_generieren_delta();


	int read_operations = 0;
	int write_operations = 0;
	int process_change_operations = 0;
	int read_acess_violations = 0;
	int write_acess_violations = 0;

public:
	CPU();
	~CPU();

	void operate();

	void print();

};

#endif