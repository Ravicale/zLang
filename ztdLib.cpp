/*Richard Wiedenroth
The humble standard library for zLang.*/
#include "ztdLib.h"

lexeme* zPrint (lexeme* argList) {
	if (argList != NULL) {
		zPrint(argList->right);
		lexeme* curr = NULL;
		vector<lexeme*> arrayVals = argList->left->GetValues();
		int size = arrayVals.size();
		for (int i = 0; i < size; i++) {
			curr = arrayVals[i];
			switch(curr->type) {
				case INT:
					cout << curr->GetInt() << " ";
					break;
				case CHAR:
					cout << curr->GetChar();
					break;
				default:
					curr->InDisplay();
					break;
			}
		}
	}
	return argList;
}

lexeme* zPrintL (lexeme* argList) {
	lexeme* retVal = zPrint(argList);
	cout << endl;
	return retVal;
}

lexeme* zSize (lexeme* argList) {
	lexeme* curr = NULL;
	if (argList != NULL) {
		lexeme* prev = zSize(argList->right);
		curr = new lexeme(VALS);
		curr->Push(prev);
		curr->Push(new lexeme(argList->left->GetSize()));
	}
	return curr;
}

lexeme* zOpenF (lexeme* argList) {
	lexeme* curr = NULL;
	if (argList != NULL) {
		lexeme* prev = zOpenF(argList->right);
		string fileName = string();
		for (int i = 0; i < argList->left->GetSize(); i++) {
			lexeme* currChar = argList->left->GetValue(i);
			fileName += currChar->GetChar();
		}
		curr = new lexeme(VALS);
		curr->Push(prev);
		FILE *fp = fopen(fileName.c_str(), "r");
		if (fp != NULL) {
			curr->Push(new lexeme(fp));
		} else {
			cout << "Warning: Failed to open file." << endl;
		}
	}
	return curr;
}

lexeme* zGetF (lexeme* argList) {
	if (argList != NULL) {
		lexeme* fp = argList->left->GetValue();
		if (fp->type == FILEP && !feof(fp->GetFile())) {
			int i = 0;
			fscanf(fp->GetFile(), "%d", &i);
			return new lexeme(i);
		} else {
			cout << "Warning: Attempted reading past end of file." << endl;
		}
	}
	return new lexeme(ERR);
}

lexeme* zEoF (lexeme* argList) {
	if (argList != NULL) {
		lexeme* fp = argList->left->GetValue();
		if (fp->type == FILEP) {
			if (!feof(fp->GetFile())) {
				return new lexeme(1);
			}
		}
	}
	return new lexeme(0);
}

lexeme* zCloseF (lexeme* argList) {
	if (argList != NULL) {
		lexeme* fp = argList->left->GetValue();
		if (fp->type == FILEP) {
			if (!fclose(fp->GetFile())) {
				return new lexeme(1);
			}
		}
	}
	return new lexeme(0);
}