/*Richard Wiedenroth
Defines the lexer class which parses an input file to verify all tokens are valid.*/
#ifndef LEXER
#define LEXER
#pragma once
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "type.h"
#include "lexeme.h"

using namespace std;

class lexer {
	public:
		lexer(ifstream&, bool); //Constructor
		bool Scan(); //Performs lexical analysis and returns whether or not all items are recognized.
		lexeme* Lex(); //Main lex function, categorizes characters to the right type.
		void Reset(); //Resets file pointer and lineCount to beginning.
		int GetLine(); //Returns current lineCount;

	private:
		ifstream& prog; //Filestream for program.
		int lineCount; //Number of lines read.
		bool debug; //If set to true, will print additional debug information.
		
		//Lexing functions.
		void SkipWhiteSpace(); //Skips whitespace and comments to next character.
		lexeme* LexChar(); //Lexeme generators for types with values.
		lexeme* LexInt();
		lexeme* LexStr();
		lexeme* LexVarOrWord();
};
#endif
