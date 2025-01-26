#ifndef POLIZ_H
#define POLIZ_H

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include"lex_analyzer.hpp"

class Poliz{
	Lex* p;
	int size, free;
public:
	Poliz(int max_size);
	~Poliz();
	void put_lex(Lex l);
	void put_lex(Lex l, int place);
	Lex pop_lex();
	void blank();
	int get_free();
	Lex & operator[](int k);
	void print();
};
#endif
