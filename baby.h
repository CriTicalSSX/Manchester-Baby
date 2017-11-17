/*
 * Sam Glendenning
 * Dylan Keningale
 * Matthew Daldry
 * Will Lockett
 *
 * COMPUTER SYSTEMS 2A: ASSIGNMENT 3
 * THE MANCHESTER SMALL-SCALE EXPERIMENTAL MACHINE (BABY)
 * Header file for Baby object
 */

#ifndef BABY_H_
#define BABY_H_

#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <fstream>
#include <cmath>

/*
 * Should be returned from any of the 7 standard operations.
 * This helps diagnose any faults and safely shut Baby down.
 * Feel free to add more if you think they could be useful.
 */
#define SUCCESS	(0)					//task succeeded
#define OUT_OF_RANGE (1)			//arithmetical sum went beyond what Baby can store
#define STOP (2)					//stop execution
#define CONTINUE (3)				//continue execution
#define NOT_ENOUGH_MEMORY (4)		//Baby ran out of memory for the current program

using namespace std;

class Baby
{
private:
	bool store[256][32];			//Baby's memory store
	string accumulator;				//Accumulator register
	string currentInstruction;		//Current instruction register
	string presentInstruction;		//Present instruction register
	string register4;				//Second data register
	int memorySize;					//Number of lines held in store

//See baby.cpp for details of each function
public:
	Baby();
	Baby(int size);

	void fetch();
	int getCurrentInstructionAddress();
	void incrementCurrentInstruction();

	string readLineFromStore(int lineNumber);
	int getOperand();
	int getOpcode();

	void JMP();
	void JRP();
	void LDN();
	void STO();
	int SUB();
	void CMP();
	int ADD();
	int MUL();
	int DIV();
	void MVF();
	void MVT();

	void insertInstruction(string line, int lineNumber);

	int decode();
	void printState();

	int test();

	~Baby();
};

#endif /* BABY_H_ */

