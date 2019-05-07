/*Richard Wiedenroth
Main file for lexical analyzer.*/

#include <iostream>
#include <fstream>
#include <cctype>
#include "type.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "printer.h"
#include "evaluator.h"

#define DEBUG_PARSE 0
#define DEBUG_EVAL 0

using namespace std;

int main (int argc, char* argv[]); //Main function.