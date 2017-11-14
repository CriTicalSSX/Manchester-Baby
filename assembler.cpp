#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;

string text_file[32] = "#";

void file_search() {
  	string name;
			cout << "Enter the name of the file to open." << endl;
			cin.ignore();
			getline(cin, name);

			ifstream input(name);
			if (input.is_open())
			{
				string line;
				while (getline(input, line))
				{
					cout << line << '\n';
				}

				input.close();
			}
			else
			{
				cout << "Unable to open this file." << endl;
			}
}

int converter() {
  	for (int times = 0; times < 32; times++) {
    	int length = text_file[times].size();
    	string current = text_file[times];
    	int start = 0;  //after the : (for defining lines)
    	int end = length;    //before the ; (for defining what a comment is)
    	
    	if (current.at(0) == '#') {
			return 1;  //completed the current set
		}

    	for (int char_check = 0;char_check < length; char_check++) {
    	  	char curr_char = current.at(char_check);
      		if (curr_char == ';') {
				end = char_check+1;
     		} else if (curr_char == ':') {
				start = char_check+1;
     		}
    	}


    	int copy_size = end - start;
    	text_file[times] = current.substr(start,copy_size);

    	//these lines arnt needed but they edit the output of the last line and remove starting and ending white space (it makes it more readable)
    	current = text_file[times];
    	start = current.find_first_not_of(" \t");
    	end = current.find_last_not_of(" \t");
    	copy_size = end - start;
    	text_file[times] = current.substr(start,copy_size);


    	cout << text_file[times] << endl;
	}
  return 1;
}


int main() {
	text_file[0] = "          VAR 0       ; Declare 32-bit variable to fill space ";
	text_file[1] = "START:    LDN NUM01   ; Copy variable to accumulator (negated)";
	text_file[2] = "          SUB NUM02   ; Subtract variable from accumulator to get sum (negated)";
	text_file[3] = "          STO MYSUM   ; Store accumulator to variable - saves answer";
	text_file[4] = "         LDN MYSUM   ; Copy variable to accumulator (negated)";
	text_file[5] = "          STO MYSUM   ; Store accumulator to variable";
	text_file[6] = "	END:      STP         ; Stop processor";
	text_file[7] = "  NUM01:    VAR 1025    ; Declare 32-bit variable";
	text_file[8] = " NUM02:    VAR 621     ; Declare 32-bit variable";
	text_file[9] = " MYSUM:    VAR 0       ; Declare 32-bit variable";
	file_search();
	converter();

}
