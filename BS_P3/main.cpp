/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: debian
 *
 * Created on June 28, 2017, 5:12 AM
 */

#include"Process.h"
#include"MMU.h"
#include"CPU.h"
#include<vector>
#include<random>
#include<ctime>

using namespace std;

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	const size_t NUM_OF_PROCESS = 255;

	vector<Process> processes;

	/*	Generates Random Numbers */
	random_device rd;
	mt19937 gen(rd());

	/*Pick a random adress*/
	int rand_command;

	/** generate Processes for testing	
	===========================================	*/
	for (size_t i = 0; i < NUM_OF_PROCESS; i++){
		processes.push_back(Process());
	}

	CPU cpu(processes);

	/**	CPU picks a command	*/

	discrete_distribution<> stochastic_distribution({ 3, 2, 1 });
	for (size_t i = 0; i < 1500; i++)
	{       
		rand_command = stochastic_distribution(gen);
		cpu.execute(rand_command);		// call first process
	}
      
	cpu.print();

	return 0;
}