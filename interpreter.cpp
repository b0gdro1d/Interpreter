#include"interpreter.hpp"
#include "TID.hpp"
using namespace std;

extern Table_Ident TID;
void Interpreter::interpret(char *program){
	parser.analyze(program);
	executer.execute(parser.prog);
}