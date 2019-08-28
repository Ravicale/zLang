CC = g++
OBJS = main.o parser.o lexeme.o
OOPTS = -Wall -Wextra -g -c -std=c++11
LOPTS = -Wall -Wextra -g -std=c++11 	

all : main

main : main.o evaluator.o ztdLib.o parser.o lexer.o lexeme.o type.h
	g++ $(LOPTS) main.o evaluator.o ztdLib.o parser.o lexer.o lexeme.o -o zLang

main.o : main.cpp main.h parser.h evaluator.h lexer.h lexeme.h 
	g++ $(OOPTS) main.cpp

evaluator.o : evaluator.cpp evaluator.h ztdLib.h lexeme.h type.h
	g++ $(OOPTS) evaluator.cpp

ztdLib.o : ztdLib.cpp ztdLib.h lexeme.h type.h
	g++ $(OOPTS) ztdLib.cpp

parser.o : parser.cpp parser.h lexeme.h lexer.h
	g++ $(OOPTS) parser.cpp

lexer.o : lexer.cpp lexer.h lexeme.h
	g++ $(OOPTS) lexer.cpp

lexeme.o : lexeme.cpp lexeme.h
	g++ $(OOPTS) lexeme.cpp

example : all
	./zLang example/arrays.z
	./zLang example/conditionals.z
	./zLang example/recursion.z
	./zLang example/iteration.z
	./zLang example/functions.z
	./zLang example/lambda.z
	./zLang example/objects.z

example-mem : all
	valgrind ./zLang example/arrays.z
	valgrind ./zLang example/conditionals.z
	valgrind ./zLang example/recursion.z
	valgrind ./zLang example/iteration.z
	valgrind ./zLang example/functions.z
	valgrind ./zLang example/lambda.z
	valgrind ./zLang example/objects.z

error1 :
	cat example/error1.z

error1x : all
	./zLang example/error1.z

error2 :
	cat example/error2.z

error2x : all
	./zLang example/error2.z

error3 :
	cat example/error3.z

error3x : all
	./zLang example/error3.z

error4 :
	cat example/error4.z

error4x : all
	./zLang example/error4.z

error5 :
	cat example/error5.z

error5x : all
	./zLang example/error5.z

arrays :
	cat example/arrays.z

arraysx : all
	./zLang example/arrays.z

conditionals :
	cat example/conditionals.z
	
conditionalsx : all
	./zLang example/conditionals.z

recursion :
	cat example/recursion.z

recursionx : all
	./zLang example/recursion.z

iteration :
	cat example/iteration.z

iterationx : all
	./zLang example/iteration.z

functions :
	cat example/functions.z

functionsx : all
	./zLang example/functions.z

lambda :
	cat example/lambda.z

lambdax : all
	./zLang example/lambda.z

objects :
	cat example/objects.z

objectsx : all
	./zLang example/objects.z

problem :
	cat example/problem.z
	cat example/ints.txt

problemx : all
	./zLang example/problem.z example/ints.txt

clean :
	rm -f $(OBJS) *.o *.h.gch zLang
