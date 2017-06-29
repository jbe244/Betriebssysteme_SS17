#include "Prozess.h"



Prozess::Prozess(Festplatte* HDD_h)
{
    std_byte = new Byte;
    write_byte = new Byte;

    data_size = rand() % 20 + 1; //Anzahl der Seiten im Festplattenspeicher
    data_startpoint = HDD_h->bytes.size();


    PT = new Seitentabelle(data_size);

    for (int i = 0; i < data_size; i++)
    {
        for (int i = 0; i < Seitengroesse; i++)
        {
            HDD_h->bytes.push_back(new Byte(std_byte));
        }
    }

}


Prozess::~Prozess()
{
    delete PT;
    delete std_byte;
    delete write_byte;
}