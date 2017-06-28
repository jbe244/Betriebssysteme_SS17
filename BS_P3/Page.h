/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Page.h
 * Author: debian
 *
 * Created on June 28, 2017, 5:15 AM
 */

#pragma once
#include<bitset>
#include<string>

using namespace std;
class Page
{
public:
	friend class Process;
	Page(){};
	Page(const bitset<6>& virtual_adress, const size_t& content);
	bitset<6> getVirtualAdress() const{ return m_virtual_adress; }
	size_t getContent()const{ return m_content; }
	~Page(){};
private:
	bitset<6> m_virtual_adress;
	size_t m_content;
};

