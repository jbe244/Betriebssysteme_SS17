/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: debian
 *
 * Created on June 28, 2017, 5:15 AM
 */

#pragma once
#include"Page.h"
#include<bitset>
#include<array>
#include<vector>

using namespace std;
class Process
{
public:
	static size_t ID_COUNTER;
	static const size_t OFFSET_LENGTH;
	static const size_t PAGENR_LENGTH;
	static const size_t SIZE;
	Process();
	vector<Page> getVirtualMemory() const{ return m_virtual_memory; }
	size_t getId() const { return m_id; }
	int getModified(const int& pageNr)const;
	int getPresent(const int& pageNr)const;
	bitset<8> getPageFrame(const int& pageNr)const;
	vector<array<size_t, 4>>& getPageTable(){ return m_table; }
	Page find(const bitset<6>& adress);
	~Process();
private:
	/**	information:
	Reference Bit	-	Modified Bit	-	Present / Absent	-	Page Frame	*/
	vector<array<size_t, 4>> m_table;

	/*	virtual memory	*/
	vector<Page> m_virtual_memory;

	size_t m_id;
};

