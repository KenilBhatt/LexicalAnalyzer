/****************************************************************
* FILE NAME:	Kenil_Bhatt_Proj1.cpp
* DESCRIPTION:	Program performs the lexical analysis of a source
*				code that is read from an external input file and
*				prints results to an output file.
* AUTHOR:		Kenil P. Bhatt (kpbhatt@asu.edu)
* SUBJECT:		CSE 340 Priciples of Programming Language
* Project # 1	Class Time: MW 12:00pm	 Date: February 11, 2013
****************************************************************/
// Included Header files.
#include <iostream>		// For I/O to counsel window.
#include <fstream>		// For I/O to external files.
#include <string>		// For string declaration and functions.
#include <cstdlib>		// For access of standard library
#include <cstring>

// Using namespace standard
using namespace std;

// Global variables accessible by all the functions.
char ch;			// Character ch read one-by-one
char outFile[32];	// Character array holding output file name
string tab;			// Holds the tab character.
string token;		// String holds tokens
ofstream fout;		// Output file stream

// Arrays of language token used by source code.
const string compareOps[] = {">", "<", ">=", "<="};
const string groupOps[] = {"{", "}", "[", "]", "{}", "[]"};
const string keywords[] = {"begin", "end", "if", "then", "for", "while", "print", "int", "float"};
const string errCodes[] = {"ERROR CODE 0: Lexical error: invalid character found in token",
							"ERROR CODE 1: Lexical error: token not valid in language"};
const char allchars[] = {'.', '+', '-', '*', '/', '=', '>', '<', '{', '}', '[', ']'};


// Function extracts the name of output file from input file that
// is passed in as a parameter and opens it.
void openOutputFile(char *pInFile) {
	// Local variables accessible only in this function.
	int i;
	string strOut = pInFile, ext = ".out\0";

	// Copy input file name to output without the extension.
	for (i = 0; strOut[i] != '.'; i++)
		outFile[i] = strOut[i];

	// Add ".out" extension to output file name.
	for (int j = 0; j < ext.length(); j++)
		outFile[i++] = ext[j];

	// Open the output file name.
	fout.open(outFile);
}

// Function extracts lexeme from the source code and returns it.
string GetToken(ifstream &pfin) {
	// Temporary string, which will hold extcated lexeme
	string tempStr = "";

	while (!(ch == '\n' || ch == ' ')) {
		tempStr += ch;
		pfin.get(ch);
	}
	
	// Insert right amount of tabs based on string length.
	if (tempStr.length() < 8)
		tab = "\t";
	else 
		tab = "";

	// Return the string.
	return tempStr;
}

// Boolean function returns true if the lexeme is key word, else false.
bool isKeyword(string pTok) {
	// Traverse through array of reservd words.
	for (int i = 0; i < 9; i++) {
		// Return true if key word matches parameter lexeme
		if (pTok == keywords[i])
			return true;
	}

	// Return false if lexeme is not found.
	return false;
}

// Function returns boolean value true if the parameter lexeme is an 
// integer else it return false.
bool isInteger(string pTok) {
	// Traverse through lexeme to check if all characters are digit or not.
    for (int i = 0; i < pTok.length(); i++) {
		if (!isdigit(pTok[i]))
			return false;
	}

    // Return true if all characters are numerical.
    return true;
}

// Function returns boolean value true if the parameter lexeme is an 
// float else it return false.
bool isFloat(string pLex) {
	int dots = 0;	   // Variable to count number of dots in the string.

	for (int i = 0; i < pLex.length(); i++) {
		// Return false if it is invalid character or there are two or 
		// more dots in the string.
		if (!isdigit(pLex[i]) && pLex[i] != '.')
			return false;
		else if (pLex[i] == '.')
			dots++;
		if (dots >= 2)
			return false;
	}

	// Return true if it is flaot.
	return true;
}

// Boolean function returns true if the lexeme is an identifier, else it
// return false.
bool isIdentifier(string pLex) {
	// Check whether each character is either numerical or alphabetical.
	if (isalpha(pLex[0])) {
		for (int i = 0; i < pLex.length(); i++) {
			// Return false if non-valid character is found.
			if (!isalnum(pLex[i]))
				return false;
		}
	} else {
		return false;
	}

	// Return true if the string is valid
	return true;
}

// Function returns true if character is Comparison Operator, else False.
bool isCompOp(string pLex) {
	// Traverse through the list of comparison operator
	for (int i = 0; i < 4; i++){
		// Return true if match is found.
		if (pLex == compareOps[i])
			return true;
	}

	// Return false if no match is found.
	return false;
}

