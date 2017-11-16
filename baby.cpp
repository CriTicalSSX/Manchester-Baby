/*
 * Sam Glendenning
 * Dylan Keningale
 * Matthew Daldry
 * Will Lockett
 *
 * COMPUTER SYSTEMS 2A: ASSIGNMENT 3
 * THE MANCHESTER SMALL-SCALE EXPERIMENTAL MACHINE (BABY)
 *
 * Baby source file
 */

#include <iostream>
#include <bitset>
#include "baby.h"
#include <stdlib.h>
#include <fstream>
#include <cmath>

using namespace std;

/*
 * Constructor for Baby. Assigns all values in the store to 0 and sets accumulator to 0.
 * Accumulator will increase to 1 before any instructions are performed.
 * Currently, accumulator value of 1 indicates first line should be read (which has index 0).
 * This can, and probably should, be changed.
 */
Baby::Baby()
{
	accumulator = "00000000000000000000000000000000";
	currentInstruction = "00000000000000000000000000000000";
	presentInstruction = "00000000000000000000000000000000";
	register4 = "00000000000000000000000000000000";
	memorySize = 32;

	for (int i=0; i<memorySize; i++)
	{
		for (int j=0; j<32; j++)
		{
			store[i][j] = 0;
		}
	}
}

//Alternate constructor for Baby. Allows for different sizes of memory.
Baby::Baby(int size)
{
	accumulator = "00000000000000000000000000000000";
	currentInstruction = "00000000000000000000000000000000";
	presentInstruction = "00000000000000000000000000000000";
	register4 = "00000000000000000000000000000000";
	memorySize = size;

	for (int i=0; i<memorySize; i++)
	{
		for (int j=0; j<32; j++)
		{
			store[i][j] = 0;
		}
	}
}

/*
 * Destructor for Baby. Sets all values in the store to 0 and sets all registers' values
 * to empty strings.
 */
Baby::~Baby()
{
	for (int i=0; i<memorySize; i++)
	{
		for (int j=0; j<32; j++)
		{
			store[i][j] = 0;
		}
	}

	accumulator = "";
	currentInstruction = "";
	presentInstruction = "";
	register4 = "";
	memorySize = 0;

	cout << "Shutting Baby down..." << endl;
}

/*
 * Returns the current value of the current instruction register.
 */
int Baby::getCurrentInstructionAddress()
{
	return binaryToDecimal(currentInstruction);
}

/*
 * Increment current instruction by 1
 */
void Baby::incrementCurrentInstruction()
{
	for (unsigned i=0; i<currentInstruction.length(); i++)
	{
		if (currentInstruction[i] == '0')
		{
			currentInstruction[i] = '1';

			for (int j=i; j>-0; j--)
			{
				currentInstruction[j-1] = '0';
			}

			break;
		}
	}
}


/*
 * Sets a new present instruction.
 */
void Baby::fetch()
{
	int lineNumber = binaryToDecimal(currentInstruction);
	presentInstruction = readLineFromStore(lineNumber);
}

/*
 * Reads a 32-bit number from the store. Requires the entire store to be passed in and the
 * appropriate line number (first line = 0).
 * In future, this line will not need an integer representing the line number to be passed in.
 * What will actually happen is it will just read the accumulator (since accumulator will hold
 * the appropriate operand from the last instruction? Is that right?)
 */
string Baby::readLineFromStore(int lineNumber)
{
	string instruction = "";

	for (int i=0; i<32; i++)
	{
		instruction += to_string(store[lineNumber][i]);
	}

	return instruction;
}

/*
 * Retrieves the decimal value of the opcode of a specific line in the store.
 * This function may be better off just reading a string instruction passed in from the
 * current instruction register, so subject to change.
 */
int Baby::getOpcode()
{
	string opcode = "";

	for (int i=13; i<17; i++)
	{
		opcode += presentInstruction[i];		//Obtains the entire store line as string
	}

	return binaryToDecimal(opcode);
}

/*
 * Retrieves the decimal value of the operand of a specific line in the store.
 * This function may be better off just reading a string instruction passed in from the
 * current instruction register, so subject to change.
 */
int Baby::getOperand()
{
	string operand = "";

	for (int i=0; i<5; i++)
	{
		operand += presentInstruction[i];
	}

	return binaryToDecimal(operand);		//Retrieves the value 0-31 for the operand
}

/*
 * Simple conversion from a string of a binary number of any length to its integer equivalent
 */
