/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MMU.cpp
 * Author: Marcel Obel
 * 
 * Created on 11. Juni 2017, 18:20
 */

#include "MMU.h"

void MMU::virt_to_phys(int option) {
    //Die MMU kann nur auf die Seitentabelle des übergebenen Prozesses zugreifen
    //Virtulle Adresse in Phyische umwandeln und dann diese zurückgeben
    //Prozess.read() / Prozess.write() aufrufen mit der physischen Adresse
    std::string virt_adress = curr_process.GetVirt_adress();
    std::string phys_adress;
    

    std::string virt_seitennummer = virt_adress.substr(0,3);   //4-Bit Seitennummer
    std::string offset = virt_adress.substr(4,15);
    
    std::string phys_seitennummer = "0101";
    
    if((virt_seitennummer == "1101") && (/*modify_bit free*/) && (option == 0))  //Seitenrahmen am index 13 is frei
    {
        //Lesen
        curr_process.read(phys_seitennummer+offset);
    }
    
}