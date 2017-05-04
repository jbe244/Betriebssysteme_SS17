
#ifndef PIDGENERATOR_HPP
#define	PIDGENERATOR_HPP

#include "headers.h"

class PidGenerator{
public:
    static uint getId();
private:
    static uint next_id;
};

#endif	/* PIDGENERATOR_HPP */

