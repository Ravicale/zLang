#include "printer.h"

printer::printer(lexeme* t) {
	parseTree = t;
}

void printer::Print() {
	cout << "\\Output:\\";
	PrintBlock(parseTree, 0);
	cout << endl;
	return;
}

void printer::PrintBlock(lexeme* block, int indent) {
	while (block != NULL) {
		PrintIndent(indent);

		lexeme* content = block->left;
		block = block->right;
		if (content != NULL) {
			switch(content->type) {
			case VAR:
				cout << "=";
			case CALL:
				if (content->left != NULL) {
					PrintVar(content->left);
					PrintMemVar(content->left);
				} else {
					cout << "~";
					PrintFunc(content->left, indent);
				}
				
				PrintArgList(content->right);
				cout << ";";
				break;
			case FUNC:
				cout << "==";
				PrintVar(content->left);
				PrintFunc(content, indent);
				cout << ";" << endl;
				break;
			case RETURN:
				cout << "RETURN";
				if (content->left != NULL) {
					if (content->left->type == FUNC) {
						cout << " ~";
						PrintFunc(content->left, indent);
					} else {
						PrintVal(content->left);
					}
				} else if (content->right != NULL) {
					PrintArr(content->right);
				}
				cout << ";";
				break;
			case IF: { //Explicitly defining scope because g++ is too dumb to realize that I'm initializing checkElse.
				cout << "IF";
				PrintVal(content->left->left);
				cout << " {";
				PrintBlock(content->left->right, indent + 1);
				PrintIndent(indent);
				cout << "}";
				lexeme* checkElse = content->right;
				while (checkElse != NULL) {
					cout << " ELSE";
					PrintVal(checkElse->left->left);
					cout << " {";
					PrintBlock(checkElse->left->right, indent + 1);
					PrintIndent(indent);
					cout << "}";
					checkElse = checkElse->right;
				}
				break; }
			case LOOP:
				cout << "LOOP";
				PrintVal(content->left);
				cout << " {";
				PrintBlock(content->right, indent + 1);
				PrintIndent(indent);
				cout << "}";
				break;
			default:
				cerr << "ERROR: Unexpected symbol in parse tree." << endl;
				return;
			}
		}
	}
	return;
}

void printer::PrintFunc(lexeme* currNode, int indent) {
	PrintParamList(currNode->right->left);
	cout << " {";
	PrintBlock(currNode->right->right, indent + 1);
	PrintIndent(indent);
	cout << "}";
}

void printer::PrintArgList(lexeme* currNode) {
	while (currNode != NULL) {
		if (currNode->right->type == POUND || currNode->right->type == STR) {
			PrintArr(currNode->right);
		} else {
			PrintVal(currNode->right);
			PrintMemVar(currNode->right);
		}
		currNode = currNode->left;
	}
	return;
}

void printer::PrintParamList(lexeme* currNode) {
	while (currNode != NULL) {
		cout << " " << currNode->GetString();
		currNode = currNode->left;
	}
	return;
}

void printer::PrintVal(lexeme* currNode) {
	if (currNode != NULL) {
		switch (currNode->type) {
		case INT:
			cout << " " << currNode->GetString();
			break;
		case CHAR:
			cout << " '" << currNode->GetString() << "'";
			break;
		case ID:
			cout << " ";
			PrintVar(currNode);
			break;
		case RETVAL:
			PrintRetVal(currNode);
			break;
		case FUNC:
			cout << " ~";
			PrintFunc(currNode, 0);
		default:
			return;
		}
	}
	return;
}

void printer::PrintVar(lexeme* currNode) {
	cout << currNode->GetString();
	if (currNode->left != NULL) {
		cout << "[" << currNode->left->GetString() << "]";
	}
	return;
}

void printer::PrintMemVar(lexeme* currNode) {
	if (currNode->right != NULL && currNode->right->type == MEMVAR) {
		currNode = currNode->right;
		cout << ".";
		PrintVar(currNode->left);
		PrintMemVar(currNode->left);
	}
	return;
}

void printer::PrintArr(lexeme* currNode) {
	if (currNode->type == STR) {
		cout << " \"" << currNode->GetString() << "\"";
	} else if (currNode->type == POUND) {
		cout << " #";
		PrintVar(currNode->left);
		PrintMemVar(currNode->left);
	}
	return;
}

void printer::PrintRetVal(lexeme* currNode) {
	cout << " (";
	currNode = currNode->right;
	if (currNode->type == CALL) {
		if (currNode->left->type == FUNC) {
			cout << "~";
			PrintFunc(currNode->left, 0);
		} else {
			PrintVar(currNode->left);
			PrintMemVar(currNode->left);
		}
		PrintArgList(currNode->right);
	} else {
		PrintExpr(currNode);
	}
	cout << ")";

	return;
}

void printer::PrintExpr(lexeme* currNode) {
	if (currNode != NULL) {
		if (currNode->type != ID) {
			PrintExpr(currNode->left);
		}
		switch (currNode->type) {
			case EQ:
				cout << " =";
				break;
			case PLUS:
				cout << " +";
				break;
			case MIN:
				cout << " -";
				break;
			case TIMES:
				cout << " *";
				break;
	        case DIV:
	        	cout << " /";
	        	break;
	        case MOD:
	        	cout << " %";
	        	break;
	        case OR:
	        	cout << " |";
	        	break;
	        case NOT:
	        	cout << " !=";
	        	break;
	        case GREATER:
	        	cout << " >";
	        	break;
	        case LESSER:
	        	cout << " <";
	        	break;
	       	default:
	       		PrintVal(currNode);
	       		break;
	    }
	   	PrintVal(currNode->right);
	}
	
	return;
}

void printer::PrintIndent(int indent) {
	cout << endl;
	for (int i = 0; i < indent; i++) {
		cout << "    ";
	}
}