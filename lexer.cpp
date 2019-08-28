/*Richard Wiedenroth
Implementation file for lexer.h*/
#include "lexer.h"

lexer::lexer(ifstream& program, bool dbg) :prog(program) {
	lineCount = 1;
	debug = dbg;
}

bool lexer::Scan() {
	lexeme* currLexeme = Lex();
	while (currLexeme->type != END) { //Where the majority of the action happens.
		if (currLexeme->type == ERR) {
			return false; //Error code if lex runs into issue.
		}
		if (debug) {
			currLexeme->Display();
		}
		currLexeme = Lex();
	}
	if (debug) {
		currLexeme->Display(); //Required to display EOF.	
	}
	Reset();
	return true;
}

void lexer::Reset() {
	prog.clear();
	prog.seekg(0, ios::beg); //Return file stream to start.
	lineCount = 1; //Reset linecount.
	return;
}

int lexer::GetLine() {
	return lineCount;
}

lexeme* lexer::Lex() {
	SkipWhiteSpace();
	char ch;
	prog.get(ch);
	if (prog.fail()) { //eof() doesn't seem to work in this case, but fail() does. Will likely ask about during office hours.
		return new lexeme(END);
	}

	switch(ch) {
		case '=':
			return new lexeme(EQ);
		case '+':
			return new lexeme(PLUS);
		case '*':
			return new lexeme(TIMES);
		case '/':
			return new lexeme(DIV);		
		case '%':
			return new lexeme(MOD);
		case '&':
			return new lexeme(AND);
		case '|':
			return new lexeme(OR);
		case '!':
			return new lexeme(NOT);
		case '~':
			return new lexeme(TILDA);
		case ';':
			return new lexeme(SEMICOLON);
		case '[':
			return new lexeme(OBRACKET);
		case ']':
			return new lexeme(CBRACKET);
		case '{':
			return new lexeme(OBRACE);
		case '}':
			return new lexeme(CBRACE);
		case '<': 
			return new lexeme(LESSER);
		case '>':
			return new lexeme(GREATER);
		case '#':
			return new lexeme(POUND);
		case '(':
			return new lexeme(OPAREN);
		case ')':
			return new lexeme(CPAREN);
		case '.':
			return new lexeme(DOT);
		default:
			if (ch == '"') {
				return LexStr();
			} else if (isdigit(ch) || ch == '-') {
				prog.unget();
				return LexInt();
			} else if (ch == '\'') {
				return LexChar();
			} else if (isalpha(ch)) {
				prog.unget();
				return LexVarOrWord();
			}
	}

	cout << "Error on line: " << lineCount << " | Invalid item '" << ch << "'" << endl;
	return new lexeme(ERR);
}

void lexer::SkipWhiteSpace () {
	char item;
	bool inComment = false;
	prog.get(item);
	while (!prog.eof() && (isspace(item) || inComment || item == '\\')) {
		if (item == '\\' && inComment == false) { //Lets me skip over comments, even on multiple lines.
			inComment = true;
		} else if (item == '\\') {
			inComment = false;
		}
		if (item == '\n') {
			lineCount++;
		}
		prog.get(item);	
	}
	prog.unget();
	return;	
}

lexeme* lexer::LexChar() {
	char ch;
	string str;
	prog.get(ch);
	if (!prog.eof() && ch != '\\' && ch != '\'') { //Covers the majority of characters.
		str.push_back(ch);
		prog.get(ch);
		if (!prog.eof() && !prog.fail() && ch == '\'') {
			return new lexeme(CHAR, str);
		}
	} else if (!prog.eof() && ch == '\\') { //In case the character is something like a newline.
		str.push_back(ch);
		prog.get(ch);
		if (!prog.eof() && !prog.fail()) {
			str.push_back(ch);
			prog.get(ch);
			if (!prog.eof() && !prog.fail() && ch == '\'') {
				return new lexeme(CHAR, str);
			}
		}
	}

	if (prog.eof()) {
		cout << "Error on line: " << lineCount << " | Unexpected end of file." << endl;
	} else {
		cout << "Error on line: " << lineCount << " | Invalid \"character\": " << endl;
	}
	return new lexeme(ERR);
}

lexeme* lexer::LexInt() {
	char ch;
	string str;
	prog.get(ch);

	if (ch == '-') {
		str.push_back(ch);
		prog.get(ch);
		if (prog.eof() || !isdigit(ch)) {
			prog.unget();
			return new lexeme(MIN);
		}
	}

	while (!prog.eof() && isdigit(ch)) {
		str.push_back(ch);
		prog.get(ch);
	}
	prog.unget();

	return new lexeme(INT, str);
}

lexeme* lexer::LexStr() {
	char ch;
	string str;
	prog.get(ch);
	while (!prog.eof() && ch != '"') {
		if (ch == '\n') {
			lineCount++;
		}
		str.push_back(ch);
		prog.get(ch);
	}

	if (prog.eof()) {
		cout << "Error on line: " << lineCount << " | Unexpected end of file." << endl;
		return new lexeme(ERR);
	}

	return new lexeme(STR, str);
}

lexeme* lexer::LexVarOrWord() {
	char ch;
	string str;
	prog.get(ch);
	while (!prog.eof() && (isalnum(ch) || ch == '_')) {
		str.push_back(ch);
		prog.get(ch);
	}
	prog.unget();

	if (!str.compare("LOOP")) { //Keyword handling.
		return new lexeme(LOOP);
	} else if (!str.compare("IF")) {
		return new lexeme(IF);
	} else if (!str.compare("ELSE")) {
		return new lexeme(ELSE);
	} else if (!str.compare("RETURN")) {
		return new lexeme(RETURN);
	}

	return new lexeme(ID, str);
}
