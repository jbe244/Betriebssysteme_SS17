#include "Arbeitsspeicher.h"



Arbeitsspeicher::Arbeitsspeicher()
{

	for (size_t i = 0; i < 1024; i++)
	{
            bytes.push_back(new Byte(0));
	}


	for (size_t i = 0; i < Anzahl_der_Seiten; i++)
	{
            rahmen.push_back(new Rahmen);
	}
        
        for(size_t i = 0; i < Anzahl_der_Seiten; i++)
        {
            rahmen.at(i)->number = i+1;
        }

}


Arbeitsspeicher::~Arbeitsspeicher()
{
	for (size_t i = 0; i < rahmen.size(); i++)
	{
		delete rahmen.at(i);
	}

	for (size_t i = 0; i < 1024; i++)
	{
		delete bytes.at(i);
	}
}