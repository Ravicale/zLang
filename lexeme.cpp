	/*Richard Wiedenroth
Implementation file for lexeme.h*/

#include "lexeme.h"

//Values to print for display function. Index corresponds to strValue of tokenType.
const char* typeStr[] = {
	"STRING",
	"INTEGER",
	"CHARACTER",
	"ID",
	"EQUALS",
	"MINUS",
	"PLUS",
	"TIMES",
	"DIVIDES",
	"MODULO",
	"AND",
	"OR",
	"NOT",
	"LOOP",
	"IF",
	"ELSE",
	"RETURN",
	"TILDA",
	"SEMICOLON",
	"OPENING_BRACKET",
	"CLOSING_BRACKET",
	"OPENING_BRACE",
	"CLOSING_BRACE",
	"LESS_THAN",
	"GREATER_THAN",
	"POUND",
	"OPENING_PARENTHESIS",
	"CLOSING_PARENTHESIS",
	"DOT",
	"END_OF_FILE",
	"ERROR",
	"JOIN",
	"VALS",
	"BLOCK",
	"FUNC",
	"VAR",
	"CALL",
	"RETVAL",
	"MEMVAR",
	"|Environment|",
	"CLOSURE",
	"BUILTIN",
	"FILE"
};

lexeme::lexeme() {
	type = INT;
	left = NULL;
	right = NULL;
	builtin = NULL;
	intValue = 0;
	file = NULL;
}

lexeme::lexeme(tokenType t) {
	type = t;
	left = NULL;
	if (type == ENV) {
		right = new lexeme(VAR);
	} else {
		right = NULL;
	}
	builtin = NULL;
	intValue = 0;
	file = NULL;
}

lexeme::lexeme(lexeme* (*func)(lexeme*)) {
	type = BUILTIN;
	builtin = func;
	left = NULL;
	right = NULL;
	intValue = 0;
	file = NULL;
}

lexeme::lexeme(FILE* fp) {
	type = FILEP;
	builtin = NULL;
	left = NULL;
	right = NULL;
	intValue = 0;
	file = fp;
}

lexeme::lexeme(int val) {
	type = INT;
	strValue = to_string(val);
	intValue = val;
	left = NULL;
	right = NULL;
	builtin = NULL;
	file = NULL;
}

lexeme::lexeme(tokenType t, string& val) :strValue(val){
	type = t;
	intValue = 0;
	if (t == INT) {
		intValue = stoi(val);
	}
	left = NULL;
	right = NULL;
	builtin = NULL;
	file = NULL;
}

lexeme::lexeme(tokenType t, lexeme* l, lexeme* r) {
	type = t;
	left = l;
	right = r;
	builtin = NULL;
	intValue = 0;
	file = NULL;
}

lexeme::lexeme(lexeme* env, lexeme* variables, lexeme* vals) {
	type = ENV;
	left = variables;
	right = new lexeme(VAR, vals, env);
	builtin = NULL;
	intValue = 0;
	file = NULL;
}

void lexeme::Display() {
	InDisplay();
	if (type != JOIN) {
		cout << endl;
	}
	return;
}

void lexeme::InDisplay() {
	if (type == JOIN) {
		return;
	}
	cout << typeStr[type]; //Uses type enum to access correct string.
	if (type == INT || type == CHAR || type == STR || type == ID) { //In case lexeme has some extra strValue.
		cout << " " << strValue;
	} else if (type == VALS) {
		cout << " [";
		int size = value.size();
		for (int i = 0; i < size; i++) {
			value[i]->InDisplay();
			cout << " ";
		}
		cout << "]";
	}
	return;
}

void lexeme::PrintEnv() {
	cout << "/-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-/" << endl;
	lexeme* head = this;
	int indent = 0;
	lexeme* var = left;
	lexeme* val = NULL;
	while (head != NULL) {
		var = head->left;
		while (var != NULL) {
			for (int i = 0; i < indent; i++) {
				cout << "    ";
			}
			var->left->Display();
			var = var->right;
		}
		head = head->right->right;
		indent++;
	}
	head = this;
	indent = 0;
	while (head != NULL) {
		val = head->right->left;
		while (val != NULL) {
			for (int i = 0; i < indent; i++) {
				cout << "    ";
			}
			val->left->Display();
			val = val->right;
		}
		head = head->right->right;
		indent++;
	}
	cout << "/-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-/" << endl;
}

