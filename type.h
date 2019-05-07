/*Richard Wiedenroth
type.h - Contains an enum for all valid lexeme types in the language*/

#ifndef TYPE
#define TYPE
#pragma once
//Tracking types via enum with an array to map it to a string.
//This allows for both easy comparison AND printing!
enum tokenType {
	//Data types.
	STR,
	INT,
	CHAR,
	//Variable names.
	ID,
	//Operators
	EQ,
	MIN,
	PLUS,
	TIMES,
	DIV,
	MOD,
	AND,
	OR,
	NOT,	
	//Keywords
	LOOP,
	IF,
	ELSE,
	RETURN,
	//Punctuation
	TILDA,
	SEMICOLON,
	OBRACKET,
	CBRACKET,
	OBRACE,
	CBRACE,
	LESSER,
	GREATER, //Also an operator, depending on whether or not there's a preceding '<'
	POUND,
	OPAREN,
	CPAREN,
	DOT,
	//Exit conditions
	END,
	ERR,
	//Meta types.
	JOIN, //Generic structural component.
	VALS, //List of values.
	//Meta types exclusively for parse tree.
	BLOCK,
	FUNC, //Function Declaration
	VAR, //Variable Declaration/
	CALL, //Function Call
	RETVAL, //Returned Value
	MEMVAR, //Member Variable
	//Meta type exlcusively for environments.
	ENV, //Environment
	CLOSURE, //Function definition stored in Environment
	BUILTIN, //Marks built in functions.
	FILEP //Contains a file pointer.
};
#endif