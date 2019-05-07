Richard Wiedenroth

________________/\\\_______________________________________________________        
 _______________\/\\\_______________________________________________________       
  _______________\/\\\____________________________________________/\\\\\\\\__      
   __/\\\\\\\\\\\_\/\\\______________/\\\\\\\\\_____/\\/\\\\\\____/\\\////\\\_     
    _\///////\\\/__\/\\\_____________\////////\\\___\/\\\////\\\__\//\\\\\\\\\_    
     ______/\\\/____\/\\\_______________/\\\\\\\\\\__\/\\\__\//\\\__\///////\\\_   
      ____/\\\/______\/\\\______________/\\\/////\\\__\/\\\___\/\\\__/\\_____\\\_  
       __/\\\\\\\\\\\_\/\\\\\\\\\\\\\\\_\//\\\\\\\\/\\_\/\\\___\/\\\_\//\\\\\\\\__ 
        _\///////////__\///////////////___\////////\//__\///____\///___\////////___

"The last programming language you will ever want to choose, and not in a good way."

Introduction:
  zLang (henceforth referred to as Z) is an imperative interpeted language built around
  a minimalist syntax. It provides mechanisms to allow for easy usage of arrays and
  recursion to improve your productivity- not that you can really be productive with
  a language that leaks this much memory. This guide will teach you how to get the most
  out of Z. All code snippets contained will be surrounded in pipe symbols (the '|'
  symbol) for clarity. While this guide isn't all encompassing, it covers everything you
  will need to know to program using Z assuming a general understanding of other programming
  languages.

Comments:
  To create a comment (text that will not be evaluated by Z), surround the desired
  portion of code in backslashes:
    |\This will not be evaluated or executed\|
    |\It even works across multiple lines of |
    |text!\                                  |
    |This will be evaluated and executed     |
  Throughout this guide, code snippets will be annotated with comments.

