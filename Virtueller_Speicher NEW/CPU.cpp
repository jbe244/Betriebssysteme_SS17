#include "CPU.h"




CPU::CPU()
{

    RAM = new Arbeitsspeicher;
    mmu = new MMU(RAM);


    for (int i = 0; i < Anzahl_der_Prozesse; i++)
    {
            Prozesse.push_back(new Prozess(mmu->get_os()->get_hdd()));  //Prozesse initialisieren
    }

        
        //Ersten aktiven Prozess setzen
        
    aktiver_Prozess = Prozesse.at(rand() % Anzahl_der_Prozesse);   //aktiven Prozess für CPU setzen

    mmu->set_Seitentabelle(aktiver_Prozess->get_Seitentabelle());  //Die Seitentabelle des aktiven Prozess für die MMU setzen

    mmu->get_os()->set_aktiven_Prozess(aktiver_Prozess);   //Den aktiven Prozess für das Betriebssystem setzen

        cout << "#################################" << endl;
        cout << "Neuer aktiver Prozess: " << aktiver_Prozess->get_data_start() << endl;
        cout << "#################################" << endl;
}


CPU::~CPU()
{
    delete RAM;
    delete mmu;
}

void CPU::operate() 
{
    for(int i = 0; i < Anzahl_Operationen; i++)
    {   
    int operation = rand() % 100 + 1;

    if (operation < 70)
    {
            if (read(Adresse_generieren_delta()))
            {
                read_operations++;  
            }
            else
            {
                cout << endl << "Prozess versucht ausserhalb seines erlaubten Bereiches zu lesen" << endl << endl;
            }
    }

    if (operation >= 70 && operation < 90)
    {
            if (write(Adresse_generieren_delta())) 
            {
                write_operations++;
            }
            else 
            {
                cout << endl << "Prozess versucht ausserhalb seines erlaubten Bereiches zu schreiben" << endl << endl;
            }
    }

    if (operation >= 90)
    {
            aktiver_Prozess = Prozesse.at(rand() % Anzahl_der_Prozesse);

            mmu->set_Seitentabelle(aktiver_Prozess->get_Seitentabelle()); 

            mmu->get_os()->set_aktiven_Prozess(aktiver_Prozess);

            cout << endl << "#################################" << endl;
            cout << "Neuer aktiver Prozess: " << aktiver_Prozess->get_data_start() << endl;
            cout << "#################################" << endl << endl;
            
            process_change_operations++;
    }
        
        usleep(sleep_microseconds);
    }   //Ende for-loop
}

bool CPU::write(int adress)
{
    if (RAM->bytes[mmu->translate(adress)]->value != aktiver_Prozess->get_stdb()->value)
    {
            write_acess_violations++;

            return false;
    }
    else 
        {
            cout << "------------------------------------------------------";
            cout << endl << "Schreibe - Operation für die Adresse: " << adress << " erfolgreich" << endl;
            cout << "------------------------------------------------------" << endl << endl << endl;
    }
        
        int page = adress / Seitengroesse;
        
        aktiver_Prozess->get_Seitentabelle()->seiten.at(page)->mod = true;   //Modify Bit der aktuell veränderten Seite setzen --> crasht Programm
        RAM->bytes[mmu->translate(adress)] = aktiver_Prozess->get_writeb();
        //cout << "Mod B: " << aktiver_Prozess->get_Seitentabelle()->seiten.at(mmu->translate(adress))->mod << endl;
        //cout << "Mod A: " << aktiver_Prozess->get_Seitentabelle()->seiten.at(mmu->translate(adress))->mod << endl;
    return true;
}

bool CPU::read(int adress)
{
    if (RAM->bytes[mmu->translate(adress)]->value != aktiver_Prozess->get_stdb()->value)
    {
            read_acess_violations++;

            return false;
    }
    else 
        {
            cout << "------------------------------------------------------";
            cout << endl <<  "Lese - Operation für die Adresse: " << adress << " erfolgreich" << endl;  
            cout << "------------------------------------------------------" << endl << endl << endl;
    }

    return true;
}

int CPU::Adresse_generieren() 
{    
    int old_adress = 0;
    
    if(aktiver_Prozess->used_adr.size() > 0)        //Letzte verwendete Adresse wieder verwenden, wenn es welche gibt, nur am Anfang leer
        old_adress = aktiver_Prozess->used_adr.back();  
    
    int old_or_new = rand() % 100 + 1;
    
    if(old_or_new <= 65)
    {
        return old_adress;
    }
    else
    {
        int random_multi = aktiver_Prozess->get_size() * Seitengroesse;    //Adresse innerhalb des Prozesses generieren
    
        int adress = rand() % random_multi; //generate random adress


        for (size_t i = 0; i < aktiver_Prozess->used_adr.size(); i++) //schon verwendet, wenn ja dann neue, da old_or_new > 65
        {
            if (aktiver_Prozess->used_adr.at(i) == adress)
            {
                return Adresse_generieren();    //Wenn vorhanden, noch einmal die Funktion neu aufrufen
            }
        }

        aktiver_Prozess->used_adr.push_back(adress);

        return adress;
        
    }
    
    return 0;
}

int CPU::Adresse_generieren_delta() 
{
    int old_adress = 0;
    
    int adress_raum = aktiver_Prozess->get_size() * Seitengroesse;
    
    //int adress = rand() % value;
    
    int random = rand() % 100 + 1;
    
    if(random < 50)
    {
        if(aktiver_Prozess->used_adr.size() > 0)
        {
            old_adress = aktiver_Prozess->used_adr.back();
            aktiver_Prozess->used_adr.push_back(abs((old_adress + Delta) % adress_raum));
            return (abs((old_adress + Delta) % adress_raum));
        }
        else
            Adresse_generieren();   //Wenn keine Adresse vorhanden, verwende einmal eine zufällige
    }
    else
    {
         if(aktiver_Prozess->used_adr.size() > 0)
        {
            old_adress = aktiver_Prozess->used_adr.back();
            aktiver_Prozess->used_adr.push_back(abs((old_adress - Delta) % adress_raum));
            return (abs((old_adress - Delta) % adress_raum));
        }
        else
            Adresse_generieren();   //Wenn keine Adresse vorhanden, verwende einmal eine zufällige   
    }
}



void CPU::print() {

        cout << "################################################################################" << endl;
        cout << "Schreibe - Operationen: " << write_operations << endl << endl;
    cout << "Lese - Operationen: " << read_operations << endl << endl;
    cout << "Prozesswechsel : " << process_change_operations << endl << endl;
    //cout << "Lesezugriff - versuchte Verletzungen: " << read_acess_violations << endl << endl;
    //cout << "Schreibzugriff - versuchte Verletzungen: " << write_acess_violations << endl << endl;
    cout << "Anzahl der entfernten Seiten: " << mmu->get_os()->Seiten_entfernt << endl << endl;
    cout << "Anzahl der geladenen Seiten: " << mmu->get_os()->Seiten_geladen << endl << endl;
    cout << "################################################################################" << endl;
}