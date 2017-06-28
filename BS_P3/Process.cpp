/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Process.h"
#include <iostream>
#include<string>
#include <cmath>

const size_t Process::OFFSET_LENGTH = 4;			
const size_t Process::PAGENR_LENGTH = 2;
size_t Process::ID_COUNTER = 1;
const size_t Process::SIZE = pow(2,PAGENR_LENGTH);


Process::Process()
:m_id(ID_COUNTER++), m_virtual_memory(SIZE), m_table(vector<array<size_t, 4>>(SIZE))
{
	const unsigned char GARBAGE_PAGE_NR = 0x3;
	const unsigned char OFFSET = 0x4;
	const unsigned NEXT_ADRESS = 0x10;

	unsigned start_adress = 0x0;

	/*	create pages */
	for (size_t i = 0; i < m_virtual_memory.size(); i++)
	{
		bitset<6> block(start_adress + OFFSET);
		m_virtual_memory[i] = Page(block, m_id);
		
		start_adress += NEXT_ADRESS;
	}

	/*	create page table*/
	array <size_t, 4> content = { 0, 0 , 0, GARBAGE_PAGE_NR };
	for (size_t i = 0; i < m_table.size(); i++){
		m_table[i] = content;
	}
}

int Process::getModified(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return m_table[pageNr][1];
}
int Process::getPresent(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return m_table[pageNr][2];
}
bitset<8> Process::getPageFrame(const int& pageNr)const{
	if (pageNr < 0 || pageNr >= m_table.size()) throw out_of_range("Error_ Page Number not found.\n");
	return bitset<8>(m_table[pageNr][0]);
}

/*	page assigned to process */
Page Process::find(const bitset<6>& adress){
	for (Page& page : m_virtual_memory){
		if (page.getVirtualAdress().to_string() == adress.to_string()) return page;
	}
	throw exception();
}

Process::~Process()
{


}