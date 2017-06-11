/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: Marcel Obel
 *
 * Created on 11. Juni 2017, 18:20
 */

#ifndef PROCESS_H
#define PROCESS_H

#include"PageTable.h"
#include"RAM.h"
#include<string>

class Process {
public:
    Process()
    {
        std::vector<PageTableEntry> page_table_to_fill;
        for(int i = 0; i < 15; i++) 
        {
            page_table_to_fill.at(i).SetPhys_adress("0101"); 
            page_table_to_fill.at(i).SetModify_bit(0);
        }
        own_page_table.SetPage_table(page_table_to_fill);
    }
    
    PageTable GetOwn_page_table() const {
        return own_page_table;
    }

    void SetOwn_page_table(PageTable own_page_table) {
        this->own_page_table = own_page_table;
    }

    RAM GetRam() const {
        return ram;
    }

    void SetRam(RAM ram) {
        this->ram = ram;
    }

    std::string GetVirt_adress() const {
        return virt_adress;
    }

    void SetVirt_adress(std::string virt_adress) {
        this->virt_adress = virt_adress;
    }

    void read(std::string);
    void write(std::string);
    
private:
    RAM ram;
    PageTable own_page_table;
    std::string virt_adress;
};

#endif /* PROCESS_H */

