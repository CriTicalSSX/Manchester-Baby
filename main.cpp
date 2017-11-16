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
#include "baby.h"

//For file input errors
#define BAD_FILE (1)
#define FILE_NOT_FOUND (2)
#define EXCEEDED_LENGTH (3)

using namespace std;

void clear()
{
	cout << string( 100, '\n' );
}

void displayInstructionSet()
{
	clear();

	cout << "OPCODE\t\tFUNCTION\tDESCRIPTION" << endl;
	cout << "0000\t\tJMP\t\tJumps to instruction stored in present instruction register" << endl;
	cout << "1000\t\tJRP\t\tJumps to instruction stored in present instruction register + value of current instruction register" << endl;
	cout << "0100\t\tLDN\t\tNegates value in present instruction register and stores it in accumulator register" << endl;
	cout << "1100\t\tSTO\t\tStore the number in the accumulator register in the address pointed to by the current instruction register" << endl;
	cout << "0010/1010\t\tSUB\t\tSubtract value of present instruction register from accumulator register and store in accumulator register" << endl;
	cout << "0110\t\tCMP\t\tSkips the next instruction if the accumulator is negative" << endl;
	cout << "1110\t\tSTP\t\tStops execution of any further instructions" << endl;
	cout << "0001/1001\t\tADD\t\tAdd value of present instruction register to accumulator register and store in accumulator register" << endl;
	cout << "0101/1101\t\tMUL\t\tMultiply value of present instruction register by value of accumulator register and store in accumulator register" << endl;
	cout << "0011/1011\t\tDIV\t\tDivide value of accumulator register by present instruction register and store in accumulator register" << endl;
	cout << "0111\t\tMVF\t\tMoves instruction in current instruction register to Register 4" << endl;
	cout << "1111\t\tMVT\t\tMoves instruction in Register 4 to current instruction register" << endl << endl;
}

/*
 *	Checks whether a user's file is OK
 */
int checkFile(string name)
{
	ifstream input(name);
	if (input.is_open())		//if file can be opened
	{
		string line;
		bool fileOK = true;
		int lineCount = 0;

		while (getline(input, line) && fileOK == true)
		{
			int numberCount = 0;
			lineCount++;

			if (lineCount > 31)					//if there are too many lines for Baby to store
			{
				fileOK = false;
			}
			
			for (unsigned i=0; i<line.length(); i++)
			{
				if (line[i] == '0' || line[i] == '1')
				{
					numberCount++;
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

		if (lineCount > 31)
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

/*
 * Function that will allow the program to continue if the user enters an empty value. Otherwise, ends
 * the program.
 */
int cont()
{
	string userInput = "";

	cin >> userInput;

	if(userInput != "x")
	{
		return END_PROGRAM;
	}
	else
	{
		return CONTINUE;
	}
}

int main()
{
	Baby *baby = new Baby();		//A pointer to a new baby object. Refer to it as baby, not *baby

	baby->printState();

	cout << endl << "This is the Manchester Baby!" << endl;

	char input;
	while (input != '1')
	{
		cout << "Choose an option:" << endl;
		cout << "1. Load a machine code program into Baby" << endl;
		cout << "2. View Baby's instruction set" << endl;
		cout << "3. Quit" << endl;

		cin >> input;

		while (input != '1' && input != '2' && input != '3')
		{
			cout << "Error. Please enter either 1, 2 or 3." << endl;

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
			clear();
			return 0;
		}
	}

	//Asking user for file name to open
	string name;
	cout << endl << "Enter the name of the machine code file for Baby to run." << endl;
	getline(cin, name);

	if (checkFile(name) == FILE_NOT_FOUND)
	{
		cout << name << " could not be found." << endl << endl;
		return 1;
	}
	else if (checkFile(name) == BAD_FILE)
	{
		cout << name << " did not contain proper machine code." << endl << endl;
		return 1;
	}
	else if (checkFile(name) == EXCEEDED_LENGTH)
	{
		cout << name << " contained too many instructions for Baby to store." << endl << endl;
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

		bool stop = false;

		do
		{
			clear();

			baby->incrementCurrentInstruction();
			baby->fetch();

			baby->printState();

			if(baby->decode() == STOP)
			{
				stop = true;
			}
			else if(cont() == END_PROGRAM)
			{
				stop = true;
			}
			else if (baby->getCurrentInstructionAddress() > 30)
			{
				stop = true;
			}
		}
		while (stop == false);

		if (baby->getCurrentInstructionAddress() > 30)
		{
			cout << "Error! Reached end of program without finding STOP." << endl;
		}

		cout << "Terminating." << endl;
	}

	return 0;
}
