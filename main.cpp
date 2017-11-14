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
			lineCount++;

			if (lineCount > 31)
			{
				fileOK = false;
			}
			else if (line.length() != 32)			//if each line does not contain 32 bits
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

int main()
{
	Baby *baby = new Baby();		//A pointer to a new baby object. Refer to it as baby, not *baby

	//Asking user for file name to open
	string name;
	cout << "Enter the name of the machine code file for Baby to run." << endl;
	getline(cin, name);

	if (checkFile(name) == FILE_NOT_FOUND)
	{
		cout << name << " could not be found." << endl << endl;
	}
	else if (checkFile(name) == BAD_FILE)
	{
		cout << name << " did not contain proper machine code." << endl << endl;
	}
	else if (checkFile(name) == EXCEEDED_LENGTH)
	{
		cout << name << " contained too many instructions for Baby to store." << endl << endl;
	}
	else
	{
		//Actually inserting the file into the store
		string line;
		ifstream input(name);
		int lineNumber = 1;

		while (getline(input, line))
		{
			baby->insertInstruction(line, lineNumber);
			lineNumber++;
		}

		input.close();

		cout << "Program successfully written to Baby." << endl << endl;
	}

//////////////////////////////////////////////////////////////////////////////////////////

	bool stop = false;
	int lineNumber = 1;

	while (stop == false)
	{
		baby->setCurrentInstruction(baby->readLineFromStore(lineNumber));

		baby->setPresentInstruction(baby->getCurrentInstruction());

		if(baby->callOpcode(lineNumber) == STOP)
		{
			stop = true;
		}
	
		baby->printState();

		if(baby->cont() == END_PROGRAM)
		{
			stop = true;
		}

		lineNumber++;
	}

	cout << "Terminating." << endl;
}

