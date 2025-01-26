#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H
#include <iostream>
#include <cstring>
#include <stack>
#include <vector>

enum type_of_lex 
{
    LEX_NULL,

    LEX_NOT, LEX_AND, LEX_OR, LEX_BOOL, LEX_ELSE,/*1, 2, 3, 4, 5*/
    LEX_IF, LEX_FALSE, LEX_INT, LEX_RECORD, LEX_PROGRAM,/*6, 7, 8, 9, 10, 11*/
    LEX_BEGIN, LEX_TYPE, LEX_END, LEX_VAR,/*12, 13, 14*/
    LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE,/*15, 16, 17 18*/
    LEX_THEN, LEX_DO,/*19, 20, 21, 22*/

    LEX_DOT, LEX_SEMICOLON, LEX_FIN, LEX_COMA, LEX_COLON, LEX_ASSIGN,/*23, 24, 25, 26, 27, 28 ,29*/
    LEX_BRAC_OPEN, LEX_BRAC_CLOSE,/*30, 31*/
    LEX_EQ, LEX_LSS, LEX_GRT, LEX_LSEQ, LEX_GTEQ, LEX_NOTEQ,/*32, 33, 34, 35, 36, 37*/
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_DIVIDE,/*38, 39, 40, 41*/
    LEX_UNAR_PLUS, LEX_UNAR_MINUS,/*42, 43*/
    LEX_NUM,/*44*/
    LEX_ID,/*45*/
    LEX_TYPE_INT, LEX_TYPE_BOOL,/*46, 47, 48*/

    POLIZ_LABEL,/*49*/
    POLIZ_ADDRESS,/*50*/
    POLIZ_GO,/*51*/
    POLIZ_FGO,/*52*/
    POLIZ_DELETE,/*53*/
};

class Lex 
{
    int t_lex;
    int v_lex;
    public:
    Lex(int t = LEX_NULL, int v = 0): t_lex(t), v_lex(v) {}
    int get_type(void);
    int get_val(void);
    friend std::ostream& operator<<(std::ostream& out, const Lex & l);
};

class Ident 
{
    char *name;
    bool var;
    bool declare;
    int metatype;
    int type;
    bool assign;
    int value;
    public:
	Ident(bool v=true, bool s=false): name(nullptr), var(v), declare(false), assign(false) {}
	Ident(const char* n);
	~Ident();
	Ident& operator=(Ident id);
	bool get_var();
	char* get_name();
	void put_name(const char* n, bool is_rec = false);
	bool get_declare();
	void put_declare();
	int get_type();
	bool is_var();
	void put_type(int t);
	bool get_assign();
	void put_assign();
	int get_value();
	void put_value(int v);
};

class Table_Ident
{
	Ident *p;
	int size;
	int top;
    public:
	Table_Ident(): size(0), top(0){}
	Table_Ident(int max_size);
	~Table_Ident();
	Ident & operator [](int k);
	void set_size(int s);
	int put(const char *buf);
	int put(Ident id);
	int get_top();
};
#endif
