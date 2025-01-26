#ifndef EXECUTER_H
#define EXECUTER_H
#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include "poliz.hpp"


class Executer{
	Lex pc_el;
public:
	void execute(Poliz & prog);
};
#endif