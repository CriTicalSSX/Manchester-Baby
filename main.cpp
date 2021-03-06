/*
 * Sam Glendenning
 * Dylan Keningale
 * Matthew Daldry
 * Will Lockett
 *
 * COMPUTER SYSTEMS 2A: ASSIGNMENT 3
 * THE MANCHESTER SMALL-SCALE EXPERIMENTAL MACHINE (BABY)
 *
 * Main source file
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include "baby.h"

//For file input errors
#define BAD_FILE (1)
#define FILE_NOT_FOUND (2)
#define EXCEEDED_LENGTH (3)

using namespace std;

/*
* Clears the screen
*/
void clear()
{
	cout << string( 100, '\n' );
}

/*
* Outputs all of Baby's instructions to the terminal
*/
void displayInstructionSet()
{
	clear();

	cout << "OPCODE\t\tFUNCTION\tDESCRIPTION" << endl;
	cout << "0000\t\tJMP\t\tJumps to instruction specified by present instruction operand" << endl;
	cout << "1000\t\tJRP\t\tJumps to instruction specified by present instruction operand + value of current instruction register" << endl;
	cout << "0100\t\tLDN\t\tNegates value of line specified by present instruction operand and stores it in accumulator register" << endl;
	cout << "1100\t\tSTO\t\tStore the number in the accumulator register in the line number specified by the operand of present instruction." << endl;
	cout << "0010/1010\tSUB\t\tSubtract value of line specified by present instruction from accumulator register and store in accumulator register" << endl;
	cout << "0110\t\tCMP\t\tSkips the next instruction if the accumulator is negative" << endl;
	cout << "1110\t\tSTP\t\tStops execution of any further instructions" << endl;
	cout << "0001/1001\tADD\t\tAdd value of line specified by present instruction to accumulator register and store in accumulator register" << endl;
	cout << "0101/1101\tMUL\t\tMultiply value of line specified by present instruction by value of accumulator register and store in accumulator register" << endl;
	cout << "0011/1011\tDIV\t\tDivide value of accumulator register by line specified by present instruction and store in accumulator register" << endl;
	cout << "0111\t\tMVF\t\tMoves instruction in current instruction register to Register 4" << endl;
	cout << "1111\t\tMVT\t\tMoves instruction in Register 4 to current instruction register" << endl << endl;
}

/*
 *	Checks whether a user's file is OK
 */
int checkFile(string name, int memorySize)
{
	ifstream input(name);
	if (input.is_open())		//if file can be opened
	{
		string line;
		bool fileOK = true;
		int lineCount = 0;

		while (getline(input, line) && fileOK == true)			//while there are lines in the file to read
		{
			int numberCount = 0;
			lineCount++;

			if (lineCount > (memorySize-1))					//if there are too many lines for Baby to store
			{
				fileOK = false;
			}
			
			for (unsigned i=0; i<line.length(); i++)
			{
				if (line[i] == '0' || line[i] == '1')		
				{
					numberCount++;							//Used to ensure there are 32 bits per line
				}
			}
			
			if (numberCount != 32)			//if line does not contain 32 bits
			{
				fileOK = false;
			}
			else
			{
				for (unsigned i=0; i<line.length(); i++)
				{
					if (line[i] != '0' && line[i] != '1')			//if any character is not equal to a 0 or a 1
					{
						fileOK = false;
					}
				}
			}
		}

		input.close();

		if (lineCount > (memorySize-1))
		{
			return EXCEEDED_LENGTH;
		}

		if (fileOK == false)
		{
			return BAD_FILE;
		}

		return SUCCESS;
	}
	else
	{
		return FILE_NOT_FOUND;
	}
}

int main()
{
	cout << endl << "This is the Manchester Baby!" << endl;

	char input;
	int sizeInput = 32;

	while (input != '1')
	{
		//Main menu
		cout << "Choose an option:" << endl;
		cout << "1. Load a machine code program into Baby" << endl;
		cout << "2. View Baby's instruction set" << endl;
		cout << "3. Change Baby's memory size" << endl;
		cout << "4. Quit" << endl;

		cin >> input;

		while (input != '1' && input != '2' && input != '3' && input != '4')
		{
			cout << "Error. Please enter either 1, 2, 3 or 4." << endl;

			if (!cin)
			{
				cin.clear();
				cin.ignore();
			}

			cin >> input;
		}

		cin.clear();
		cin.ignore();

		if (input == '2')
		{
			displayInstructionSet();
		}

		if (input == '3')
		{
			cout << "What size of memory would you like?" << endl;
			cout << "Please enter a size between 32 and 256 lines." << endl;
						
			cin >> sizeInput;

			while(!cin || sizeInput < 32 || sizeInput > 256)
			{
				if(!cin)
				{
					cout << "Error. Please enter an integer value." << endl;
				
					cin.clear();
					cin.ignore();
				}
				else
				{
					cout << "Error. Please enter a size between 32 and 256 lines." << endl;
				}

			cin >> sizeInput;
			}
		}

		if (input == '4')
		{
			return 0;
		}
	}

	Baby *baby = new Baby(sizeInput);

	//Asking user for file name to open
	string name;
	cout << endl << "Enter the name of the machine code file for Baby to run." << endl;
	getline(cin, name);

	if (checkFile(name, sizeInput) == FILE_NOT_FOUND)
	{
		cout << name << " could not be found." << endl << endl;
		delete baby;
		return 1;
	}
	else if (checkFile(name, sizeInput) == BAD_FILE)
	{
		cout << name << " did not contain proper machine code." << endl << endl;
		delete baby;
		return 1;
	}
	else if (checkFile(name, sizeInput) == EXCEEDED_LENGTH)
	{
		cout << name << " contained too many instructions for Baby to store." << endl << endl;
		delete baby;
		return 1;
	}
	else
	{
		//Actually inserting the file into the store
		string line;
		ifstream input(name);
		int lineNumber = 0;

		while (getline(input, line))
		{
			baby->insertInstruction(line, lineNumber);
			lineNumber++;
		}

		input.close();

		cout << "Program successfully written to Baby." << endl << endl;

//////////////////////////////////////////////////////////////////////////////////////////
		//PROGRAM LOOP

		bool stop = false;

		do
		{
			clear();		//clear screen

			baby->incrementCurrentInstruction();		//does what it says on the tin. Starts execution of programs from second line of store (this is where they are loaded to)
			baby->fetch();				//retrieve the instruction from that address

			baby->printState();			//print the current state of Baby

			int returnCode = baby->decode();		//decode instruction and execute

			if(returnCode == STOP)					
			{
				stop = true;
			}
			else if (returnCode == NOT_ENOUGH_MEMORY)			//Instruction tried to access non-existent part of memory
			{
				cout << "Instruction error! Present instruction refers to line " << baby->getOperand() << ", which is non-existent!" << endl;
			}

			if (baby->getCurrentInstructionAddress() > sizeInput -1)		//Reached end of store without finding stop instruction
			{
				stop = true;
			}
		}
		while (stop == false);

		if (baby->getCurrentInstructionAddress() > sizeInput-1)
		{
			cout << "Error! Reached end of program without finding STOP." << endl;
		}

		cout << "Terminating." << endl;
	}

	delete baby;
	return 0;
}
