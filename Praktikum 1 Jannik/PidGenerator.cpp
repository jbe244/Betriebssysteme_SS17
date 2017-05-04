
#include "PidGenerator.hpp"

uint PidGenerator::next_id = 0;

uint PidGenerator::getId(){
    next_id+=1;
    return next_id - 1;
}
