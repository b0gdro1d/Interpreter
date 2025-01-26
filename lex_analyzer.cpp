#include "lex_analyzer.hpp"
#include "TID.hpp"
extern Table_Ident TID;

int Lex::get_type(){return t_lex;}
int Lex::get_val(){return v_lex;}
std::ostream & operator <<(std::ostream & out, const Lex & cl)
{
	out << '(' << cl.t_lex << ',' << cl.v_lex << ");";
	return out;
}

Ident::Ident(const char *n){
	name = new char[strlen(n)+1];
	strcpy(name, n);
}
Ident::~Ident(){delete[] name;}
Ident & Ident::operator=(Ident id)
{
	this->put_name(id.get_name());
	var = id.get_var();
	declare = id.get_declare();
	type = id.get_type();
	assign = id.get_assign();
	value = id.get_value();
	return *this;
}
bool Ident::get_var(){return var;}
char* Ident::get_name(){return name;}
void Ident::put_name(const char *n, bool is_rec)
{
	if(is_rec || !name)
	{
		name = new char[strlen(n)+6];
		strcpy(name, n);
	}
}
bool Ident::get_declare(){return declare;}
void Ident::put_declare(){declare = true;}
int Ident::get_type(){return type;}
bool Ident::is_var(){return var;}
void Ident::put_type(int t){type = t;}
bool Ident::get_assign(){return assign;}
void Ident::put_assign(){assign = true;}
int Ident::get_value(){return value;}
void Ident::put_value(int val){value = val;}

Table_Ident::Table_Ident(int max_size)
{
	p = new Ident[size = max_size];
	top = 1;
}
Table_Ident::~Table_Ident(){delete[] p;}
Ident & Table_Ident::operator[](int k){return p[k];}
void Table_Ident::set_size(int s)
{
	p = new Ident[size = s];
	top = 1;
}
int Table_Ident::put(const char *buf)
{
	for(int i=1; i<top; i++)
		if(!strcmp(buf, p[i].get_name()))
			return i;
	p[top].put_name(buf);
	top++;
	return top-1;
}
int Table_Ident::put(Ident id)
{
	p[top++] = id;
	return top-1;
}
int Table_Ident::get_top(){return top;}