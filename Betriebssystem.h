
#ifndef Betriebssystem_H
#define	Betriebssystem_H

#include "Festplatte.h"
#include "Arbeitsspeicher.h"
#include "Prozess.h"
#include "settings.h"
#include <iostream>
#include <algorithm>

using namespace std;

class Betriebssystem
{

private:

	Festplatte* HDD;
	Arbeitsspeicher * RAM;

	Prozess* aktiver_prozess;

	int freien_RahmenFinden();
	void clear_frame_2nd_chance();
        void clear_frame_NRU();
	void write_back(Seite*);

public:
	Betriebssystem(Arbeitsspeicher*);
	~Betriebssystem();

	Festplatte* get_hdd() { return HDD; }
	int load_page(int);
	void set_aktiven_Prozess(Prozess* ap_h) { aktiver_prozess = ap_h; };

	int Seiten_entfernt = 0;
	int Seiten_geladen = 0;

};

#endif
