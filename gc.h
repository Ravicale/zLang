/*Richard Wiedenroth
Contains functions and data structures to handle garbage collection in Z.*/

#ifndef GC
#define GC
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "lexeme.h"

using namespace std;

class gc {
	public:
		gc();
		void Alloc(lexeme*); //Insert lexeme into temporary list.
		void Protect(lexeme*); //Insert lexeme into permanent list.
		void Dealloc(); //Perform garbage collection.

	private:
		vector<lexeme*> permanent; //List of permanent lexemes (IE: head of parse tree) from which tracing begins.
		vector<lexeme*> allocated; //List of lexemes that might not exist forever.
};

#endif
