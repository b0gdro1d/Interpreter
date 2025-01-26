#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include"lex_analyzer.hpp"

class Scanner
{
	enum state{ H, IDENT, NUMB, COM, ALE, DELIM, NEQ};
	static const char *TW[];
	static const type_of_lex words[];
	static const char *TD[];
	static const type_of_lex dlms[];
	state cs;
	FILE *fp;
	char c;
	char buf[86];
	int buf_top;
	void clear();
	void add();
	void add(int num);
	int look(const char *buf, const char **list) const;
	void gc();
	public:
	Scanner(){}
	void scan_prog(const char *prog);
	void close_prog(const char *prog);
	Lex get_lex();
};
#endif
