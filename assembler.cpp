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
#include <bitset>

using namespace std;

string text_file[256];
int code_length = -1;

//custom made instruction set for compiling
//first column is the assembly code name
//second is if it needs a integer after it (y/n)
//third is its value in binary for conversion
//var has no value as the line its on gets converted to the number it stores
string instructions[13][3] = {
{"VAR", "y", "NULL"},
{"JMP", "y", "0000"},
{"JRP", "y", "1000"},
{"LDN", "y", "0100"},
{"STO", "y", "1100"},
{"SUB", "y", "0010"},
{"CMP", "n", "0110"},
{"STP", "n", "1110"},
{"ADD", "y", "0001"},
{"MUL", "y", "0101"},
{"DIV", "y", "0011"},
{"MVT", "n", "1111"},
{"MVF", "n", "0111"},
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

		loc = currLine.find_first_not_of("\n \t");

		//if the first character of the line is a ; then the line can be ignored
		if(currLine.at(loc) != (';')){

			//checks there is space in the store before adding
			if(i < 256){
				
				//the first free bucket stores currLine
				text_file[i] = currLine;
				i++;
				cout << "Added to text_file\t" << currLine << endl;
				code_length++;
			}	else{
				cout << "Error: Trying to add to a full store!\n";
				return 1;
			}
		}
	}

	//closes the file input stream
	inFile.close();

	return 0;
}

string decimalToBinary(int decimal, string type)
{
	if (decimal > 2147483647 || decimal < -2147483647)
	{
		cout << "This number is out of range of what 32 bits can store!" << endl;
		return "NULL";
	}

	string binary;
	string temp;

	if (type == "operand")
	{
		temp = bitset<8>(decimal).to_string();
		binary = "00000000";
	} else {
		temp = bitset<32>(decimal).to_string();
		binary = "00000000000000000000000000000000";
	}

	for (unsigned i=0; i<temp.length(); i++)
	{
		if (temp[temp.length() - 1 - i] == '0')
		{
			binary[i] = '0';
		}
		else
		{
			binary[i] = '1';
		}
	}

	return binary;
}

int converter() {

  	for (int times = 0; times < 256; times++) {

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
	}	
	return 0;	
}

string compiler(int var_no) {
	string var_data[var_no][4];
	var_no = 0;

	for (int line = code_length;line >= 0; line--) {
		string current = text_file[line];
		int pos = 0;
		int start = 0;
		size_t found;
		found = current.find(':');
		if (found!=string::npos) {
			pos = (current.find_first_of(':')+1);
		}
		start = current.find_first_not_of(" \t",pos);
		string holder = current.substr(start,3);


		if ((found!=string::npos) && (holder == "VAR")) {

			//adds the variable name to list of variables
			pos = (current.find_first_of(':'));
			var_data[var_no][0] = current.substr(0,pos);


			//changes the pos so it can find the number related to the variable
			pos = current.find(holder);
			pos = (pos + 3);

			//finds the integer related to the variable
			found = current.find_first_not_of("-1234567890 \t",pos);
			string decimal = current.substr(pos,11);
			int value = atoi(decimal.c_str());
			var_data[var_no][1] = decimalToBinary(value,"variable");
			var_data[var_no][2] = decimalToBinary(line,"operand");				
			var_data[var_no][3] = to_string(line);
			var_no++;
		}

		//just initilise the text file line to be 32 bits for conversion 
		text_file[line] = "00000000000000000000000000000000";

		//if its defined as a variable it changes the entire line to the retropective number
		if (holder == "VAR") {
			for (int times = 0; times < var_no;times++) {
				int value = atoi(var_data[times][3].c_str());
				if (line == value) {
					text_file[line] = var_data[times][1];
				}
			}
		} else {
			for (int times = 0; times < 13; times++) {
				if (holder == instructions[times][0]) {
					text_file[line].replace(13,4,instructions[times][2]);

					if (instructions[times][1] == "y") {
						string compare = "";

						pos = current.find(holder);
						pos = (pos + 4);
						pos = current.find_first_not_of(" \t",pos);
						compare = (current.substr(pos,32));
						pos = compare.find_first_of(" \t");
						compare = compare.substr(0,pos);

						for (int search = 0; search < var_no;search++) {
							if (compare==var_data[search][0]) {
								text_file[line].replace(0,8,var_data[search][2]);
							}
						}					
					}
				}
			}
		}
	}
	return "Compile complete";
}

//scans lines searching for errors and memorising variable locations;
string scanner() {

	string error = "";
	int var_no = 0;
	for (int line = code_length;line >= 0; line--) {

		//skips past variable names and such
		int pos = 0;
		size_t found = text_file[line].find(':');
		int start = 0;
		
		string var_name = "";

		if (found!=string::npos) {
			pos = (text_file[line].find_first_of(':')+1);
		}

		bool correct = false;

		//checks that the opcode matches the instruction set
		start = text_file[line].find_first_not_of(" \t",pos);
		string holder = text_file[line].substr(start,3);

		for (int times = 0; times < 13 ; times++) {

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

		if (holder == "VAR") {
			size_t found = text_file[line].find(':');

			if (found!=string::npos) {
				pos = (text_file[line].find_first_of(':'));
			}
			var_name = text_file[line].substr(0,pos);
			var_no++;
		}

		cout << text_file[line] << endl;

		for (int times = 0; times < 13; times++) {
			if (holder == instructions[times][0]) {
				if (instructions[times][1] == "y"){

					//tests to make sure an oprand that needs something after it actually does
					pos = text_file[line].find(holder);
					pos = (pos + 3);

					//test to make sure a variable has a valid number after it
					if (holder == "VAR") {
						found=text_file[line].find_first_not_of("-1234567890 \t",pos);
						if (found!=string::npos) {
							error = "error on line ";
							error += to_string(line); 
							error += " problem with variable number  : ";
							error += text_file[line].substr(pos,32);
							return error;
						}
					}
				}			
			}
		}
	}
	cout << "Scan Complete" << endl;
	error = compiler(var_no);
	return error;

}


/*
string decimalToBinary(int input) {



return output;
}*/

//converts the assembly code into machine code line by line
void convertToBinary(){
}

//writes the final machine code to a text file that can be read by the baby
void writeFile(){

	string fileName = "Out.txt";

	//opens a file output stream for the given file
	ofstream outFile(fileName);
	
	for(int i = 0; i < 256; i++){
		if (text_file[i] == "#") {
			break;
		}
		cout<<text_file[i]<<endl;
		outFile << text_file[i] << "\n";	
	}

	//closes the file output stream
	outFile.close();
}

int main() {

	//sets everyline to # (for seeing how many lines of the program are not needed)
  	for (int times = 0; times < 256; times++) {
  		text_file[times] = "#";
  	}

	string return_error = "";

	string fileName;

	cout << "\nPlease enter the file name of the assembly code file:\n";
	cin >> fileName;

	loadFile(fileName);

	cout << endl;
	cout << "\nFinished loading.\n\n";
	cout << endl;

	converter();

	cout << endl;
	cout << "\nconverting finished.\n";
	cout << endl;

	return_error = scanner();
    
    cout << return_error << endl;
    cout << endl;
	writeFile();
	
	cout << "\nWriting finished.\n";
}

