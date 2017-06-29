#include "Byte.h"



Byte::Byte()
{
    value = rand() % 256;
}

Byte::Byte(Byte* org) 
{

    value = org->value;

}

Byte::Byte(int val)
{
	value = val;
}

Byte::~Byte()
{
}

