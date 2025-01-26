#include "scanner.hpp"
#include "TID.hpp"
extern Table_Ident TID;

const char * Scanner::TW[]={
	NULL,
	"not",
	"and",
	"bool",
	"do",
	"else",
	"if",
	"false",
	"int",
	"record",
	"or",
	"program",
	"begin",
	"type",
	"end",
	"var",
	"read",
	"then",
	"true",
	"while",
	"write",
	"!",
	"!F",
	NULL
};

const char * Scanner::TD[]={
	NULL,
	".",
	";",
	"@",
	",", 
	":",
	":=",
	"(",
	")",
	"==",
	"<",
	">",
	"+",
	"-",
	"*",
	"/",
	"<="
	"!=",
	">=",
	NULL
};

const type_of_lex Scanner::words[]={
	LEX_NULL, LEX_NOT, LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF,
	 LEX_FALSE, LEX_INT, LEX_RECORD, LEX_OR, LEX_PROGRAM, LEX_BEGIN, LEX_TYPE, LEX_END, LEX_VAR,
	  LEX_READ, LEX_THEN, LEX_TRUE, LEX_WHILE, LEX_WRITE, LEX_NULL
};

const type_of_lex Scanner::dlms[]={LEX_NULL, LEX_DOT, LEX_SEMICOLON, LEX_FIN, LEX_COMA, LEX_COLON,
	 LEX_ASSIGN, LEX_BRAC_OPEN, LEX_BRAC_CLOSE, LEX_EQ, LEX_LSS, LEX_GRT, LEX_PLUS, LEX_MINUS,
	  LEX_TIMES, LEX_DIVIDE, LEX_LSEQ, LEX_NOTEQ, LEX_GTEQ, LEX_NULL
};

void Scanner::add(){
	if(buf_top < 79)
		buf[buf_top++] = c;
	else{
		std::cout << "WARN!: identifier name too long!: " << buf << std::endl;
	}
}

void Scanner::add(int num){
	if(num == 0){
		buf[buf_top++] = '0';
		return;
	}
	add(num/10);
	buf[buf_top++] = '0' + num%10;
}

void Scanner::clear(){
	for(int i=0; i < 86; i++)
		buf[i] = '\0';
	buf_top = 0;
}

int Scanner::look(const char *buf, const char **list) const{
	int i = 1;
	while(list[i]){
		if(!strcmp(buf, list[i]))
			return i;
		i++;
	}
	return 0;
}

void Scanner::gc(){
	c = fgetc(fp);
}

void Scanner::scan_prog(const char *prog){
	fp = fopen(prog, "r");
	cs = H;
	clear();
	gc();
}

void Scanner::close_prog(const char *prog){
	fclose(fp);
}

Lex Scanner::get_lex(){
	int d, j;
	cs = H;
	do{
		switch(cs){
		case H:
			if(c == ' ' || c == '\n' || c == '\r' || c == '\t')
				gc();
			else if(isalpha(c)){
				j = 0;
				clear();
				add();
				gc();
				cs = IDENT;
			}else if(isdigit(c)){
				d = c - '0';
				gc();
				cs = NUMB;
			}else if(c == '{'){
				gc();
				cs = COM;
			}else if(c == ':' || c == '=' || c == '<' || c == '>'){
				clear();
				add();
				gc();
				cs = ALE;
			}else if(c == '@'){
				return Lex(LEX_FIN);
			}else if(c == '!'){
				clear();
				add();
				gc();
				cs = NEQ;
			}else{
				cs = DELIM;
			}
			break;
		case IDENT:
			if(isalpha(c) || isdigit(c) || c == '.'){
				add();
				gc();
			}else if((j = look(buf, TW)) != 0){
				return Lex(words[j], j);
			}else{
				j = TID.put(buf);
				return Lex(LEX_ID, j);
			}
			break;
		case NUMB:
			if(isdigit(c)){
				d = d*10 + c - '0';
				gc();
			}else{
				return Lex(LEX_NUM, d);
			}
			break;
		case COM:
			if(c == '}'){
				gc();
				cs = H;
			}else if(c == '@'){
				throw c;
			}else{
				gc();
			}
			break;
		case ALE:
			if(c == '='){
				add();
				gc();
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}else{
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}
			break;
		case NEQ:
			if(c == '='){
				add();
				gc();
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}else{
				throw '!';
			}
			break;
		case DELIM:
			clear();
			add();
			j = look(buf, TD);
			if(j){
				gc();
				return Lex(dlms[j], j);
			}else{
				throw c;
			}
			break;
		}
	}while(true);
}