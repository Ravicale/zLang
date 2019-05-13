/*Richard Wiedenroth
Defines the parser class which parses an input file to verify correctness and generate parse tree.*/
#ifndef PARSER
#define PARSER
#pragma once
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "type.h"
#include "lexeme.h"
#include "lexer.h"

using namespace std;

class parser {
	public:
		parser(lexer&, bool); //Constructor
		bool Scan(); //Performs semantic analysis and returns whether or not the input is valid grammar.
		void PrintTree(); //Uses PrintTreeR to recursively output pre-order of environment tree.
		lexeme* parseTree; //Pointer to root of Parse Tree.

	private:
		lexer lxr; //Lexer used to collect lexemes from program.
		lexeme currLexeme; //Current lexeme being processed.
		lexeme errLexeme; //Lexeme that caused error (if any).
		bool debug; //If set to true, will print additional debug information.
		bool syntaxError; //If semantic analysis uncovers an error.
		int errLine; //Line first error appeared on (if any).

		//Semantics functions.
		bool Check(tokenType); //Checks if currLexeme is of the same type as t.
		lexeme* Match(tokenType); //Checks if currLexeme is valid then advances.
		void Advance(); //Calls Lex() and updates currLexeme.
		void ReportError(); //Collects/Prints error information.

		//Functions for recursive descent.
		void BlockInit(); void Block(lexeme*); bool BlockPending(); //BlockInit() used for initial block call to initialize tree.
		void Loop(lexeme*);
		void Conditional(lexeme*);
		void Else(lexeme*); bool ElsePending();
		void Statement(lexeme*); bool StatementPending();
		void Return(lexeme*);
		void Definition(lexeme*);
		void Function(lexeme*);
		//IDKnown is used to fix a case of ambiguity regarding ReturnedValues without making the syntax (more) unwieldy.
		void Call(lexeme*); void CallIDKnown(lexeme*, lexeme*);
		void ParamList(lexeme*);
		void Value(lexeme*); bool ValuePending();
		void ReturnedValue(lexeme*);
		void Expression(lexeme*, lexeme*);
		void Operator(lexeme*); bool OperatorPending();
		void Variable(lexeme*);
		void Index(lexeme*); //Implicitly optional for convenience.
		void MemberVar(lexeme*); //Implicitly optional for convenience. 
		void ArgList(lexeme*, bool); bool ArgListPending();
		void FullArray(lexeme*); bool FullArrayPending();

		//Misc functions.
		void PrintTreeR(lexeme*); //Helper for PrintTree();
};
#endif