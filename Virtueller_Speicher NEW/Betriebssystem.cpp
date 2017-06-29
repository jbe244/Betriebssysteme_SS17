#include "Betriebssystem.h"

template <typename T>
void moveItemToBack(vector<T>& v, size_t itemIndex)
{
    auto it = v.begin() + itemIndex;
    rotate(it, it + 1, v.end());
}


Betriebssystem::Betriebssystem(Arbeitsspeicher * RAM_h)
{
    HDD = new Festplatte;
    RAM = RAM_h;

    Seiten_entfernt = 0;
    Seiten_geladen = 0;
}


Betriebssystem::~Betriebssystem()
{
    delete HDD;
}

int Betriebssystem::load_page(int page) //into the RAM
{
    int frame_number = freien_RahmenFinden();

    for (size_t i = 0; i < Seitengroesse; i++)
    {
        RAM->bytes[frame_number*Seitengroesse + i] = HDD->bytes.at(aktiver_prozess->get_data_start() + i);
    }

    RAM->rahmen.at(frame_number)->Page = aktiver_prozess->get_Seitentabelle()->seiten.at(page);

    cout << "Seite " << page << " vom Prozess " << aktiver_prozess->get_data_start() << " in den Rahmen an Stelle " << frame_number << " laden" << endl;

    Seiten_geladen++;

    return frame_number;
}




int Betriebssystem::freien_RahmenFinden()
{

    for (size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if (RAM->rahmen.at(i)->Page == nullptr)
        {

            cout << "Freier Rahmen an Stelle " << i+1 << " gefunden" << endl;

            return i;
        }
    }

    clear_frame_NRU();
    //clear_frame_2nd_chance();
    return freien_RahmenFinden();   //Nachdem Rahmen freigemacht wurde, Funktion erneut aufrufen

}

void Betriebssystem::clear_frame_2nd_chance()
{
    
    //2nd-Chance
    for (size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if (!RAM->rahmen.at(i)->Page->ref)  //Wenn Reference Bit == 0
        {

            //write_back(RAM->frames.at(i)->Page);  //crasht Programm

            cout << "Seite wird ausgelagert, Rahmen Nr." << RAM->rahmen.at(i)->number << " ist nun frei" << endl;

            RAM->rahmen.at(i)->Page = nullptr;

            Seiten_entfernt++;

            return;

        }
        else 
        {
            RAM->rahmen.at(i)->Page->ref = false;   //Wenn Reference Bit == 1, setze es auf 0 und gehe weiter
            //Ans Ende des Vektors setzen 
            moveItemToBack(RAM->rahmen, i);            
            cout << "Reference Bit von Rahmen Nr. " << RAM->rahmen.at(i)->number << " auf 0 gesetzt" << endl;
        }
    }
}

void Betriebssystem::clear_frame_NRU() 
{
    //R-Bit == 0 & M-Bit == 0
    for(size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if((!RAM->rahmen.at(i)->Page->ref) && (!RAM->rahmen.at(i)->Page->mod))
        {
            //write_back(RAM->frames.at(i)->Page);  //crasht Programm

            cout << "Seite wird ausgelagert, Rahmen Nr. " << i << " ist nun frei" << endl;

            RAM->rahmen.at(i)->Page = nullptr;

            Seiten_entfernt++;

            return;
        }
    }
    
    //R-Bit == 0 & M-Bit == 1
    for(size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if((RAM->rahmen.at(i)->Page->ref) && (!RAM->rahmen.at(i)->Page->mod))
        {
            //write_back(RAM->frames.at(i)->Page);  //crasht Programm

            cout << "Seite wird ausgelagert, Rahmen Nr." << i << " ist nun frei" << endl;

            RAM->rahmen.at(i)->Page = nullptr;

            Seiten_entfernt++;

            return;
        }
    }
    
    //R-Bit == 1 & M-Bit == 0
    for(size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if((!RAM->rahmen.at(i)->Page->ref) && (RAM->rahmen.at(i)->Page->mod))
        {
            //write_back(RAM->frames.at(i)->Page);  //crasht Programm

            cout << "Seite wird ausgelagert, Rahmen Nr." << i << " ist nun frei" << endl;

            RAM->rahmen.at(i)->Page = nullptr;

            Seiten_entfernt++;

            return;
        }
    }
    
    //R-Bit == 0 & M-Bit == 0
    for(size_t i = 0; i < Anzahl_der_Seiten; i++)
    {
        if((RAM->rahmen.at(i)->Page->ref) && (RAM->rahmen.at(i)->Page->mod))
        {
            //write_back(RAM->frames.at(i)->Page);  //crasht Programm

            cout << "Seite wird ausgelagert, Rahmen Nr." << i << " ist nun frei" << endl;

            RAM->rahmen.at(i)->Page = nullptr;

            Seiten_entfernt++;

            return;
        }
    }
}


void Betriebssystem::write_back(Seite * page_rb)
{

	//TODO write ram page back to hdd where it belongs !

	int startpoint_hdd = aktiver_prozess->get_data_start() + (page_rb->page_number * Seitengroesse);
	int startpoint_ram = page_rb->frame_number * Seitengroesse;

	cout << "___OS___: writing back page " << page_rb->page_number << "from frame " << page_rb->frame_number << " to hdd" << endl;

	for (size_t i = 0; i < Seitengroesse; i++)
	{
		HDD->bytes.at(startpoint_hdd + i)->value = RAM->bytes.at(startpoint_ram + i)->value;
	}

}