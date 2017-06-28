/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "OS.h"
#include<iostream>
#include<cmath>
#include<stdexcept>

size_t OS::PAGE_ERROR_COUNTER = 0;
size_t OS::TIMEOUT_TRACKER = 0;
const size_t OS::TIMEOUT_QUANTUM = 4;

void OS::set(const bitset<6>& adress, vector<Page>& hard_disk, Process* current_process, vector<unsigned char>& ram, vector<array<size_t, 4 >> &table) {
    size_t p_class;
    bool freeRAM = 1;
    for (size_t i = 0; i < hard_disk.size(); i++) {
        if (adress.to_ulong() == hard_disk[i].getVirtualAdress().to_ulong() && hard_disk[i].getContent() == current_process->getId()) {
            PAGE_ERROR_COUNTER++; // page error increment    

            /*  search duplicate and delete */
            if (m_page_classes.empty()) {
                p_class = determineClass(current_process, adress);
                m_page_classes.insert(make_pair(p_class, hard_disk[i])); // add page
            } else
                resolveDuplicate(current_process, hard_disk[i]);

            /* no memory left in RAM*/
            freeRAM = findEmptyMemory(current_process, adress, ram);
            if (!freeRAM) {
                m_assignable_page = hard_disk[i];
                throw exception();
            }

            return; // exit loop after page;
        }
    }

    cerr << "ERROR_ page not found.\n";
}

void OS::resolveDuplicate(Process* current_process, Page& page) {
    size_t p_class;

    for (auto it = m_page_classes.begin(); it != m_page_classes.end(); it++) {
        if (it->second.getVirtualAdress().to_ulong() == page.getVirtualAdress().to_ulong()) {
            p_class = determineClass(current_process, it->second.getVirtualAdress());
            Page tempPage = it->second;
            m_page_classes.insert(make_pair(p_class, tempPage)); // add page
            m_page_classes.erase(it); // delete old page
            break;
        }
    }
}

bool OS::findEmptyMemory(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram) {
    size_t idx = 0;
    for (size_t i = 0; i < ram.size(); i++) {
        if (ram[i] == 0) {
            cout << "Wrote page " << adress.to_string() << " to physical memory\n\n";

            addRAM(current_process, adress, ram, i);
            idx = i;

            /* update present bit*/
            size_t idx = getPageNum(current_process->find(adress).getVirtualAdress());

            current_process->getPageTable()[idx][2] = 0x1;
            current_process->getPageTable()[idx][3] = idx; //	insert page frame index

            cout << "\ncurrent page table is updated.\n";
            return true;
        }
    }

    return false;
}

void OS::addRAM(Process* current_process, const bitset<6>& adress, vector<unsigned char>& ram, const size_t& index) {
    m_fifo_queue.push_back(current_process->find(adress)); // FIFO

    for (size_t j = index; j < index + Process::OFFSET_LENGTH; j++) {
        ram[j] = current_process->find(adress).getContent();
        cout << "Index: 0x" << j << endl;
    }
}

void OS::updateRefMod(const size_t& whichBit, Process* current_process, const bitset<6>& adress) {

    enum BIT {
        REF_BIT, MOD_BIT
    };

    size_t index = getPageNum(adress);
    switch (BIT(whichBit)) {
        case REF_BIT:
            current_process->getPageTable()[index][0] = 1;
            break;
        case MOD_BIT:
            current_process->getPageTable()[index][0] = 1;
            current_process->getPageTable()[index][1] = 1;
            break;
        default:
            cerr << "Unspecified bit\n";
            break;
    }
}

Page& OS::randomLowestClass() {
    size_t counter = 0, r_counter= 0;
    int randIdx;  // generate random index for page of the lowest class   
    auto p_lowest_class = m_page_classes.equal_range(m_page_classes.begin()->first); // get iterator for pages of lowest existing class

    for (auto it = p_lowest_class.first; it != p_lowest_class.second; it++) {
        r_counter++;
    }
    
    randIdx = rand() % r_counter;
    
    for (auto it = p_lowest_class.first; it != p_lowest_class.second; it++) {
        if (counter == randIdx) return it->second;
        counter++;
    }
}

