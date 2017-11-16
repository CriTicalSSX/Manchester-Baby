/*  AC21009 assignment 3
    Manchester Baby Assembler

    Will Lockett
    Dylan Keningale
    Sam Glendenning
    Matthew Daldry
*/

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;

string text_file[32];
string memoriser[32];

//custom made instruction set for compiling
//first column is the assembly code name
//second is if it needs a integer after it (y/n)
//third is its value in binary for conversion
//var has no value as the line its on gets converted to the number it stores
string instructions[8][3] = {
{"VAR", "y", "NULL"},
{"JMP", "y", "000"},
{"JRP", "y", "100"},
{"LDN", "y", "010"},
{"STO", "y", "110"},
{"SUB", "y", "001"},
{"CMP", "n", "011"},
{"STP", "n", "111"},
};

//loads a file into the array to be processed
int loadFile(string fileName){

	//opens a file input stream for the given file
	ifstream inFile(fileName);

	//checks the input file exists
	if(!inFile){
		cout << "Error: File can't be opened!\n";
		return 1;
	}
	
	string currLine;
	int i = 0;
	int loc = 0;

	//while the next line does not equal null
	while(getline(inFile, currLine)){

		loc = currLine.find_first_not_of("\t");

		//if the first character of the line is a ; then the line can be ignored
		if(currLine.at(loc) != (';')){

			//checks there is space in the store before adding
			if(i < 32){
				
				//the first free bucket stores currLine
				text_file[i] = currLine;
				i++;
				cout << "Added to text_file\t" << currLine << endl;
			}
			else{
				cout << "Error: Trying to add to a full store!\n";
				return 1;
			}
		}
	}

	//closes the file input stream
	inFile.close();

	return 0;
}

int converter() {

  	for (int times = 0; times < 32; times++) {

    		int length = text_file[times].size();
    		string current = text_file[times];
    		int end = length;    //before the ; (for defining what a comment is)
    	

    		if (current.at(0) == '#') {
			return 1;  //completed the current set
		}

		//finds out where the comments are
    		for (int char_check = 0;char_check < length; char_check++) {

    	  		char curr_char = current.at(char_check);

      			if (curr_char == ';') {
					end = char_check;
     			}
    		}

    		//removes the comments
    		text_file[times] = current.substr(0,end);

    		cout << text_file[times] << endl;
	}		
}

//scans lines searching for errors and memorising variable locations;
string scanner() {

	string error = "";

	for (int line = 0;line < 32; line++) {

		//if it finds the # (denoting a null line) it returns scan complete
		if (text_file[line].at(0) == '#') {
			return "Scan complete";  //completed the current set
		}
		
		//skips past variable names and such
		int pos = 0;
		size_t found = text_file[line].find(':');
		int start = 0;

		if (found!=string::npos) {
			pos = (text_file[line].find_first_of(':')+1);
		}

		bool correct = false;

		//checks that the opcode matches the instruction set
		start = text_file[line].find_first_not_of(" \t",pos);
		string holder = text_file[line].substr(start,3);

		for (int times = 0; times < 8 ; times++) {

			if (holder == instructions[times][0]) {
				correct = true;
			}
		}

		//returns an error if the opcode is wrong with the line its on
		if (correct == false) {
			error = "error on line ";
			error += to_string(line); 
			error += " problem with opcode : ";
			error += holder;
			return error;
		}

		cout << text_file[line] << "\n";
	}
}

//converts the assembly code into machine code line by line
void convertToBinary(){
}

//writes the final machine code to a text file that can be read by the baby
void writeFile(){
}

int main() {

	//sets everyline to # 
  	for (int times = 0; times < 32; times++) {
  		text_file[times] = "#";
  	}

	string return_error = "";

	string fileName;

	cout << "\nPlease enter the address of the assembly code file\n";
	cin >> fileName;

	loadFile(fileName);

	cout << "\nFinished loading.\n\n";

	converter();

	cout << "\nconverting finished\n";

	return_error = scanner();
    	cout << return_error << endl;
}

