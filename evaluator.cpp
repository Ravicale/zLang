/*Richard Wiedenroth
The evaluator implementation. Does the stuff.*/
#include "evaluator.h"

evaluator::evaluator(int debugMode, char** argv, int argc) {
	globalScope = new lexeme(ENV);

	if (debugMode) {
		debug = true;
	} else {
		debug = false;
	}

	//Insert builtin functions.
	string bId;
	bId = string("PRINT");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zPrint));
	bId = string("PRINTL");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zPrintL));
	bId = string("SIZE");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zSize));
	bId = string("FOPEN");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zOpenF));
	bId = string("FGET");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zGetF));
	bId = string("FEOF");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zEoF));
	bId = string("FCLOSE");
	globalScope->Insert(new lexeme(ID, bId), new lexeme(zCloseF));

	//Insert each command line argument into global environment.
	if (argc > 2) {
			lexeme* currArgv = NULL;
			lexeme*	currArgn = NULL;
			lexeme* argObject = NULL;
			lexeme* argArray = new lexeme(VALS);
			for (int i = 2; i < argc; i++) {
				argObject = new lexeme(ENV); //Declare new CL object.
				bId = string(argv[i]); //Generate string of command line argument.
				currArgv = EvalString(new lexeme(STR, bId));
				bId = string("arg"); //Insert argument into object.
				currArgn = new lexeme(ID, bId);
				argObject->Insert(currArgn, currArgv);
				argArray->Push(argObject); //Push argument object to array of objects.
			}
			bId = string("CL"); //Push array of arguments to global scope.
			globalScope->Insert(new lexeme(ID, bId), argArray);
	}

}

void evaluator::evaluate(lexeme* parseTree) {
	if (parseTree != NULL && parseTree->type != ERR) {
		Eval(parseTree, globalScope);
	}
	return;
}

lexeme* evaluator::Eval(lexeme* node, lexeme* env) {
	if (node == NULL) {
		cout << "Err: Null passed to eval." << endl;
		exit(EXIT_FAILURE);
	}

	switch(node->type) {
		case INT:
		case CHAR:
		case BUILTIN:
		case FILEP:
			return node;
		case STR:
			return EvalString(node);
		case POUND:
			return EvalFullArray(node->left, env);
		case ID:
			return EvalId(node, env);
		case EQ:
		case MIN:
		case PLUS:
		case TIMES:
		case GREATER:
		case LESSER:
		case DIV:
		case MOD:
		case AND:
		case OR:
		case NOT:
			return EvalOperator(node, env);
		case RETVAL:
			return Eval(node->right, env);
		case VAR:
			return EvalVarDefOrAsn(node, env);
		case FUNC:
			return EvalFuncDef(node, env);
		case CALL:
			return EvalFuncCall(node, env);
		case RETURN:
			return EvalReturn(node, env);
		case IF:
			return EvalConditional(node, env);
		case LOOP:
			return EvalLoop(node, env);
		case BLOCK:
			return EvalBlock(node, env);
		default:
			cout << "Err: Unexpected lexeme evaluated- ";
			node->Display();
			exit(EXIT_FAILURE);
	}
}

lexeme* evaluator::EvalId(lexeme* node, lexeme* env) {
	int index = 0; //Check for index of array.
	if (node->left != NULL) {
		index = Eval(node->left, env)->GetInt();
	}

	lexeme* value = env->Lookup(node, index); //Variable may resolve to object.
	if (node->right != NULL && value->type == ENV) { //Recursive to allow nested objects.
		return EvalId(node->right->left, value);
	} else if (value->type == ERR) {
		cout << "Err: Undeclared variable." << endl;
		exit(EXIT_FAILURE);
	}

	return value;
}

lexeme* evaluator::EvalVarDefOrAsn(lexeme* node, lexeme* env) {
	lexeme* newValues = ArgListToLexeme(node->right, env);
	if (env->Lookup(node->left)->type == ERR) { //Definition.
		return env->Insert(node->left, newValues);	
	}
	//Assignment
	return EvalAssign(node->left, newValues, env);
}

