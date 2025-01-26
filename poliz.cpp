#include "poliz.hpp"
#include "TID.hpp"
extern Table_Ident TID;
Poliz::Poliz(int max_size){
	p = new Lex[size = max_size];
	free = 0;
}
Poliz::~Poliz(){delete[] p;}
void Poliz::put_lex(Lex l){
	p[free++] = l;
}
void Poliz::put_lex(Lex l, int place){
	if(place < 0 || place >= size) throw "WARN:POLIZ!:wrong place";
	p[place] = l;
}
Lex Poliz::pop_lex(){
	if(free > 0){
		return p[--free];
	}else throw "WARN:POLIZ!: nothing to pop";
}
void Poliz::blank(){
	free++;
	if(free >= size) throw "WARN:POLIZ!: out of range";
}
int Poliz::get_free(){return free;}
Lex & Poliz::operator[](int k){
	if(k < 0 || k >= size) throw "WARN:POLIZ!: place is out of range";
	if(k > free) throw "WARN:POLIZ!: no such actual place";
	return p[k];
}
void Poliz::print(){
	for(int i=0; i < free; i++)
		std::cout << p[i];
}