Building and Running Z:
  The supplied makefile can be used to compile the Z language into an executable named
  'zLang'. It is assumed that you have G++ installed with support for C++14. Z scripts
  can be run by running the executable with the script provided as an argument (IE: 'zLang
  myScript.z'). Z has been tested using WSL on Windows 10.

Variables:
  Declaring variables in Z is simple. Just place an 'equals' sign followed by
  the name of your variable, any values it contains (it *must* contain something),
  and a semicolon:
    |=x 1;|
  Every variable in Z is a variably typed dynamic array, as a result, you can
  follow up your variable with a list of values of the size you want.
    |=array 1 2 3 4 5;        |
    |=array 1 'a' 2 'b' 3 'c';|
  As with most other languages, variables can be used as values for declaration:
    |=x 5;                   |
    |=y x; \y is equal to 5\ |
  If you want to include all values within a variable, then that variable can be
  prefixed with a pound symbol. This is especially useful for strings because, as
  far as Z is concerned, a string is just an array of characters:
    |=hi "Hello";                               |
    |=wo "world.";                              |
    |=hw #hi ' ' #wo; \hw equals "Hello world."\|
  Specific values within an array can be accessed in much the same way as other languages
  by using square brackets surrounding a value that resolves to the desired index.
  Note that in all cases other than the name in definition or assignment, just supplying
  the name of a variable without an index is equivalent to specifying the first index.
  Z uses 0 based indexing.
    |=hi "Hello";                                        |
    |=wo "world.";                                       |
    |=s wo hi[1] wo[3] wo[4]; \s equals "weld"\          |
    |=s[0] hi[0]; \=s[0] hi; has the same result- "Held"\|
    |=s[1] wo[1]; \s now equals "Hold"\                  |
  The syntax for assignment, that is changing variables, is identical to declaration.
  The one that will be used depends on whether or not the given name exits in the
  current 'environment':
    |=x 5; \x equals 5\    |
    |=x 6; \x now equals 6\|

Control Flow:
  Z provides two main methods of control flow, conditionals and loops.
  Conditionals act much the same way as if/else in other languages. They execute the
  demarcated block of code if the conditional value resolves to anything but zero, and
  move onto the next bit if it does resolve to 0:
    |IF x {                                                  |
    |    \some code that executes when x is not 0\           |
    |} ELSE y {                                              |
    |    \some code that executes when y is not 0 and x is 0\|
    |} ELSE {                                                |
    |    \some code that executes when x and y are 0\        |
    |}                                                       |
  Loops work in much the same way, repeatedly executing the demarcated block of code
  until its conditional value resolves to 0.
    |LOOP x {                                                 |
    |    \some code that repeatedly executes until x is not 0\|
    |}                                                        |
  Note that I used the word resolve when describing how these constructs treat the
  conditional value. That's because you can surround a mathematical expression in
  parenthesis to have it resolve to a value:
    |IF (x = 5) {                           |
    |   \Executes when x is equal to 5\     |
    |}                                      |
    |IF (y > x) {                           |
    |   \Executes when y is greater than x\ |
    |}                                      |
    |IF (y < x) {                           |
    |   \Executes when y is less than x\    |
    |}                                      |
    |IF (y & x) {                           |
    |    \Executes when x and y aren't 0\   |
    |}                                      |
    |IF (y | x) {                           |
    |    \Executes when x or y aren't 0\    |
    |}                                      |
    |IF (x != y) {                          |
    |   \Executes when x is not equal to y\ |
    |}                                      |
  Because variable declarations and assignment look for values to tie to a name,
  you can define variables using mathematical expressions. Expressions can also
  make use of parenthesis inside of them to control the order things are calculated.
    |=a (1 + 1); \a is 2\      |
    |=b (3 - 1); \b is 2\      |
    |=c (a * b); \c is 4\      |
    |=d (c / a); \d is 2\      |
    |=e (c % a); \e is 0\      |
    |=f ((a + e) * d); \f is 4\|
    |=g (a + (e * d)); \g is 2\|
  This is very useful for getting value out of loops:
    |=i 0;                  |
    |LOOP (i < 10) {        |
    |    \Executes 10 times\|
    |    =i (i + 1);        |
    |}                      |
  Note that only the equal '=' and not-equal '!=' operators work with characters.

Functions:
  Functions in Z aren't too disimilar from other languages. They contain a block of
  code and a list of parameters to be taken in as arguments when called. In Z, 
  functions are treated as just another type of value. As a result, the syntax
  should look quite familiar:
    |=FUNCTIONNAME ~param1 p2 \Number of parameters can be anything\ {|
    |    \Code goes here\                                             |
    |};                                                               |
  Calling a function is easy. Just say the name, the list of arguments to be passed in,
  and a semicolon:
    |FUNCTIONNAME arg1 arg2;|
  Functions normally 'return' the last value they have evaluated, but your intentions
  can be made explicit with a RETURN statement.
    |=FUNCTIONNAME ~param1 p2 {     |
    |    RETURN \insert value here\;|
    |};                             |
  In many cases, you might want to call a function as part of an expression. You
  can make use of parenthesis around a function call to evaluate it:
    |=FIVE ~{ RETURN 5; };     |
    |=x (FIVE); \x is 5\       |
    |=y ((FIVE) + 1); \y is 6\ |
  Because functions are values, they can be used anonymously in any case where a value
  can be used:
    |=x (~{ RETURN 5; }); \x is 5\                                                        |
    |=y (~a b { RETURN (a + b); } 5 1); \y is 6\                                          |
    |SOME_CALL ~a b { RETURN (a + b); }; \Calls SOME_CALL using a function as a parameter\|
  As a word of warning, while these features are similar to how functions and parenthesis 
  are handled in lisp derivatives; the following snippet of code is syntactically invalid.
    |=MULT ~a {                          |
    |    RETURN ~b { RETURN (a * b); };  |
    |};                                  |
    |=x ((MULT 2) 4);                    |
  Z thinks of the "((MULT 5) 5)" as being syntactically equivalent to (value value)
  rather than (function_call argument), so the parser will return an error. The
  correct way to write this code is:
    |=MULT ~a {                          |
    |    RETURN ~b { RETURN (a * b); };  |
    |};                                  |
    |=DOUBLE (MULT 2);                   |
    |=x (DOUBLE 4); \x is 8\             |

Objects:
  An class constructor in Z is created in much the same way as a function, the only
  difference is that you return 'THIS' at the end.
    |=OBJECT ~a b c {                              |
    |    =memberOne a;                             |
    |    =memberTwo (a - b);                       |
    |    =METHOD ~x y {                            |
    |        RETURN (c x y);                       |
    |    };                                        |
    |    RETURN THIS;                              |
    |}                                             |
  Now, to create an instance of this class, we simply call the function and store it
  in a variable. The resulting object's methods and member variables can be accessed by
  appending a '.' to your variable names. Note that the members of an object are 'local'
  to that object.
    |=thing (OBJECT 5 2 ~x y { RETURN (x + y); }); |
    |=x thing.memberOne; \x is 5\                  |
    |=y thing.memberTwo; \y is 3\                  |
    |=thing.memberTwo 5; \memberTwo is now 5\      |
    |=z (thing.METHOD 2 2); \z is 4\               |

Printing to Console:
  Z comes with two simple methods to print to the console:
    PRINT: Prints out the values or types of given variables in order.
      |=x 1;                                   |
      |=s "abcd";                              |
      |==func { RETURN 5; };                   |
      |PRINT 5 'a' "str " x s #s func (func);  |
      |\Prints "5 astr 1 aabcdCLOSURE5 "\      |
    PRINTL: Identical to print, but it inserts a newline at the end.

Command Line Arguments:
  Z provides easy access to the arguments passed into the command line via the CL object.
  Individual arguments can be accessed using the '.arg' member on an inex matching the desired
  argument. For example, if we execute a program and supply the arguments "file.txt -w -v", then
  here are the things contained within CL.
    |PRINT #CL[0].arg; \Prints 'file.txt'\|
    |PRINT #CL[1].arg; \Prints '-w'\      |
    |PRINT #CL[2].arg; \Prints '-v'\      |
  You may want to get the number of command line arguments, you can do this by using the builtin
  SIZE function on the CL object. The SIZE function returns the number of values in an array.
    |PRINT (SIZE #CL); \Prints 3\                 |
    |=ints 1 2 3 4 5;                             |
    |PRINT (SIZE #ints); \Prints 5\               |
    |=string "Words!";                            |
    |PRINT (SIZE #ints #string #CL) \Prints 5 6 3\|

File Reading:
  The final set of builtin functions provided by Z are used for very basic file reading. Here is
  an example program that uses each of these functions to print out every integer from a file
  specified through the command line:
    |\FOPEN returns an array of file pointers based on the passed in strings.\       |
    |=fp (FOPEN #CL[0].arg);                                                         |
    |LOOP (FEOF fp) { \FEOF checks if the given file pointer is at the end of a file\|
    |    PRINT (FGET fp); \FGET returns an integer from a file\                      |
    |}                                                                               |
    |FCLOSE fp; \FCLOSE closes the file.\                                            |

Planned Features:
  1. Garbage collection and removal of memory leaks.
    As it stands right now, Z's memory model is nonexistent. Likewise, it currently does not free
    certain portions of data that are allocated and then lost during the parsing and evaluation
    processes. The current plan is to implement a tri-color marking system that silently operates
    on all lexemes that are allocated such that existing code can remain unchanged and that
    and to address any potential memory leaks that may appear in the future.
  2. Remove '==' syntax for function declarations.
    This is an older form of function declaration that offers little advantage over the current lambda
    based form of declaration. Removing it would allow for some nice simplification of the parser and
    evaluator.
  3. RETURN
    It currently doesn't stop execution of the current function, this is dumb and should be an easy
    fix.
  4. Rework error handling.
    This would involve providing a more consistent mechanism for the parser, lexer, and evaluator
    to report errors; and creating a system by which users can create constraints on code to catch
    errors.
  5. Standard library features for debugging.
    Provide builtin functions for performing debugging actions such as displaying the current
    environment or returning a trace stack. This is especially important since the language
    currently doesn't provide very helpful error messages.