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

/*
 * Should be returned from any of the 7 standard operations.
 * This helps diagnose any faults and safely shut Baby down.
 * Feel free to add more if you think they could be useful.
 */
#define SUCCESS	(0)
#define FAIL (1)
#define OUT_OF_RANGE (2)
#define STOP (3)
#define CONTINUE (4)
#define END_PROGRAM (5)

using namespace std;

class Baby
{
private:
	int store[32][32];				//Baby's memory store
	string accumulator;				//Accumulator register
	string currentInstruction;		//Current instruction register
	string presentInstruction;		//Present instruction register

//See baby.cpp for details of each function
public:
	Baby();

	string getAccumulator();
	string getCurrentInstruction();
	string getPresentInstruction();
	void setPresentInstruction(string NewInstruction);
	void setCurrentInstruction(string NewInstruction);

	string readLineFromStore(int lineNumber);
	int getOperand(int lineNumber);
	int getOpcode(int lineNumber);
	int binaryToDecimal(string binary);

	void JMP();
	void JRP();
	void LDN();
	void STO(int operand);
	int SUB();
	void CMP();

	void insertInstruction(string line, int lineNumber);

	int callOpcode(int lineNumber);
	void printState();
	int cont();

	int test();

	~Baby();
};

#endif /* BABY_H_ */