int Baby::binaryToDecimal(string binary)
{
	int decimal = 0;

	if (binary[binary.length() - 1] == '0' || binary.length() < 32)
	{
		for (unsigned int i=0; i<binary.length(); i++)
		{
			if (binary[i] == '1')
			{
				decimal += pow(2, i);
			}
		}
	}
	else if (binary[binary.length() - 1] == '1' && binary.length() == 32)
	{
		if (binary[0] == '1')
		{
			binary[0] ='0';
		}
		else
		{
			for (unsigned int i=0; i<binary.length(); i++)
			{
				if (binary[i] == '1')
				{
					binary[i] = '0';

					for (int j=0; j>-1; j--)
					{
						binary[i] = '1';
					}

					break;
				}
			}
		}

		for (unsigned i=0; i<binary.length(); i++)
		{
			if (binary[i] == '0')
			{
				binary[i] = '1';
			}
			else
				binary[i] = '0';
		}

		for (unsigned int i=0; i<binary.length(); i++)
		{
			if (binary[i] == '1')
			{
				decimal += pow(2, i);
			}
		}

		decimal = decimal * -1;
	}

	return decimal;
}

/*
 * Jump to the address stored in present instruction
 */
void Baby::JMP()
{
	int presentInstructionValue = binaryToDecimal(presentInstruction);
	string binary = bitset<32>(presentInstructionValue).to_string();

	currentInstruction = "00000000000000000000000000000000";

	for (unsigned int i=0; i<binary.length(); i++)
	{
		if (binary[i] == '0')
		{
			currentInstruction[31 - i] = '0';
		}
		else
		{
			currentInstruction[31 - i] = '1';
		}
	}
}

/*
 * Jump to the address stored in currentInstruction + the value stored in presentInstruction
 */
void Baby::JRP()
{
	int currentInstructionValue = binaryToDecimal(currentInstruction);
	int presentInstructionValue = binaryToDecimal(presentInstruction);

	string binary = bitset<32>(currentInstructionValue + presentInstructionValue).to_string();

	for (unsigned int i=0; i<binary.length(); i++)
	{
		if (binary[i] == '0')
		{
			currentInstruction[31 - i] = '0';
		}
		else
		{
			currentInstruction[31 - i] = '1';
		}
	}
}

/*
 * Retrieves the decimal value of the present instruction, converts it to binary and then
 * swaps its values round so it reads left to right. This is stored in the accumulator
 */
void Baby::LDN()
{
	int negativePresentInstruction = binaryToDecimal(presentInstruction) * -1;
	string negativeBinary = bitset<32>(negativePresentInstruction).to_string();

	accumulator = "00000000000000000000000000000000";

	for (unsigned int i=0; i<negativeBinary.length(); i++)
	{
		if (negativeBinary[i] == '0')
		{
			accumulator[31 - i] = '0';
		}
		else
		{
			accumulator[31 - i] = '1';
		}
	}
}

/*
 * Test STO function. STO is used to store the value of the accumulator register in a
 * specified location in the store. In the test function, this changes the first line of
 * the store (run the program and see).
 */
void Baby::STO()
{
	for (int i=0; i<32; i++)
	{
		if (accumulator[i] == '0')
		{
			store[getCurrentInstructionAddress()][i] = 0;
		}
		else
		{
			store[getCurrentInstructionAddress()][i] = 1;
		}
	}
}

/*
 * Takes the decimal version of the present instruction away from the decimal version of the
 * accumulator. The decimals are obtained through Dylan's binary conversion function.
 * The calculated decimal is then transformed back to binary through a handy to_string function
 * in the biset library
 */
int Baby::SUB()
{
	int result = binaryToDecimal(accumulator) - binaryToDecimal(presentInstruction);

	if (result > 2147483647 || result < -2147483647)
	{
		return OUT_OF_RANGE;
	}

	string binary = bitset<32>(result).to_string();

	for (unsigned int i=0; i<binary.length(); i++)
	{
		if (binary[i] == '0')
		{
			accumulator[31 - i] = '0';
		}
		else
		{
			accumulator[31 - i] = '1';
		}
	}

	return SUCCESS;
}

/*
 * Returns true if the rightmost digit is 1, which usually indicates the number is negative
 */
void Baby::CMP()
{
	if (accumulator[31] == '1')
	{
		if (currentInstruction[0] == '0')
		{
			currentInstruction[0] = '1';
		}
		else
		{
			for (unsigned int i=0; i<currentInstruction.length(); i++)
			{
				if (currentInstruction[i] == '0')
				{
					currentInstruction[i] = '1';

					for (int j=i; j>-0; j--)
					{
						currentInstruction[j-1] = '0';
					}

					break;
				}
			}
		}
	}
}

int Baby::ADD()
{
	int result = binaryToDecimal(accumulator) + binaryToDecimal(presentInstruction);

		if (result > 2147483647 || result < -2147483647)
		{
			return OUT_OF_RANGE;
		}

		string binary = bitset<32>(result).to_string();

		for (unsigned int i=0; i<binary.length(); i++)
		{
			if (binary[i] == '0')
			{
				accumulator[31 - i] = '0';
			}
			else
			{
				accumulator[31 - i] = '1';
			}
		}

		return SUCCESS;
}


