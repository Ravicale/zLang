/*Richard Wiedenroth
The humble standard library for zLang.*/
#ifndef ZTDLIB
#define ZTDLIB
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"
#include "lexeme.h"

lexeme* zPrint(lexeme*);
lexeme* zPrintL(lexeme*);
lexeme* zSize(lexeme*);
lexeme* zOpenF(lexeme*);
lexeme* zGetF(lexeme*);
lexeme* zEoF(lexeme*);
lexeme* zCloseF(lexeme*);

#endif