/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pidgenerator.h"

uint pidgenerator::next_pid = 0;

uint pidgenerator::getPid(){
    next_pid+=1;
    return next_pid - 1;
}