int Baby::MUL()
{
	int result = binaryToDecimal(accumulator) * binaryToDecimal(presentInstruction);

	if (result > 2147483647 || result < -2147483647)
	{
		return OUT_OF_RANGE;
	}

	string binary = bitset<32>(result).to_string();

	for (unsigned int i=0; i<binary.length(); i++)
	{
		if (binary[i] == '0')
		{
			accumulator[31 - i] = '0';
		}
		else
		{
			accumulator[31 - i] = '1';
		}
	}

	return SUCCESS;
}

int Baby::DIV()
{
	int result = binaryToDecimal(accumulator) / binaryToDecimal(presentInstruction);

	if (result > 2147483647 || result < -2147483647)
	{
		return OUT_OF_RANGE;
	}

	string binary = bitset<32>(result).to_string();

	for (unsigned int i=0; i<binary.length(); i++)
	{
		if (binary[i] == '0')
		{
			accumulator[31 - i] = '0';
		}
		else
		{
			accumulator[31 - i] = '1';
		}
	}

	return SUCCESS;
}

void Baby::MVF()
{
	register4 = currentInstruction;
}

void Baby::MVT()
{
	currentInstruction = register4;
}

void Baby::insertInstruction(string line, int lineNumber)
{
	for (unsigned i=0; i<line.length(); i++)
	{
		if (line[i] == '0')
		{
			store[lineNumber][i] = 0;
		}
		else
		{
			store[lineNumber][i] = 1;
		}
	}
}

/*
 * Calls the appropriate function based on the opcode given in the line number.
 */
int Baby::decode()
{
	int opcode = getOpcode();

	cout << "Press 'x' to execute or any other key to terminate program." << endl;

	if(opcode == 0)
	{
		cout << "Performing JMP..." << endl;
		JMP();
		return CONTINUE;
	}
	else if(opcode == 1)
	{
		cout << "Performing JRP..." << endl;
		JRP();
		return CONTINUE;
	}
	else if(opcode == 2)
	{
		cout << "Performing LDN..." << endl;
		LDN();
		return CONTINUE;
	}
	else if(opcode == 3)
	{
		cout << "Performing STO..." << endl;
		STO();
		return CONTINUE;
	}
	else if(opcode == 4 || opcode == 5)
	{
		cout << "Performing SUB..." << endl;

		if (SUB() == OUT_OF_RANGE)
		{
			cout << "Sum went out of range!" << endl;
			return STOP;
		}

		return CONTINUE;
	}
	else if(opcode == 6)
	{
		cout << "Performing CMP..." << endl;
		CMP();
		return CONTINUE;
	}
	else if (opcode == 7)
	{
		return STOP;
	}
	else if (opcode == 8 || opcode == 9)
	{
		cout << "Performing ADD..." << endl;
		if (ADD() == OUT_OF_RANGE)
		{
			cout << "Sum went out of range!" << endl;
			return STOP;
		}

		return CONTINUE;
	}
	else if (opcode == 10 || opcode == 11)
	{
		cout << "Performing MUL..." << endl;
		if (MUL() == OUT_OF_RANGE)
		{
			cout << "Multiplication calculation went out of range!" << endl;
			return STOP;
		}

		return CONTINUE;
	}
	else if (opcode == 12 || opcode == 13)
	{
		cout << "Performing DIV..." << endl;
		if (DIV() == OUT_OF_RANGE)
		{
			cout << "Division calculation went out of range!" << endl;
			return STOP;
		}

		return CONTINUE;
	}
	else if (opcode == 14)
	{
		cout << "Performing MVF..." << endl;
		MVF();
		return CONTINUE;
	}
	else
	{
		cout << "Performing MVT..." << endl;
		MVT();
		return CONTINUE;
	}
}

/*
 * Prints the current state of the baby: the store, the accumulator, the current instruction
 * and the present instruction.
 */
void Baby::printState()
{
	cout << "Store:" << endl;

	for (int i=0; i<memorySize; i++)
	{
		for (int j=0; j<32; j++)
		{
			cout << store[i][j];
		}
		
		cout << endl;
	}

	cout << endl << "Accumulator: " << accumulator << " = " << binaryToDecimal(accumulator) << endl;
	cout << "Current Instruction: " << currentInstruction << " = " << binaryToDecimal(currentInstruction) << endl;
	cout << "Present Instruction: " << presentInstruction << " = " << binaryToDecimal(presentInstruction) << endl;
	cout << "Register 4: " << register4 << " = " << binaryToDecimal(register4) << endl << endl;
	cout << "Operand: " << getOperand() << endl;
	cout << "Opcode: " << getOpcode() << endl;
}

/*
 * Simple demo of Baby's functionality.
 * NOTE: The variables instruction, accumulator, operand and opcode are NOT affiliated with
 * Baby's own variables of similar names. This was purely a demo to test Baby's functionality
 * with such variables.
 */
