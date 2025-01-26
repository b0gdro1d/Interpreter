#include <iostream>
#include <cstring>
#include <vector>
#include <stack>

#include "lex_analyzer.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "poliz.hpp"
#include "executer.hpp"
#include "interpreter.hpp"
#include "TID.hpp"
extern Table_Ident TID;

int main(int argc, char **argv){
	Parser parser;
	char program[150];
	strcpy(program, argv[1]);
	try{
		Interpreter Inter;
		Inter.interpret(program);
	}
	catch(char c){
		std::cout << "INVALID SYMBOL: " << c << std::endl;
	}
	catch(const char *str){
		std::cout << "WARN!:ERROR: " << str << std::endl;
	}
	catch(Lex l){
		std::cout << "LEXEMME " << l << " WAS UNEXPECTED" << std::endl;
	}
	catch(std::string str){
		std::cout << "ERROR!: " << str << std::endl;
	}
	return 0;
}