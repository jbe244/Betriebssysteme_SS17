#include "MMU.h"



MMU::MMU(Arbeitsspeicher* RAM_h)
{

	
	RAM = RAM_h;
	OS = new Betriebssystem(RAM);

}


MMU::~MMU()
{

	delete OS;
	
}


void MMU::set_Seitentabelle(Seitentabelle* pt_h) {

	PT = pt_h;

}



int MMU::translate(int adress)
{
	int page = adress / Seitengroesse;
	int offset = adress % Seitengroesse;

	cout << "Virtuelle Seite Nr. " << adress << " uebersetzen" << endl;

	if (PT->seiten.at(page)->pres == true)   //Seite vorhanden
	{

		cout << "Seite vorhanden, die physikalische Adresse " << PT->seiten.at(page)->frame_number*Seitengroesse + offset << " wurde berechnet" << endl;

		PT->seiten.at(page)->ref = true;    //Seite vorhanden, R-Bit wird == 1 gesetzt
		return PT->seiten.at(page)->frame_number*Seitengroesse + offset;
	}
	else    //Seite nicht vorhanden (in Seitenrahmen)
        {

		cout << "Seite nicht im RAM" << endl;

		PT->seiten.at(page)->frame_number = OS->load_page(page);
		PT->seiten.at(page)->pres = true;
		return PT->seiten.at(page)->frame_number*Seitengroesse + offset;

	}

}