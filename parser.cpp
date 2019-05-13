/*Richard Wiedenroth
Implementation file for parser.h*/
#include "parser.h"

parser::parser(lexer& lexer, bool dbg) :lxr(lexer) {
    lexeme currLexeme();
    lexeme errLexeme();
    parseTree = NULL;
    debug = dbg;
    syntaxError = false;
    errLine = 0;
}

bool parser::Scan () {
    Advance();
    BlockInit();

    if (syntaxError) {
        cout << "Err found on line: " << errLine << endl;
        cout << "Unexpected Token:  ";
        errLexeme.Display();
    } else if (debug) {
        cout << "legal" << endl;
    }

    return !syntaxError;
}


void parser::Advance() {
    currLexeme = lxr.Lex();
    return;
}

void parser::ReportError() {
    if (syntaxError != true) {
        syntaxError = true;
        errLine = lxr.GetLine();
        errLexeme = currLexeme;
    }
    return;
}

bool parser::Check(tokenType t) {
    if (currLexeme.type == t) {
        return 1;
    }
    return 0;
}

lexeme* parser::Match(tokenType t) {
    lexeme* retLexeme = NULL;
    if (currLexeme.type == t) {
        if (currLexeme.type == ID || currLexeme.type == INT || currLexeme.type == CHAR || currLexeme.type == STR) {
            string currVal = string(currLexeme.GetString());
            retLexeme = new lexeme(currLexeme.type, currVal);
        } else {
            retLexeme = new lexeme(t);
        }
        Advance();
    } else {
        ReportError();
        retLexeme = new lexeme(ERR);
    }

    return retLexeme; //Replace with copy-constructor later
}

bool parser::BlockPending() {
    return StatementPending() || Check(LOOP) || Check(IF);
}

void parser::BlockInit() {
    parseTree = new lexeme(BLOCK);
    if (StatementPending()) {
        Statement(parseTree);
    } else if (Check(LOOP)) {
        Loop(parseTree);
    } else if (Check(IF)) {
        Conditional(parseTree);
    }
    
    if (BlockPending()) {
        Block(parseTree);
    } else if (!Check(CBRACE) && !Check(END)) {
        ReportError(); //Special case error call that might not otherwise be covered by match.
    }

    return;
}

/*
    [currNode]
            \
            [+Block]
           /       \
    [+???]        [?BLOCK]
*/
void parser::Block(lexeme* currNode) {
    currNode = currNode->right = new lexeme(BLOCK);
    if (StatementPending()) {
        Statement(currNode);
    } else if (Check(LOOP)) {
        Loop(currNode);
    } else if (Check(IF)) {
        Conditional(currNode);
    }
    
    if (BlockPending()) {
        Block(currNode);
    } else if (!Check(CBRACE) && !Check(END)) {
        ReportError(); //Special case error call that might not otherwise be covered by match.
    }

    return;
}

/*
               [currNode]
              /
        [+LOOP]
        /     \
[+Value]      [+Block]
 */
void parser::Loop(lexeme* currNode) {
    currNode = currNode->left = Match(LOOP);
    Value(currNode);
    Match(OBRACE);
    Block(currNode);
    Match(CBRACE);
    return;
}

/*
                       [currNode]
                      /
                 [+IF]
                /     \
        [+JOIN]      [?ELSE]
       /       \
[+Value]       [+Block]
 */
void parser::Conditional(lexeme* currNode) {
    currNode = currNode->left = Match(IF);
    currNode->left = new lexeme(JOIN);
    Value(currNode->left);
    Match(OBRACE);
    Block(currNode->left);
    Match(CBRACE);

    if (ElsePending()) {
        Else(currNode);
    }

    return;
}

bool parser::ElsePending() {
    return Check(ELSE);
}

/*
      [currNode]
               \
                [+ELSE]
                /     \
        [+JOIN]      [?ELSE]
       /       \
[?Value]       [+Block]
 */
void parser::Else(lexeme* currNode) {
    currNode = currNode->right = Match(ELSE);
    currNode->left = new lexeme(JOIN);
    if (ValuePending()) {
        Value(currNode->left);
    }
    Match(OBRACE);
    Block(currNode->left);
    Match(CBRACE);
    if (ElsePending()) {
        Else(currNode);
    }
    return;
}

bool parser::StatementPending() {
    return Check(ID) || Check(RETURN) || Check(EQ) || Check(TILDA);
}

