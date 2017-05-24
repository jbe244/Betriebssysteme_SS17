
#ifndef REPORTER_HPP
#define	REPORTER_HPP

#include "Prozessmanager.hpp"
#include "headers.h"

class Prozessmanager;
class Prozess;

class Reporter {
public:
    Reporter() = delete;
    Reporter (Prozessmanager * manager);
    void printReport();
    void printProcess(Prozess*);
    void printProcessDesc();
private:
    Prozessmanager * processmanager;
};

#endif	/* REPORTER_HPP */

