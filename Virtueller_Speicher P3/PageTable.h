/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageTable.h
 * Author: Marcel Obel
 *
 * Created on 11. Juni 2017, 18:20
 */

#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "PageTableEntry.h"
#include<vector>


class PageTable {
public:
    PageTable();
    
    std::vector<PageTableEntry> GetPage_table() const {
        return page_table;
    }

    void SetPage_table(std::vector<PageTableEntry> page_table) {
        this->page_table = page_table;
    }

private:
    std::vector<PageTableEntry> page_table;
};

#endif /* PAGETABLE_H */

