/* Richard Wiedenroth
Implementation for scanner.h */

#include "main.h"

int main (int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Invalid number of command line arguments.\n";
		return -1;
	}

	ifstream program;
	program.open(argv[1]);
	
	if (program.fail()) {
		cerr << "Error opening file.\n";
		return -1;
	}

	lexer lexer(program, false);
	parser parser(lexer, false);
	bool semResult = parser.Scan();

	if (DEBUG_PARSE) {
		parser.PrintTree();
		printer printer(parser.parseTree);
		printer.Print();
		cout << endl << endl;
	}

	if (!semResult) { //If lexer or parser ran into an error.
		cout << "Illegal" << endl;
		return 0;
	} else {
		evaluator ev = evaluator(DEBUG_EVAL, argv, argc);
		ev.evaluate(parser.parseTree);
	}
	
	return 0;
}