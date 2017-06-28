/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MMU.h
 * Author: debian
 *
 * Created on June 28, 2017, 5:13 AM
 */

#pragma once
#include"Process.h"

#include<vector>
#include<array>
#include<bitset>
#include<iostream>
#include<stdexcept>
using namespace std;
class MMU
{
public:
    MMU(){};
    void setCurrentTable(vector<array<size_t, 4>>& current_table);
    size_t convertVirtualAdress(Process* current_process, const bitset<6>& adress);
    ~MMU(){};
private:
    vector<array<size_t, 4>> m_page_table;
};

