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

//finds the file and sets the data from the file to the var text_file[32]
void file_search() {

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
  return 1;
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
	}
}

int main() {

	//sets everyline to # 
  	for (int times = 0; times < 32; times++) {
  		text_file[times] = "#";
  	}

  	//fills data (remove later)
	text_file[0] = "          VAR 0       ; Declare 32-bit variable to fill space ";
	text_file[1] = "START:    LDN NUM01   ; Copy variable to accumulator (negated)";
	text_file[2] = "          SUB NUM02   ; Subtract variable from accumulator to get sum (negated)";
	text_file[3] = "          STO MYSUM   ; Store accumulator to variable - saves answer";
	text_file[4] = "          LDN MYSUM   ; Copy variable to accumulator (negated)";
	text_file[5] = "          STw MYSUM   ; Store accumulator to variable";
	text_file[6] = "END:      STP         ; Stop processor";
	text_file[7] = "NUM01:    VAR 1025    ; Declare 32-bit variable";
	text_file[8] = "NUM02:    VAR 621     ; Declare 32-bit variable";
	text_file[9] = "MYSUM:    VAR 0       ; Declare 32-bit variable";

	string return_error = "";

	file_search();
	converter();
    return_error = scanner();
    cout << return_error << endl;
}

