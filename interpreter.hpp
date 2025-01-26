#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include "parser.hpp"
#include "executer.hpp"

class Interpreter{
	Parser parser;
	Executer executer;
	char *program;
public:
	void interpret(char *program);
};
#endif