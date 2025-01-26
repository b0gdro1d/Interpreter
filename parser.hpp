#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
#include "poliz.hpp"
#include"scanner.hpp"

class Parser
{
	Lex curr_lex;
	int c_type;
	int c_val;
	int in_cycle;
	bool descr_permit;
	Scanner scan;
	std::stack<int> st_lex;
	std::vector<int> v_identifiers;
	void P(); void D1(); void B();
/*D1->*/	void D(char* recname);
	void S();
	void E();//=E
	void E1();//=E1
	void T();//=T
	void F();//=F
	void check_id();
	void check_op();
	void check_not();
	void eq_bool();
	void check_id_in_read();
	void start_cycle();
	void finish_cycle();
	bool is_in_cycle();
	void forbid_descr();
	void gl();
    public:
	Poliz prog;
	Parser(): in_cycle(0), descr_permit(true), prog(1000){}
	void parse_prog(const char *program);
	void close_prog(const char *program);
	void analyze(const char *program);
};

#endif