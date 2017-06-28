/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MMU.h"


void MMU::setCurrentTable(vector<array<size_t, 4>>& current_table)
{
	m_page_table = current_table;
}

size_t MMU::convertVirtualAdress(Process* current_process, const bitset<6>& adress)
{
	int present_bit;
	bitset<2> index = adress[adress.size() - 2] + (2 * (int) adress[adress.size() - 1]); 	//	extract Page Number from virtual adress

	/*	validation check	*/
	try{
		current_process->find(adress);
	}
	catch (exception& eo){
		throw invalid_argument("ERROR_page not found\n\n");
	}

		/*	absent bit check	*/
		present_bit = m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 2];

		//	in physical memory
	
		if (present_bit == 1) return  m_page_table[static_cast<int>(index.to_ulong())][m_page_table[0].size() - 1]; //return page frame idx

		else throw exception(); 	// Page Error
}
