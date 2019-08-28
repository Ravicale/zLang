/*Richard Wiedenroth
Lexeme class, the building block of everything in the language*/

#ifndef LEXEME
#define LEXEME
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "type.h"

using namespace std;

class lexeme {
	public:
		lexeme(); //Create an int with all values NULL.
		lexeme(tokenType);
		lexeme(tokenType, string&);
		lexeme(int); //Easy integer constructor.
		lexeme(lexeme* (*)(lexeme*)); //Constructor for Built In functions.
		lexeme(FILE*); //Constructor for file pointers.
		lexeme(tokenType, lexeme*, lexeme*); //'cons' equivelent, for use in trees/lists.
		lexeme(lexeme*, lexeme*, lexeme*); //Primarily used to implement environment extension.
		
		//Display functions.
		void InDisplay(); //Displays lexeme info.
		void Display(); //Same as above, but with a newline at the end.
		void DisplayTree(int); //Displays all lexemes in the tree.

		//Environment implementation:
		void PrintEnv();
		lexeme* Lookup(lexeme*); //Checks if variable exists in environment. Returns error otherwise.
		lexeme* Lookup(lexeme*, int);
		lexeme* Update(lexeme*, lexeme*); //Updates variable in environment.
		lexeme* Update(lexeme*, lexeme*, int); //Updates only one index of variable in env.
		lexeme* Insert(lexeme*, lexeme*); //Insert new value to environment.

		//Functions for getting values.
		string GetString();
		lexeme* GetValue();
		lexeme* GetValue(unsigned int);
		vector<lexeme*> GetValues();
		int GetInt();
		int GetInt(int);
		char GetChar();
		char GetChar(int);
		int GetSize(); //Returns size of vector.
		FILE* GetFile();

		//Functions for setting/inserting private values.
		void Push(int);
		void Push(char);
		void Push(string&);
		void Push(lexeme*);

		tokenType type;
		lexeme* left; //'car'
		lexeme* right; //'cdr'
		lexeme* (*builtin)(lexeme*); //Holds built in function pointers.
		FILE* file; //Pointer to file.

	private:
		bool Compare(lexeme&); //Compares lexemes.
		lexeme* SetValue(lexeme* val, int index); //Sets value of lexeme at index to first value in passed in lexeme.

		vector<lexeme*> value;
		int intValue;
		string strValue;
};

#endif
