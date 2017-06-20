#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>


using namespace std;

struct Block {
    int blockid = 0;
    char* datenblock;
    int last_used;
    bool content_flag = false;

};
static int lru = 0;
static int block_counter = 0;
int total_blocks1, ram_blocks1;

vector <Block> Datenspeicher;
vector <Block> Arbeitsspeicher;

void init_store(int total_blocks, int ram_blocks) {
    Block block;
    for (int i = 0; i < total_blocks; i++) {
        block.datenblock = new char[1024](); //neuer Datenblock mit Größe 1024 Byte
        block.blockid = block_counter; //Block ID setzten
        block_counter++;
        Datenspeicher.push_back(block); //Block in Container speichern (Reihe von Datenblöcken)
        //malloc(char*,1024);
    }
    for (int i = 0; i < ram_blocks; i++) {
        block.datenblock = (char*) realloc(block.datenblock, 1024); //Datenblöcke werden auf dem Arbeitsspeicher erstellt
        block.blockid = block_counter; //Block ID der Arbeitsspeicher Blöcke setzten
        block_counter++;
        Arbeitsspeicher.push_back(block); //Arbeitsspeicher Datenblöcke im Container speichern
    }
}

int writeCacheBlock(int blockID, char *buffer) {
    if (blockID == -1) { //Wenn Block ID -1 wird ein neuer Datenblock angelegt und im Datenspeicher abgespeichert
        if (block_counter != total_blocks1) {
            Block block;
            block.datenblock = new char[1024]();
            block.blockid = block_counter;
            block_counter++;
            block.content_flag = true;
            Datenspeicher.push_back(block);
            for (int u = 0; u < Arbeitsspeicher.size(); u++) {
                if (!Arbeitsspeicher[u].content_flag) {
                    Arbeitsspeicher[u] = block;
                } else {
                    Arbeitsspeicher.pop_back();
                    Arbeitsspeicher.insert(Arbeitsspeicher.begin(), block);
                }
            }
            return block.blockid;
        } else {
            return -1;
        }
    } else { //sollte der die Block ID bereits existieren muss der Buffer in den 
        for (int i = 0; i < Arbeitsspeicher.size(); i++) { //entsprechenden Block geschrieben wird
            if (Arbeitsspeicher[i].blockid == blockID) {
                //Arbeitsspeicher[i].datenblock = buffer;
                memcpy(Arbeitsspeicher[i].datenblock, buffer, 1024); //memcoy ergänzen
                Arbeitsspeicher[i].content_flag = true;
            } else {
                for (int p = 0; p < Datenspeicher.size(); p++) {
                    if (Datenspeicher[p].blockid == blockID)
                        for (int u = 0; u < Arbeitsspeicher.size(); u++) {
                            if (!Arbeitsspeicher[u].content_flag) {
                                Arbeitsspeicher[u] = Datenspeicher[p];
                            } else {
                                Arbeitsspeicher.pop_back();
                                Arbeitsspeicher.insert(Arbeitsspeicher.begin(), Datenspeicher[p]);
                            }
                        }
                }
            }

        }
    }
}

int readCacheBlock(int blockID, char *buffer) {
    for (int i = 0; i < Arbeitsspeicher.size(); i++) { //
        if (Arbeitsspeicher[i].blockid == blockID) {
            memcpy(buffer, Arbeitsspeicher[i].datenblock, 1024);
            //buffer = Arbeitsspeicher[i].datenblock;
            Block temp = Arbeitsspeicher[i];
            Arbeitsspeicher.erase(Arbeitsspeicher.begin() + i);
            vector<Block>::iterator it = Arbeitsspeicher.begin();
            Arbeitsspeicher.insert(it, temp);
            return 0;
        }
    }
    for (int i = 0; i < Datenspeicher.size(); i++) {
        if (Datenspeicher[i].blockid == blockID) {
            memcpy(buffer, Datenspeicher[i].datenblock, 1024);
            //buffer = Datenspeicher[i].datenblock;
            return 0;
            Block temp = Datenspeicher[i];
            Arbeitsspeicher.pop_back();
            Arbeitsspeicher.insert(Arbeitsspeicher.begin(), temp);
            return 0;
        }
    }
    return -1;
}

int freeCacheBlock(int blockID) { //0 wenn er es gefunden hat
    bool find = false;
    for (int i = 0; i < Datenspeicher.size(); i++) {
        if (blockID == Datenspeicher[i].blockid) {
            delete(Datenspeicher.at(i).datenblock);
            Datenspeicher.erase(Datenspeicher.begin() + i);
            find = true;

        }
    }
    for (int i = 0; i < Arbeitsspeicher.size(); i++) {
        if (blockID == Arbeitsspeicher[i].blockid) {
            free(Arbeitsspeicher.at(i).datenblock);
            Arbeitsspeicher.erase(Arbeitsspeicher.begin() + i);
            find = true;
        }

    }
    if (!find) {
        return -1;
    } else {
        return 0;
    }
}

void cleanup_store() {
    for (int i = 0; i < Datenspeicher.size(); i++) {
        delete Datenspeicher.at(i).datenblock;
    }
    for (int i = 0; i < Arbeitsspeicher.size(); i++) {
        free(Arbeitsspeicher.at(i).datenblock);
    }
}

int main() {
    bool menu = false;
    while (menu == false) {
        cout << "Wie viele Datenblöcke sollen auf dem Hauptspeicher besetzt werden?" << endl;
        cin>>total_blocks1;
        cout << "Wie viele Datenblöcke sollen auf dem Arbeitsspeicher besetzt werden?" << endl;
        cin>>ram_blocks1;
        if (total_blocks1 > ram_blocks1) {
            menu = true;
        } else {
            cout << "Die Anzahl der Datenblöcke des Hauptspeichers muss größer sein als die des Arbeitsspeichers" << endl;
        }
    }

    init_store(total_blocks1, ram_blocks1);
    cout << "Die Datenblöcke wurden auf dem Arbeitsspeicher und dem Hauptspeicher anglegt" << endl;

    for (int i = 0; i < Arbeitsspeicher.size(); i++) {

        writeCacheBlock(Arbeitsspeicher[i].blockid, Arbeitsspeicher[i].datenblock);

    }
    char* temp;
    int number;
    for (int i = 0; i < 100; i++) {
        number = rand() % Datenspeicher.size();
        readCacheBlock(number, temp);
    }

    number = rand() % Datenspeicher.size();
    freeCacheBlock(number);


    return 0;
}

