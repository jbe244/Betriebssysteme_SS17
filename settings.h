
#ifndef settings_H
#define	settings_H

    const int Anzahl_der_Prozesse = 10;
    const int Seitengroesse = 64; //in bytes, only use pow(2,x)
    const int replacement_algorythm = 0; // 0 = clock, 1 = TODO
    const int Anzahl_Operationen = 300;
    const int Delta = 100;
    //const int sleep_microseconds = 3000000;  //3-Sekunden
    const int sleep_microseconds = 0;
    
    const int Anzahl_der_Seiten = 16             //1024 / Seitengroesse;    //16 Seiten - 4 Bit Seitennummer, 6 Bit Offset

#endif