lexeme* evaluator::EvalAssign(lexeme* node, lexeme* newValues, lexeme* env) {
	int index = 0;
	bool hasIndex = false;
	if (node->left != NULL) {
		index = Eval(node->left, env)->GetInt();
		hasIndex = true;
	}

	lexeme* objEnv = env->Lookup(node, index); //Variable may resolve to object.
	if (node->right != NULL && objEnv->type == ENV) { //Recursive to allow nested objects.
		return EvalAssign(node->right->left, newValues, objEnv);
	} else if (hasIndex) {
		return env->Update(node, newValues, index);
	}
	
	return env->Update(node, newValues);
}

lexeme* evaluator::ArgListToLexeme(lexeme* node, lexeme* env) {
	lexeme* values = new lexeme(VALS);
	lexeme* currArg = node;
	while (currArg != NULL) {
		values->Push(Eval(currArg->right, env));
		currArg = currArg->left;
	}
	return values;
}

lexeme* evaluator::EvalFullArray(lexeme* node, lexeme* env) {
	int index = 0; //Check for intex of array.
	if (node->left != NULL) {
		index = Eval(node->left, env)->GetInt();
	}

	lexeme* value = env->Lookup(node, index); //Variable may resolve to object.
	if (node->right != NULL && value->type == ENV) { //Recursive to allow nested objects.
		return EvalFullArray(node->right->left, value);
	} else if (value->type == ERR) {
		cout << "Err: Undeclared variable." << endl;
		exit(EXIT_FAILURE);
	}

	return env->Lookup(node); //Return entire list of values in variable.
}

lexeme* evaluator::EvalOperator(lexeme* node, lexeme* env) {
	lexeme* lhs = Eval(node->left, env);
	lexeme* rhs = Eval(node->right, env);
	if ((lhs->type == INT && rhs->type == INT) || node->type == EQ || node->type == NOT) {
		switch (node->type) {
			case MIN:
				return new lexeme(lhs->GetInt() - rhs->GetInt());
			case PLUS:
				return new lexeme(lhs->GetInt() + rhs->GetInt());
			case TIMES:
				return new lexeme(lhs->GetInt() * rhs->GetInt());
			case DIV:
				return new lexeme(lhs->GetInt() / rhs->GetInt());
			case MOD:
				return new lexeme(lhs->GetInt() % rhs->GetInt());
			case AND:
				return new lexeme(lhs->GetInt() != 0 && rhs->GetInt() != 0);
			case OR:
				return new lexeme(lhs->GetInt() != 0 || rhs->GetInt() != 0);
			case GREATER:
				return new lexeme(lhs->GetInt() > rhs->GetInt());
			case LESSER:
				return new lexeme(lhs->GetInt() < rhs->GetInt());	
			case EQ:
				return CalcEQ(lhs, rhs);
			case NOT:
				return CalcNot(lhs, rhs);
		}
	} else {
		cout << "Err: Attempted to perform mathematical operation with non-int type." << endl;
		exit(EXIT_FAILURE);
	}
	return new lexeme(ERR);
}

lexeme* evaluator::CalcEQ(lexeme* lhs, lexeme* rhs) {
	if (lhs->type == INT && rhs->type == INT) {
		return new lexeme(lhs->GetInt() == rhs->GetInt());
	} else if (lhs->type == CHAR && rhs->type == CHAR) {
		return new lexeme(lhs->GetChar() == rhs->GetChar());
	}
	
	cout << "Err: Attempted to compare mismatched types." << endl;
	exit(EXIT_FAILURE);
}

lexeme* evaluator::CalcNot(lexeme* lhs, lexeme* rhs) {
	if (lhs->type == INT && rhs->type == INT) {
		return new lexeme(lhs->GetInt() != rhs->GetInt());
	} else if (lhs->type == CHAR && rhs->type == CHAR) {
		return new lexeme(lhs->GetChar() != rhs->GetChar());
	}
	
	cout << "Err: Attempted to compare mismatched types." << endl;
	exit(EXIT_FAILURE);
}

lexeme* evaluator::EvalBlock(lexeme* node, lexeme* env) {
	if (debug) {
		env->PrintEnv();
	}
	lexeme* result = NULL;
	lexeme* currNode = node;
	while (currNode != NULL) {
		result = Eval(currNode->left, env);
		currNode = currNode->right;

		if (debug) {
			env->PrintEnv();
		}
	}

	return result;
}

