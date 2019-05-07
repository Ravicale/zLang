/*Richard Wiedenroth
Defines a printer class takes in a parse tree and returns a valid program.*/
#ifndef PRINTER
#define PRINTER
#pragma once
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "type.h"
#include "lexeme.h"

class printer {
	public:
		printer(lexeme*);
		void Print();

	private:
		lexeme* parseTree;
		void PrintBlock(lexeme*, int);
		//Helper functions for specific types of elements within blocks.
		void PrintFunc(lexeme*, int);
		void PrintArgList(lexeme*);
		void PrintParamList(lexeme*);
		void PrintVal(lexeme*);
		void PrintVar(lexeme*);
		void PrintMemVar(lexeme*);
		void PrintArr(lexeme*);
		void PrintRetVal(lexeme*);
		void PrintExpr(lexeme*);
		void PrintIndent(int);
};

#endif