/*
       [currNode]
      /
[+???]
 */
void parser::Statement(lexeme* currNode) {
    if (Check(ID) || Check(TILDA)) {
        Call(currNode);
    } else if (Check(RETURN)) {
        Return(currNode);
    } else {
        Definition(currNode);
    }

    Match(SEMICOLON);
    return;
}

/*
                 [currNode]
                /
       [+RETURN]
      /         \
[????]          [????]
 */

void parser::Return(lexeme* currNode) {
    currNode = currNode->left = Match(RETURN);
    if (ValuePending()) {
        Value(currNode);
    } else if (FullArrayPending()){
        FullArray(currNode);
    } else if (Check(TILDA)) {
    	Function(currNode);
    } else {
        return;
    }

    return;
}

/*
                       [currNode]
                      /
                   [+VAR]
                  /     \
        [+Variable]      [+Arglist]
       /       \
[?Index]       [?MemberVar]
 */
void parser::Definition(lexeme* currNode) {
    Match(EQ);
    currNode = currNode->left = new lexeme(VAR);
    Variable(currNode);
    MemberVar(currNode->left);
    ArgList(currNode, false);

    return;
}

/*
             [currNode]
            /
     [+FUNC]
            \      
            [+JOIN]
			/	  \
[?ParamList]	   [+Block]
 */
void parser::Function(lexeme* currNode) {
    Match(TILDA);
    currNode = currNode->left = new lexeme(FUNC);
    currNode = currNode->right = new lexeme(JOIN);
    ParamList(currNode);
    Match(OBRACE);
    Block(currNode);
    Match(CBRACE);
    return;
}

/*
                            [currNode]
                           /
                    [+CALL]
                    /     \
         [+Variable]       [?Arglist]
        /          \
[?Index]           [?MemberVar]
 */
void parser::Call(lexeme* currNode) {
    currNode = currNode->left = new lexeme(CALL);
    if (Check(ID)) {
    	Variable(currNode);
    	MemberVar(currNode->left);
    } else {
    	Function(currNode);
    }
    if (ArgListPending()) {
        ArgList(currNode, false);
    }
    return;
}

//Callfunction, but placed to the right of currNode.
void parser::CallIDKnown(lexeme* currNode, lexeme* dummy) {
    currNode = currNode->right = new lexeme(CALL);
    currNode->left = dummy;
    if (ArgListPending()) {
        ArgList(currNode, false);
    }
    return;
}

/*
			[currNode]
     	   /
      [+ID]
     /
[?ID]
 */
void parser::ParamList(lexeme* currNode) {
    if (Check(ID)) {
        currNode = currNode->left = Match(ID);
        if (Check(ID)) {
            ParamList(currNode);
        }
    }
    return;
}

bool parser::ValuePending() {
    return Check(ID) || Check(INT) || Check(CHAR) || Check(OPAREN) || Check(TILDA);
}

/*
       [currNode]
      /
[+???]
 */
void parser::Value(lexeme* currNode) {
    if (Check(ID)) {
        Variable(currNode);
        MemberVar(currNode->left);
    } else if (Check(INT)) {
        currNode->left = Match(INT);
    } else if (Check(CHAR)) {
        currNode->left = Match(CHAR);
	} else if (Check(OPAREN)) {
        ReturnedValue(currNode);
    } else {
    	Function(currNode);
    }

    return;
}

/*
                    [currNode]
                   /
          [+RetVal]
                  \
                  [+???]
 */
void parser::ReturnedValue(lexeme* currNode) {
    currNode = currNode->left = new lexeme(RETVAL);
    Match(OPAREN);
    if (Check(TILDA)) {
    	lexeme* dummy = new lexeme(JOIN);
    	Call(dummy);
    	currNode->right = dummy->left;
    } else {
	    lexeme* dummy = new lexeme(JOIN);
	    Value(dummy);
	    dummy = dummy->left;
	    if (dummy->type == ID) {
	        MemberVar(dummy);
	    }
	    
	    if ((ValuePending() || FullArrayPending() || Check(CPAREN)) && dummy->type == ID) {
	        CallIDKnown(currNode, dummy);  
	    } else if (OperatorPending()) {
	        Expression(currNode, dummy);
	    }
    }

    Match(CPAREN);
}

/*
Rough reimplementation of Lusth's style in my own.
   "function expression()
        {
        var u,o,e;
        u = unary();
        if (operatorPending())
            {
            o = operator();
            e = expression();
            return cons(o.type,u,e);
            }
        else
            return u;
        }"
 */
