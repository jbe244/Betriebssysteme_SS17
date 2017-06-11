/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageTableEntry.h
 * Author: Marcel Obel
 *
 * Created on 11. Juni 2017, 18:20
 */

#ifndef PAGETABLEENTRY_H
#define PAGETABLEENTRY_H

#include<string>

class PageTableEntry {
public:
    PageTableEntry();

    int GetModify_bit() const {
        return modify_bit;
    }

    void SetModify_bit(int modify_bit) {
        this->modify_bit = modify_bit;
    }

    std::string GetPhys_adress() const {
        return phys_adress;
    }

    void SetPhys_adress(std::string phys_adress) {
        this->phys_adress = phys_adress;
    }

private:
    std::string phys_adress;
    int modify_bit;
};

#endif /* PAGETABLEENTRY_H */

