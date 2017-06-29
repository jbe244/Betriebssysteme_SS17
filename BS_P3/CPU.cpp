/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CPU.h"      
#include <random>
#include <stdexcept>
#include <iomanip>

size_t CPU::READING_COUNTER = 0, CPU::WRITING_COUNTER = 0, CPU::PROCESS_SWITCH_COUNTER = 0;

void handler(int index) {
    alarm(2);
    std::cout << "Handler called" << std::endl;
}

CPU::CPU(vector<Process>& processes)
: ram(vector<unsigned char>(1000, 0)) {
    m_processes = processes;
    m_current_process = &m_processes[rand() % m_processes.size()]; // rand process = current process
    mmu.setCurrentTable(m_current_process->getPageTable()); // assign current page table to MMU

    for (Page& page : m_current_process->getVirtualMemory()) {
        hard_disk.push_back(page);
    }
    std::cout << "PROCESS " << m_current_process->getId() << std::endl << std::endl;
    signal(SIGALRM, handler);
}

void CPU::adress_generator_delta(const unsigned &delta_adress) {
    const unsigned MAX_ADRESS = 0x34;
    const unsigned MIN_ADRESS = 0x04;

    int rand_num = static_cast<int> (rand() % 10 + 1);
    if (rand_num % 2 == 0)m_current_adress += delta_adress;
    else m_current_adress -= delta_adress;

    if (m_current_adress < MIN_ADRESS) m_current_adress = MIN_ADRESS;
    if (m_current_adress > MAX_ADRESS) m_current_adress = MAX_ADRESS;
}

void CPU::adress_generator_rand(const bool& preferCurrentAdress) {

    enum ADRESS {
        CURRENT_ADRESS, NEW_ADRESS
    };

    const unsigned MAX_ADRESS = 0x34;
    const unsigned MIN_ADRESS = 0x04;

    /*	Pseudozahlen Generator	*/
    random_device rd;
    mt19937 gen(rd());
    double prob_current = 0, prob_new = 0;

    (preferCurrentAdress ? prob_current = 0.65 : prob_current = 0.45);

    prob_new = 1.0 - prob_current;
    discrete_distribution<> stochastic_distribution({prob_current, prob_new});

    switch (ADRESS(stochastic_distribution(gen))) {
        case CURRENT_ADRESS:
            break;
        case NEW_ADRESS:
        {
            unsigned temp_adress;
            while (temp_adress = (rand() % 34 + 4), static_cast<int> (temp_adress) == static_cast<int> (m_current_adress)) {
            }
            m_current_adress = temp_adress;
            if (m_current_adress < MIN_ADRESS) m_current_adress = MIN_ADRESS;
            if (m_current_adress > MAX_ADRESS) m_current_adress = MAX_ADRESS;
            break;
        }
        default:
            std::cerr << "ERROR_please try again.\n";
            break;
    }
}

/**	arbeitet stochastisch
Befehlssatz= lesen	/	schreiben	/ Prozesswechsel	*/
void CPU::execute(const int &cmd) {
    /*	choose an adress generator	*/
    // NRU
    if (OS::TIMEOUT_TRACKER % OS::TIMEOUT_QUANTUM == 0) { // trigger time out every quantum unit
        std::cout << "TIME OUT - reset all reference bits to 0" << std::endl;
        /*  OS resets reference bit of ALL PAGES to 0 */
        for (Process& proc : m_processes) {
            os.resetRef(proc);
        }
    }
    OS::TIMEOUT_TRACKER++; // increment timeout tracker to calc next timeout for NRU 

    std::cout << "\n______________________________________\n\n";
    switch (CPU::INSTRUCTION(cmd)) {
        case CPU::READ: // LOAD address
            std::cout << "READING OPERATION\t";

            /*  set modified + referenced bit to 1   */
            CPU::READING_COUNTER++;
            break;

        case CPU::WRITE: // STORE address
            std::cout << "WRITING OPERATION\t";

            /*  set referenced bit to 1   */
            CPU::WRITING_COUNTER++;
            break;

        case CPU::SWITCH_PROCESS: //	random process switch
            std::cout << "PROCESS SWITCH\t";

            /*	swap current process with random process in the container */

            if (!m_processes.empty()) {
                int rand_number = 0;

                while (rand_number = rand() % m_processes.size(), (m_processes[rand_number]).getId() == (m_current_process)->getId()) {
                }

                m_current_process = &m_processes[rand_number]; // swap current process with a different random process
                mmu.setCurrentTable(m_current_process->getPageTable()); // assign current page table to MMU

                for (Page& page : m_current_process->getVirtualMemory()) // allocate pages without frame in hard disk
                {
                    hard_disk.push_back(page);
                }
                std::cout << "switched to process " << dec << m_current_process->getId() << '\n';

                PROCESS_SWITCH_COUNTER++;
            } else cerr << ">>\tno process left to switch to\t<<\n\n";

            break;
        default:
            cerr << ">> unknown CPU command <<\n";
            break;
    }

    /*	read/write [adress]	*/
    if (CPU::INSTRUCTION(cmd) == READ || CPU::INSTRUCTION(cmd) == WRITE) {
        /*  choose one adress generator for testing purposes    */
         adress_generator_delta(0x20);
        // adress_generator_rand(true); // current adress preferred

        cout << "LOAD 0x" << hex << static_cast<int> (m_current_adress) << "\n\n";
        try {
            readOrWriteToRAM(INSTRUCTION(cmd), mmu.convertVirtualAdress(m_current_process, m_current_adress));
        } catch (const invalid_argument& eo) {
            cout << "ERROR_ page not found" << '\n';
            return;
        } catch (const exception& eo) {
            fixPageError();
        }
        cout << endl;
    }
}