void parser::Expression(lexeme* currNode, lexeme* dummy) {
    while (OperatorPending()) {
        lexeme* oper = new lexeme(JOIN);
        Operator(oper);

        lexeme* val = new lexeme(JOIN);
        if (ValuePending()) {
            Value(val);
            dummy = new lexeme(oper->left->type, dummy, val->left);
        } else {
            ReturnedValue(val);
            dummy = new lexeme(oper->left->type, dummy, val->right);
        }
    }
    currNode->right = dummy;
    return;
}

bool parser::OperatorPending() {
    return Check(EQ) || Check(PLUS) || Check(MIN) || Check(TIMES)
            || Check(DIV) || Check(MOD) || Check(OR) || Check(NOT)
            || Check(GREATER) || Check(LESSER);
}

/*
            [currNode]
           /
[+Operator]
 */
void parser::Operator(lexeme* currNode) {
    if (Check(EQ)) {
        currNode->left = Match(EQ);
    } else if (Check(PLUS)) {
        currNode->left = Match(PLUS);
    } else if (Check(MIN)) {
        currNode->left = Match(MIN);
    } else if (Check(TIMES)) {
        currNode->left = Match(TIMES);
    } else if (Check(DIV)) {
        currNode->left = Match(DIV);
    } else if (Check(MOD)) {
        currNode->left = Match(MOD);
    } else if (Check(AND)) {
        currNode->left = Match(AND);
    } else if (Check(OR)) {
        currNode->left = Match(OR);
    } else if (Check(NOT)) {
        currNode->left = Match(NOT);
        Match(EQ);
    } else if (Check(GREATER)) {
        currNode->left = Match(GREATER);
    } else {
        currNode->left = Match(LESSER);
    }

    return;
}

/*
                     [currNode]
                    /
         [+Variable]
        /
[?Index]
 */
void parser::Variable(lexeme* currNode) {
    currNode = currNode->left = Match(ID);
    Index(currNode);
    return;
}

/*
         [currNode]
        /
[+Value]
 */
void parser::Index(lexeme* currNode) {
    if (Check(OBRACKET)) {
        Match(OBRACKET);
        Value(currNode);
        Match(CBRACKET);
    }
    return;
}

/*
            [currNode]
                     \
                     [+MEMVAR]
                    /
         [+Variable]
        /          \
[?Index]           [?Membervar]
 */
void parser::MemberVar(lexeme* currNode) {
    if (Check(DOT)) {;
        currNode = currNode->right = new lexeme(MEMVAR);
        Match(DOT);
        Variable(currNode);
        MemberVar(currNode->left);
    }
    return;
}

bool parser::ArgListPending() {
    return Check(POUND) || Check(STR) || ValuePending();
 }

/*
[currNode]
         \
         [+JOIN]
        /       \
 [?JOIN]        [+???]
 */
void parser::ArgList(lexeme* currNode, bool leftSide) {
    if (leftSide == false) {
        currNode = currNode->right = new lexeme(JOIN);
    } else {
        currNode = currNode->left = new lexeme(JOIN);
    }

    if (FullArrayPending()) {
        FullArray(currNode);
    } else {
        lexeme* dummy = new lexeme(JOIN);
        Value(dummy);
        currNode->right = dummy->left;
    }

    if (ArgListPending()) {
        ArgList(currNode, true);
    }

    return;
}

bool parser::FullArrayPending() {
    return Check(POUND) || Check(STR);
}

/*
  [currNode]
           \
           [+POUND]
          /
[+Variable]

OR

[currNode]
         \
         [+STR]
 */
void parser::FullArray(lexeme* currNode) {
    if (Check(POUND)) {
        currNode = currNode->right = Match(POUND);
        Variable(currNode);
        MemberVar(currNode->left);
    } else {
        currNode->right = Match(STR);
    }

    return;
}

void parser::PrintTree() {
    parseTree->InDisplay();
    PrintTreeR(parseTree->left);
    PrintTreeR(parseTree->right);
    cout << endl;
    return;
}

void parser::PrintTreeR(lexeme* node) {
    if (node != NULL) {
        if (node->type == BLOCK) {
            cout << endl;
        }
        node->InDisplay();
        cout << " ";
        PrintTreeR(node->left);
        PrintTreeR(node->right);
    }
    return;
}