size_t OS::determineClass(Process* current_process, const bitset<6>& adress) {

    enum CLASS {
        CLASS1 = 1, CLASS2, CLASS3, CLASS4
    };

    const size_t INDEX = getPageNum(adress);

    /*  Check Reference Bit & Modified Bit    */
    const size_t REF_BIT = current_process->getPageTable()[INDEX][0];
    const size_t MOD_BIT = current_process->getPageTable()[INDEX][1];

    if (REF_BIT == 0 && MOD_BIT == 0) return static_cast<size_t> (CLASS1);
    if (REF_BIT == 0 && MOD_BIT == 1) return static_cast<size_t> (CLASS2);
    if (REF_BIT == 1 && MOD_BIT == 0) return static_cast<size_t> (CLASS3);
    if (REF_BIT == 1 && MOD_BIT == 1) return static_cast<size_t> (CLASS4);
}

void OS::resetRef(Process& current_process) {
    /*  reset refrence bit for complete page table */
    for (array<size_t, 4>& p_table_row : current_process.getPageTable()) {
        p_table_row[0] = 0;
    }
}

void OS::addRAM(vector<unsigned char>& ram) {
    m_fifo_queue.push_back(m_assignable_page); // FIFO

    cout << endl << "Emptied memory space:\n";
    for (size_t j = m_reallocated_pageframe_nr; j < m_reallocated_pageframe_nr + Process::OFFSET_LENGTH; j++) {
        ram[j] = m_assignable_page.getContent();
        cout << "Index: 0x" << j << endl;
    }
}

bool OS::substitutePageWithNRU(Process& process, vector<unsigned char>& ram) {
    Page& rand_lowest_class_page = randomLowestClass();
    size_t pageNr;

    if (process.getId() == rand_lowest_class_page.getContent()) {
        pageNr = getPageNum(rand_lowest_class_page.getVirtualAdress());
        process.getPageTable()[pageNr][process.getPageTable()[0].size() - 2] = 0; // set present bit to 0
        m_reallocated_pageframe_nr = process.getPageTable()[pageNr][process.getPageTable()[0].size() - 1]; // page frame number of reallocated page width lowest class

        size_t assignable_pageNr = getPageNum(m_assignable_page.getVirtualAdress());
        process.getPageTable()[assignable_pageNr][ process.getPageTable()[0].size() - 1] = m_reallocated_pageframe_nr; // replace garbage with correct pageframe number 
        process.getPageTable()[assignable_pageNr][ process.getPageTable()[0].size() - 2] = 1; // set present bit of assignable page to 1

        addRAM(ram);
        return true;
    }
    return false;
}

bool OS::substitutePageWithFIFO(const int& idx, vector<array<size_t, 4> >& table, vector<array<size_t, 4> >& current_table, vector<unsigned char>& ram) {
    m_fifo_queue.front();
    m_oldest_page = m_fifo_queue.front();

    if (idx == m_oldest_page.getContent()) {
        size_t pageNr = getPageNum(m_oldest_page.getVirtualAdress());
        table[pageNr ][table[0].size() - 2] = 0; // present bit = 0
        m_reallocated_pageframe_nr = table[pageNr][table[0].size() - 1]; 

        size_t assignable_pageNr = getPageNum(m_assignable_page.getVirtualAdress());
        current_table[assignable_pageNr][current_table[0].size() - 1] = m_reallocated_pageframe_nr;
        current_table[assignable_pageNr][current_table[0].size() - 2] = 1; // present bit = 1
        addRAM(ram);

        m_fifo_queue.pop_front();

        return true;
    }
    return false;
}

size_t OS::getPageNum(const bitset<6>& adress) const {
    bitset<2> index = adress[adress.size() - 2] + 2 * adress[ adress.size() - 1];
    return index.to_ulong();
}