lexeme* lexeme::Lookup(lexeme* variable) {
	lexeme* head = this;
	lexeme* var = left;
	lexeme* val = NULL;

	while (head != NULL) {
		var = head->left;
		val = head->right->left;
		while (var != NULL && val != NULL) {
			if (variable->Compare(*var->left)) {
				return val->left;
			}
			var = var->right;
			val = val->right;
		}
		head = head->right->right;
	}

	return new lexeme(ERR);
}

lexeme* lexeme::Lookup(lexeme* variable, int index) {
	lexeme* head = this;
	lexeme* var = left;
	lexeme* val = NULL;
	while (head != NULL) {
		var = head->left;
		val = head->right->left;
		while (var != NULL && val != NULL) {
			if (variable->Compare(*var->left)) {
				return val->left->GetValue(index);
			}
			var = var->right;
			val = val->right;
		}
		head = head->right->right;
	}

	return new lexeme(ERR);
}

lexeme* lexeme::Update(lexeme* variable, lexeme* v) {
	lexeme* head = this;
	lexeme* var = NULL;
	lexeme* val = NULL;

	while (head != NULL) {
		var = head->left;
		val = head->right->left;
		while (var != NULL && val != NULL) {
			if (variable->Compare(*var->left)) {
				val->left = v;
				return val->left;
			}
			var = var->right;
			val = val->right;
		}
		head = head->right->right;
	}

	return new lexeme(ERR);
}

lexeme* lexeme::Update(lexeme* variable, lexeme* v, int index) {
	lexeme* head = this;
	lexeme* var = NULL;
	lexeme* val = NULL;

	while (head != NULL) {
		var = head->left;
		val = head->right->left;
		while (var != NULL && val != NULL) {
			if (variable->Compare(*var->left)) {
				val->left->SetValue(v, index);
				return val->left;
			}
			var = var->right;
			val = val->right;
		}
		head = head->right->right;
	}

	return new lexeme(ERR);
}

lexeme* lexeme::Insert(lexeme* variable, lexeme* v) {
	lexeme* vals = right;

	left = new lexeme(JOIN, variable, left);
	vals->left = new lexeme(JOIN, v, vals->left);
	return v;
}

string lexeme::GetString() {
	return strValue;
}

char lexeme::GetChar() {
	return GetChar(0);
}

char lexeme::GetChar(int i) {
	return strValue[i];
}

int lexeme::GetInt() {
	return intValue;
}

lexeme* lexeme::GetValue() {
	return GetValue(0);
}

lexeme* lexeme::GetValue(unsigned int index) {
	if (index > value.size()) {
		cout << "ERR: Index out of range." << endl;
		exit(EXIT_FAILURE);
	} else if (type == BUILTIN) {
		return this;
	}

	return value[index];
}

vector<lexeme*> lexeme::GetValues() {
	return value;
}

lexeme* lexeme::SetValue(lexeme* val, int index) {
	return value[index] = val->GetValue();
}

int lexeme::GetSize() {
	return value.size();
}

FILE* lexeme::GetFile() {
	return file;
}

void lexeme::Push(int val) {
	intValue = val;
	strValue = to_string(val);
	return;
}

void lexeme::Push(char val) {
	strValue += val;
	return;
}	

void lexeme::Push(string& val) {
	strValue += val;
	return;
}

void lexeme::Push(lexeme* val) {
	if (val != NULL) {
		if (val->type == VALS) {
			vector<lexeme*> lexValList = val->GetValues();
			int size = val->GetSize();
			for (int i = 0; i < size; i++) {
				value.push_back(lexValList[i]);
			}
		} else {
			value.push_back(val);
		}
	}
	return;
}

bool lexeme::Compare(lexeme& other) {
	if (!strValue.compare(other.GetString())) {
		return true;
	}
	return false;
}