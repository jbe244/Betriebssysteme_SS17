#include <unistd.h>
#include "CPU.h"


int main()
try
{
    srand(time(NULL));  //Für random-Adressen & Auswahl der Operationen

    CPU cpu;

    cpu.operate();

    cpu.print();
	
    return 0;
}
catch(exception& e)
{
    cerr << e.what() << endl << endl;
}
catch(...)
{
    cerr << "Unebkannter Fehler" << endl << endl;
}