void CPU::fixPageError() {
    try { // free ram memory space was found
        os.set(m_current_adress, hard_disk, m_current_process, ram, m_current_process->getPageTable());
        mmu.setCurrentTable(m_current_process->getPageTable());
    } catch (const exception& eo) { // error is thrown when ram memory space is exhausted

        cerr << "RAM is full - time to make room!" << '\n';

        /**  NRU Seitenersetzungsalgo 
         ===================================   */ 

        /** Das Betriebssystem teilt alle Seiten in 4 Kategorien/Klassen ein (s.o).
        Ausgewählt (zum Auslagern) wird eine zufällige Seite aus der kleinsten nicht leeren Klasse bzgl. der o.g. Kategorie.
        */

        for (Process& proc : m_processes) {
            for (Page & page : proc.getVirtualMemory()) {
              
                bool wasFound = os.substitutePageWithNRU(proc, ram);
                if (wasFound) return; // Seitenersetzungsalgo erfolgreich
            }
        }

        /**  FIFO Seitenersetzungsalgo   
         ===================================    */

        /** Die Idee ist, die Seiten als FIFO Speicher (verkettete Liste mit beschränkter Größe k) von aktuell im Speicher befindlichen Seiten zu verwalten, wobei die älteste Seite am Anfang steht.
        Verfahren:
        1. Bei Seitenfehler wird die Seite am Kopf der Liste (d.h. die älteste) entfernt 
        2. die eingelagerte Seite wird am Ende eingefügt. */

//                for (Process& proc : m_processes) {
//                    for (Page & page : proc.getVirtualMemory()) {
//                        bool wasFound = os.substitutePageByFIFO(page.getContent(), proc.getPageTable(), m_current_process->getPageTable(), ram);
//                        if (wasFound) return; // Seitenersetzungsalgo erfolgreich
//                    }
//                }
        
    }
}

void CPU::readOrWriteToRAM(const bool& isReading, const size_t& index) {
    /* check if page frame is assigned to external process	*/
    if (static_cast<size_t> (ram[index]) != m_current_process->getId()) {
        cout << "INDEX1:" << dec << static_cast<size_t> (ram[index]) << '\n';
        cout << "INDEX2:" << dec << m_current_process->getId() << '\n';
        cerr << "ERROR_ attempted to read or write from external process. \n";
        return;
    }

    switch (INSTRUCTION(isReading)) {
        case READ:
            os.updateRefMod(static_cast<size_t> (READ), m_current_process, m_current_adress);
            // Read content of page frame block ( adress + offset ) 
            for (size_t i = 0; i < Process::OFFSET_LENGTH; i++) {
                cout << dec << '[' << static_cast<int> (ram[index + i]) << "]\n";
            }
            break;

        case WRITE:
            os.updateRefMod(static_cast<size_t> (WRITE), m_current_process, m_current_adress);
            // Write content of page frame block ( adress + offset ) 
            for (size_t i = 0; i < Process::OFFSET_LENGTH; i++) {
                ram[index + i] = m_current_process->getId();
            }
            break;

        default:
            cerr << "Invalid operation\n";
            break;
    }
}

/*Statistiken fuer Bericht*/
void CPU::print() const {
    cout << dec << "\n______________________________________\n";
    cout << "______________________________________\n\n";
    cout << "REPORT\n\n";
    cout << dec << left << setw(26) << "TOTAL PAGE ERRORS:" << OS::PAGE_ERROR_COUNTER;
    cout << "\n";
    cout << left << setw(26) << "\nREADING OPERATIONS:" << READING_COUNTER << left << setw(26) << "\nWRITING OPERATIONS:" << WRITING_COUNTER << left << setw(26) << "\nPROCESS SWITCHES:" << PROCESS_SWITCH_COUNTER << right << endl << endl;
}