// Function returns true if string is shift operator, else return false.
bool isShiftOp(string pLex) {
	if (pLex == "<<" || pLex == ">>")
		return true;

	return false;
}

// Function returns true if string is grouping operator, else return false.
bool isGroupOp(string pLex) {
	// Traverse through list of grouping operators
	for (int i = 0; i < 6; i++) {
		// Return true if the string is found.
		if (pLex == groupOps[i])
			return true;
	}

	// Return false if no match is found.
	return false;
}

// Function performs error analysis of invalid string and inserts the 
// right token 
void errorAnalysis(string pLex) {
	// Instantiate local var i (index) to zero and boolean var to true.
	int i = 0;
	bool found = true;

	// While the index is less than the length of the string
	while (i < pLex.length()) {
		// Increment index if the char is alphabetical or numerical
		if (isalnum(pLex[i]))
			i++;
		// Else, chekc if the char is valid token language
		else {
			// Local variable j set to 0, and set found to false.
			int j = 0; found = false;

			// While j is less than the size of character array
			while (j < 12) {
				// Set found to true if the character is found and break
				// out of the while loop.
				if (pLex[i] == allchars[j]) {
					found = true;
					break;
				}
				// Increment the j
				j++;
			}
			// After checking all chars if bool is still false, then 
			// print error code 0 for that string.
			if (found == false)
				break;
			// Increment the i
			i++;
		}
	}

	// If the found is true then insert error code 1.
	if (found)
		fout << pLex << '\t' << tab << errCodes[1] << endl;
	// Otherwise insert error code 0.
	else if (found == false)
		fout << pLex << '\t' << tab << errCodes[0] << endl;
}


// Main function will perform lexical analysis by calling other
// 'helper' functions.
int main(int args, char **argv) {

	// Print error message if command line argument is invalid.
	if (args == 1)
		cout << "Usage: " << argv[0] << " <input file-name>\n";
	else {
		// It is assumed that argv[1] will hold input file name.
		ifstream fin(argv[1]);

		// Print error message if file fails to open.
		if (fin.fail()) {
			cout << "Error! Unable to Open the File." << endl;
			exit(1);
		} else 
			openOutputFile(argv[1]);

		// Read character until end of file character is read.
		while (fin.get(ch)) {

			// If the character is digit or alphabetical
			if (isalnum(ch)) {
				// Extract the token from the source code.
				token = GetToken(fin);
				cout << token << endl;

				// Check if the token is keyword, integer, float, or 
				// identifier, else perform error analysis of that token.
				if (isKeyword(token))
					fout << token << '\t' << tab << "Reserved Word" << endl;
				else if (isInteger(token))
					fout << token << '\t' << tab << "Integer" << endl;
				else if (isFloat(token))
					fout << token << '\t' << tab << "Float" << endl;
				else if (isIdentifier(token))
					fout << token << '\t' << tab << "Identifier" << endl;
				else 
					errorAnalysis(token);
			}

			// If the character is mathematical operator.
			else if (strchr(".+-*/=", ch)) {
				// Extract the lexeme from the code.
				token = GetToken(fin);
				cout << token << endl;

				// If length of the token is 1
				if (token.length() == 1)
					fout << token << '\t' << tab << "Math Operator" << endl;
				// Check whether the string is float or not if the first
				// character is dot.
				else if (token[0] == '.') {
					if (isFloat(token))
						fout << token << '\t' << tab << "Float" << endl;
				}
				// Otherwise perform error analysis on the stirng.
				else
					errorAnalysis(token);
			}

			// If the character is '<' or '<' 
			else if (strchr("><", ch)) {
				// Extract the lexeme from the code.
				token = GetToken(fin);
				cout << token << endl;

				// If the character is comparison operator.
				if (isCompOp(token))
					fout << token << '\t' << tab << "Comparison Operator" << endl;
				// If the string is shift operator. 
				else if (isShiftOp(token))
					fout << token << '\t' << tab << "Shift Operator" << endl;
				else 
					errorAnalysis(token);
			}

			// If the character is grouping operator.
			else if (strchr("{}[]", ch)) {
				// Extract the lexeme from the code.
				token = GetToken(fin);
				cout << token << endl;

				// If the string is grouping operator
				if (isGroupOp(token))
					fout << token << '\t' << tab << "Grouping Operator" << endl;
				else 
					errorAnalysis(token);
			}

			// Perform error analysis on in valid lexeme.
			else if (!(ch == '\0' || ch == '\n' || ch == ' ' || ch == '\t')) {
				token = GetToken(fin);
				cout << token << endl;

				errorAnalysis(token);
			}
		}

		// Close input and output files.
		fin.close();
		fout.close();
	}
}
