/* 
 * File:   MMU.h
 * Author: debian
 *
 * Created on 7. Juni 2017, 09:03
 */

#ifndef MMU_H
#define	MMU_H

using namespace std;

class MMU {
public:
    MMU(Prozess P) :  process{P} {}
    void virt_to_phys();
private:
    Prozess process;
};

#endif	/* MMU_H */

