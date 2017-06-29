#include "Seitentabelle.h"



Seitentabelle::Seitentabelle(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        Seite* tmp = new Seite;
        tmp->frame_number = 0;
        tmp->mod = false;
        tmp->pres = false;
        tmp->prot = false;
        tmp->ref = false;
        tmp->page_number = i;
        seiten.push_back(tmp);
    }

}


Seitentabelle::~Seitentabelle()
{
	for (int i = 0; i < seiten.size(); i++)
	{
		delete seiten.at(i);
	}
}