lexeme* evaluator::EvalReturn(lexeme* node, lexeme* env) {
	if (node->left != NULL) {
		return Eval(node->left, env);
	} else if (node->right != NULL) {
		return Eval(node->right, env);
	} else {
		return new lexeme(ERR);
	}
}

lexeme* evaluator::EvalString(lexeme* node) {
	lexeme* charArray = new lexeme(VALS);
	string str = node->GetString();

	int size = str.length();
	for (int i = 0; i < size; ++i) {
		lexeme* character = new lexeme(CHAR);
		character->Push(str[i]);
		charArray->Push(character);
	}
	return charArray;
}

lexeme* evaluator::EvalConditional(lexeme* node, lexeme* env) {
	if (node->left->left == NULL) { //Just eval if 'else' without condition.
		return Eval(node->left->right, env);
	}

	lexeme* value = Eval(node->left->left, env);
	if (value->type != INT) { //If conditional invalid, crash.
		cout << "Err: Invalid conditional value." << endl;
		exit(EXIT_FAILURE);
	} else if (value->GetInt() != 0) { //If conditional true, exec if.
		return Eval(node->left->right, env);
	} else if (node->right != NULL) { //If else exists, check that.
		return EvalConditional(node->right, env);
	} //Otherwise, return nothing.
	return new lexeme(ERR);
}

lexeme* evaluator::EvalLoop(lexeme* node, lexeme* env) {
	lexeme* iter = Eval(node->left, env);
	lexeme* last = NULL;
	
	if (iter == NULL || iter->type == ERR) {
		cout << "Err: Invalid iterator." << endl;
		exit(EXIT_FAILURE);
	}

	while(iter->GetInt() != 0) {
		last = Eval(node->right, env);
		iter = Eval(node->left, env);
	}
	return last;
}

lexeme* evaluator::EvalFuncDef(lexeme* node, lexeme* env) {
	lexeme* container = new lexeme(VALS);
	lexeme* closure = new lexeme(CLOSURE, env, node);
	lexeme* name = node->left;
	if (name != NULL) { //Kludgy stuff to make this work with the dynamic array variables.
		if (env->Lookup(name)->type == ERR) { //No function with this name.
			container->Push(closure);
			return env->Insert(name, container);	
		} else {
			EvalAssign(name, container, env);
		}
	}
	return closure;
}

lexeme* evaluator::EvalFuncCall(lexeme* node, lexeme* env) {
	lexeme* args = node->right;
	lexeme* eargs = EvalArgs(args, env);
	lexeme* closure = Eval(node->left, env);
	if (closure->type == BUILTIN) {
		return closure->builtin(eargs);
	}
	lexeme* params = closure->right->right->left;
	lexeme* body = closure->right->right->right;
	lexeme* senv = closure->left;
	lexeme* eparams = EvalParams(params);
	lexeme* xenv = new lexeme (senv, eparams, eargs);
	string thisString = string("THIS");
	lexeme* thisValue = new lexeme(VALS);
	thisValue->Push(xenv);
	xenv->Insert(new lexeme(ID, thisString), thisValue);
	return Eval(body, xenv); 
}

lexeme* evaluator::EvalParams(lexeme* node) {
	lexeme* vars = NULL;
	lexeme* currParam = node;
	while (currParam != NULL) {
		vars = new lexeme(JOIN, currParam, vars);
		currParam = currParam->left;
	}
	return vars;
}

lexeme* evaluator::EvalArgs(lexeme* node, lexeme* env) {
	lexeme* vals = NULL;
	lexeme* currVal = NULL;
	lexeme* valArr = NULL;
	lexeme* currArg = node;
	while (currArg != NULL) {
		currVal = Eval(currArg->right, env);
		//Send argument to dynamic array so it can be used like variable values in env.
		if (currVal->type == VALS) {
			valArr = currVal;
		} else {
			valArr = new lexeme(VALS);
			valArr->Push(currVal);
		}
		vals = new lexeme(JOIN, valArr, vals);
		currArg = currArg->left;
	}
	return vals;
}