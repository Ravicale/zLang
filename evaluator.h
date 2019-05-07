/*Richard Wiedenroth
Evaluator for the language. Does the cool code executiony stuff*/
#ifndef EVALUATOR
#define EVALUATOR
#pragma once
#pragma GCC diagnostic ignored "-Wswitch"
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <stdlib.h>
#include "type.h"
#include "lexeme.h"
#include "ztdLib.h"

class evaluator {
	public:
		evaluator(int, char**, int);
		void evaluate(lexeme*);

	private:
		lexeme* globalScope; //Top level environment.
		bool debug; //Whether or not to print the current environment in Eval() at every step.
		lexeme* Eval(lexeme*, lexeme*); //Primary dispatch function.
		lexeme* EvalId(lexeme*, lexeme*); //Evaluate an ID to a value
		lexeme* EvalOperator(lexeme*, lexeme*); //Secondary dispatch function for operators.
			lexeme* CalcEQ(lexeme*, lexeme*);
			lexeme* CalcNot(lexeme*, lexeme*);
		lexeme* EvalBlock(lexeme*, lexeme*);
		lexeme* EvalReturn(lexeme*, lexeme*);
		lexeme* EvalVarDefOrAsn(lexeme*, lexeme*);
			lexeme* EvalAssign(lexeme*, lexeme*, lexeme*);
			lexeme* ArgListToLexeme(lexeme*, lexeme*);
		lexeme* EvalFullArray(lexeme*, lexeme*);
		lexeme* EvalString(lexeme*); //Split string up into lexeme with array of characters.
		lexeme* EvalFuncDef(lexeme*, lexeme*);
		lexeme* EvalFuncCall(lexeme*, lexeme*);
			//Stuff to put function call stuff in an environment friendly format.
			lexeme* EvalParams(lexeme*); 
			lexeme* EvalArgs(lexeme*, lexeme*);
		lexeme* EvalConditional(lexeme*, lexeme*);
		lexeme* EvalLoop(lexeme*, lexeme*);
};

#endif
