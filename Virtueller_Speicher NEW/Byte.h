#ifndef Byte_H
#define	Byte_H

#include <random>

class Byte
{
public:
    Byte();
    Byte(Byte*);
    Byte(int);
    ~Byte();

    int value;
};

#endif