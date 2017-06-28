/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OS.h
 * Author: debian
 *
 * Created on June 28, 2017, 5:14 AM
 */

#pragma once
#include"Process.h"
#include<vector>
#include<list>
#include <cstdlib>
#include <string>
#include <map>
#include <utility> 

using namespace std;

class OS {
public:
    static size_t PAGE_ERROR_COUNTER; // counter page errors
    static size_t TIMEOUT_TRACKER; // counter next timeout
    static const size_t TIMEOUT_QUANTUM;

    /*  Singleton Pattern */
    static OS &getInstance() {
        static OS os;
        return os;
    }

    void set(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4 >> &table);
    void substitute(vector<Page>& hard_disk);
    bool findEmptyMemory(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram);
    void addRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index);
    void addRAM(vector<unsigned char>& ram);
    void updateRefMod(const size_t& whichBit, Process* current_process, const bitset<6>& adress);
    bool substitutePageWithNRU(Process& process, vector<unsigned char>& ram); // FIFO    
    bool substitutePageWithFIFO(const int& idx, vector<array<size_t, 4 >> &table, vector<array<size_t, 4 >> &current_table, vector<unsigned char>& ram); // FIFO
    void resetRef(Process& current_process);

    ~OS() {
    };
private:
    multimap<size_t, Page> m_page_classes;
    size_t determineClass(Process* current_process, const bitset<6>& adress);
    list<Page> m_fifo_queue;
    Page m_oldest_page;
    size_t m_reallocated_pageframe_nr;
    Page m_assignable_page;

    size_t getPageNum(const bitset<6>& adress) const;
    Page& randomLowestClass();
    void resolveDuplicate(Process* current_process, Page& page);

    OS() {
    };
    OS(const OS &obj);
    OS &operator=(const OS &obj);
};