int Baby::test()
{
	cout << "Initialising Baby..." << endl;

	printState();

	cout << "Testing STO function..." << endl;

	cout << "Output of first line of store is " << readLineFromStore(0) << endl;
	cout << "Operand = " << getOperand() << endl;
	cout << "Opcode = " << getOpcode() << endl;

	cout << endl << "Changing first line of store..." << endl;

	accumulator = "00111001011111010010001000010101";	//Random value of the accumulator
	STO();

	cout << endl << "STORE AFTER STO OPERATION:" << endl;
	printState();

	cout << endl << "Output of first line of store is " << readLineFromStore(0) << endl;
	cout << "Operand = " << getOperand() << endl;
	cout << "Opcode = " << getOpcode() << endl << endl;
	cout << "STO test complete!" << endl << endl;



	cout << "Testing CMP function..." << endl;
	cout << "CMP tests to see if the accumulator is negative. If it is, increment the current instruction register." << endl;

	currentInstruction = "11100000000000000000000000000000";
	accumulator = "11001100000101000000000010101110";

	cout << "Testing CMP with positive accumulator..." << endl << endl;
	cout << "Current instruction register before CMP check: " << currentInstruction << endl << endl;
	cout << "Performing CMP operation..." << endl;

	CMP();

	cout << "Current instruction register after CMP check: " << currentInstruction << endl << endl;

	accumulator = "11001100000101000000000010101111";

	cout << "Testing with negative number in accumulator" << endl;
	cout << "Current instruction register before CMP check: " << currentInstruction << endl;
	cout << "Performing CMP operation..." << endl;

	CMP();

	cout << "Current instruction register after CMP check: " << currentInstruction << endl;
	cout << "CMP test complete!" << endl << endl;



	cout << "Testing JMP function..." << endl;
	cout << "JMP changes the current instruction register to the value of present instruction." << endl;
	cout << "Setting present instruction equal to 11010000000000000000000000000000 (13)" << endl;

	presentInstruction = "11010000000000000000000000000000";

	cout << "Current instruction register before JMP: " << currentInstruction << endl;

	JMP();

	cout << "Current instruction register after JMP: " << currentInstruction << endl;
	cout << "JMP test complete!" << endl << endl;



	cout << "Testing JRP function..." << endl;
	cout << "JRP adds the value of present instruction to the current value of current instruction." << endl;
	cout << "Setting current instruction register to 10010000000000000000000000000000" << endl;
	cout << "Setting present instruction register to 01110000000000000000000000000000" << endl;

	currentInstruction = "10010000000000000000000000000000";
	presentInstruction = "01110000000000000000000000000000";

	cout << "JRP function jumps to address of current register (line 9) plus value of present register (14)" << endl;
	cout << "So current instruction register will contain value 23" << endl;
	cout << "Performing JRP function..." << endl;

	JRP();

	cout << endl << "Accumulator: " << accumulator << endl;
	cout << "Current Instruction: " << currentInstruction << endl;
	cout << "Present Instruction: " << presentInstruction << endl << endl;
	cout << "JRP test complete!" << endl << endl;


	cout << "Testing LDN function..." << endl;
	cout << "LDN negates present instruction register and stores it in accumulator." << endl;
	cout << "Setting present instruction register to 01110000010000000011000000000000" << endl;

	presentInstruction = "01110000010000000011000000000000";

	cout << endl << "Accumulator: " << accumulator << endl;
	cout << "Current Instruction: " << currentInstruction << endl;
	cout << "Present Instruction: " << presentInstruction << endl << endl;
	cout << "Performing LDN function..." << endl;

	LDN();

	cout << endl << "Accumulator: " << accumulator << endl;
	cout << "Current Instruction: " << currentInstruction << endl;
	cout << "Present Instruction: " << presentInstruction << endl << endl;
	cout << "LDN test complete!" << endl << endl;



	cout << "Testing SUB function..." << endl;
	cout << "SUB subtracts present instruction from accumulator and stores result in accumulator" << endl;

	presentInstruction = "01110000010000000011000000000001";

	cout << endl << "Accumulator: " << accumulator << endl;
	cout << "Current Instruction: " << currentInstruction << endl;
	cout << "Present Instruction: " << presentInstruction << endl << endl;

	cout << "Performing SUB function..." << endl;

	if (SUB() == OUT_OF_RANGE)
	{
		cout << "Could not perform SUB operation. Sum went out of range." << endl;
	}
	else
	{
		cout << endl << "Accumulator: " << accumulator << endl;
		cout << "Current Instruction: " << currentInstruction << endl;
		cout << "Present Instruction: " << presentInstruction << endl << endl;
	}

	cout << "SUB test complete!" << endl << endl;

	return SUCCESS;
}


