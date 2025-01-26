#include "parser.hpp"
#include "TID.hpp"
extern Table_Ident TID;

void Parser::start_cycle(){in_cycle++;}
void Parser::finish_cycle(){in_cycle--;}
bool Parser::is_in_cycle(){return in_cycle>0;}
void Parser::forbid_descr(){descr_permit = false;}
void Parser::gl(){
	curr_lex = scan.get_lex();
	c_type = curr_lex.get_type();
	c_val = curr_lex.get_val();
}
void Parser::parse_prog(const char *program){
	scan.scan_prog(program);
}
void Parser::close_prog(const char *program){
	scan.close_prog(program);
}
void Parser::check_id(){
	if(TID[c_val].get_declare()){
		if(TID[c_val].is_var()){
			st_lex.push(TID[c_val].get_type());
		}
		else throw "WARN!: there is a non-variable in expression";
	}else throw "WARN!: it is not declared!";
}
void Parser::check_op(){
	int t1, op, t2, t, r;
	t2 = st_lex.top(); st_lex.pop();
	op = st_lex.top(); st_lex.pop();
	t1 = st_lex.top(); st_lex.pop();
	if(t1 != t2){
		if(!((t1 == LEX_TYPE_INT && t2 == LEX_NUM) || ((t1 == LEX_TYPE_BOOL) && (t2 == LEX_TRUE || t2 == LEX_FALSE)) || (t1 == LEX_TRUE && t2 == LEX_FALSE) || (t1 == LEX_FALSE && t2 == LEX_FALSE)))
		{
			throw "WARN!: wrong op types in operation";
		}
	}
	if(op == LEX_ASSIGN){
		if(t1 == LEX_TRUE || t1 == LEX_FALSE){
			t = LEX_TYPE_BOOL;
			r = LEX_TYPE_BOOL;
		}else{
			t = t1;
			r = t1;
		}
	}else if(op == LEX_PLUS){
		if(t1 == LEX_TYPE_INT){
			r = LEX_TYPE_INT;
			t = LEX_TYPE_INT;
		}
		else throw "WARN!: wrong op types in operation";
	}else if(op == LEX_MINUS || op == LEX_TIMES || op==LEX_DIVIDE){
		r = LEX_TYPE_INT;
		t = LEX_TYPE_INT;
	}else if(op >= LEX_EQ && op <= LEX_NOTEQ){
		if(t1 == LEX_TYPE_BOOL) throw "WARN!: wrong op types in operation";
		r = LEX_TYPE_BOOL;
		t = t1;
	}else if(op == LEX_OR || op == LEX_AND){
		r = LEX_TYPE_BOOL;
		t = LEX_TYPE_BOOL;
	}
	if(t2 != t) throw "WARN!: wrong op types in operation";
	st_lex.push(r);
	prog.put_lex(Lex(op));
}
void Parser::check_not(){
	if(st_lex.top() != LEX_TYPE_BOOL)
		throw "WARN!: wrong op types in operation";
	else{
		st_lex.pop();
		st_lex.push(LEX_TYPE_BOOL);
		prog.put_lex(Lex(LEX_NOT));
	}
}

void Parser::eq_bool(){
	if(st_lex.top() != LEX_TYPE_BOOL) throw "WARN!:non-bool in if or while expression";
	st_lex.pop();
}

void Parser::check_id_in_read(){
	if(!TID[c_val].is_var()) throw "non-variable can not be used as read argumetn";
	if(!TID[c_val].get_declare()) throw "read argument was not declared yet";
}
void Parser::analyze(const char *program)
{
	parse_prog(program);
	gl();
	P();
	close_prog(program);
}

void Parser::P(){
	if(c_type == LEX_PROGRAM) gl();
	else throw curr_lex;
	if(c_type == LEX_VAR) gl();
	else throw curr_lex;
	D1();
	forbid_descr();
	if(c_type == LEX_BEGIN) gl();
	else throw curr_lex;
	B();
	if(c_type == LEX_END) gl();
	else throw curr_lex;
	if(c_type != LEX_FIN) throw "WARN!: no end of program";
}

void Parser::D1(){
	char* recname = nullptr;
	if(c_type == LEX_ID){
		D(recname);
		while(c_type == LEX_COMA){
			gl();
			D(recname);
		}
		if(c_type != LEX_SEMICOLON) throw curr_lex;
		gl();
		if(c_type == LEX_TYPE) D1();
	}
	else if(c_type == LEX_TYPE){
		gl();
		if(c_type != LEX_ID) throw curr_lex;
		recname = TID[c_val].get_name();
		gl();
		if(c_type != LEX_COLON) throw curr_lex;
		gl();
		if(c_type != LEX_RECORD) throw curr_lex;
		gl();
		D(recname);
		while(c_type == LEX_COMA){
			gl();
			D(recname);
		}
		if(c_type != LEX_END) throw curr_lex;
		gl();
		if(c_type == LEX_ID) D1();
	}else throw curr_lex;
}

void Parser::D(char* recname){
	if(c_type != LEX_ID){
		throw curr_lex;
	}
	while(c_type == LEX_ID){
		if (recname != nullptr)
		{
			std::string newname = recname + std::string(".") + TID[c_val].get_name();
			TID[c_val].put_name(newname.c_str(), true);
		}
		v_identifiers.push_back(c_val);
		gl();
		if(c_type == LEX_COMA){
			gl();
		}else if(c_type != LEX_COLON){
			throw curr_lex;
		}
	}
	gl();
	int to_ident;
	if(c_type == LEX_INT){
		to_ident = LEX_TYPE_INT;
	}else if(c_type == LEX_BOOL){
		to_ident = LEX_TYPE_BOOL;
	}else throw curr_lex;
	for(unsigned i = 0; i < v_identifiers.size(); i++){
		int v_val = v_identifiers[i];
		if(!TID[v_val].get_declare()){
			TID[v_val].put_type(to_ident);
			TID[v_val].put_declare();
		}else{
			throw (std::string("Variable ") + std::string(TID[v_val].get_name()) + std::string(" is declared twice"));
		}
	}
	v_identifiers.clear();
	gl();
}

void Parser::B(){
	while(c_type != LEX_END){
		if(c_type == LEX_FIN) throw "WARN!: end of program expeceted";
		S();
		if(c_type == LEX_SEMICOLON) gl();
	}
}

void Parser::S(){
	int pl2, pl3;
	if(c_type == LEX_VAR || c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_TYPE || c_type == LEX_RECORD){
		throw "WARN!: not possible for descriptions to be in operator scope";
	}
	if(c_type == LEX_IF){
		gl();
		E();
		eq_bool();
		pl2 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_FGO));
		if(c_type != LEX_THEN) throw curr_lex;
		gl();
		S();
		if(c_type != LEX_SEMICOLON) throw curr_lex;
		gl();
		pl3 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
		if(c_type == LEX_ELSE){
			gl();
			S();
			if(c_type != LEX_SEMICOLON) throw curr_lex;
			gl();
		}
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
	}else if(c_type == LEX_WHILE){
		int pl0, pl1;
		gl();
		pl0 = prog.get_free();
		E();
		eq_bool();
		pl1 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_FGO));
		if(c_type != LEX_DO) throw curr_lex;
		gl();
		start_cycle();
		S();
		if(c_type != LEX_SEMICOLON) throw curr_lex;
		gl();
		finish_cycle();
		prog.put_lex(Lex(POLIZ_LABEL, pl0));
		prog.put_lex(Lex(POLIZ_GO));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
	}else if(c_type == LEX_READ){
		gl();
		if(c_type != LEX_BRAC_OPEN) throw curr_lex;
		gl();
		if(c_type != LEX_ID) throw curr_lex;
		check_id_in_read();
		int tmp = c_val;
		gl();
		prog.put_lex(Lex(POLIZ_ADDRESS, tmp));
		if(c_type != LEX_BRAC_CLOSE) throw curr_lex;
		gl();
		prog.put_lex(Lex(LEX_READ));
	}else if(c_type == LEX_WRITE){
		gl();
		if(c_type != LEX_BRAC_OPEN) throw curr_lex;
		gl();
		E();
		prog.put_lex(Lex(LEX_WRITE));
		if(c_type != LEX_BRAC_CLOSE) throw curr_lex;
		gl();
	}else if(c_type == LEX_BEGIN){
		gl();
		B();
		if(c_type !=LEX_END) throw curr_lex;
		gl();
	}else if(c_type == LEX_ID){
		int tmp1 = c_val;
		int tmp_val = c_val;
		gl();
		if(c_type == LEX_ASSIGN){
			tmp_val = c_val;
			c_val = tmp1;
			check_id();
			prog.put_lex(Lex(POLIZ_ADDRESS, tmp1));
			c_val = tmp_val;
			st_lex.push(LEX_ASSIGN);
			gl();
			E();
			if(c_type != LEX_SEMICOLON) throw curr_lex;
			check_op();
			prog.put_lex(Lex(POLIZ_DELETE));
		}else if(c_type != LEX_SEMICOLON){
			c_val = tmp1;
			check_id();
			c_val = tmp_val;
			st_lex.push(c_type);
			gl();
			E();
			check_op();
		}else{
			c_val = tmp1;
			check_id();
			st_lex.pop();
			c_val = tmp_val;
			gl();
		}
	}else if(c_type == LEX_SEMICOLON) gl();
}

void Parser::E(){
	E1();
	while(c_type >= LEX_EQ && c_type <=LEX_NOTEQ){
		st_lex.push(c_type);
		gl();
		E1();
		check_op();
	}
}

void Parser::E1(){
	T();
	while(c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR){
		st_lex.push(c_type);
		gl();
		T();
		check_op();
	}
}

void Parser::T(){
	F();
	while(c_type == LEX_TIMES || c_type == LEX_DIVIDE || c_type == LEX_AND){
		st_lex.push(c_type);
		gl();
		F();
		check_op();
	}
}

void Parser::F(){
	if(c_type == LEX_BRAC_OPEN){
		gl();
		F();
		if(c_type != LEX_BRAC_CLOSE) throw curr_lex;
		gl();
	}else if(c_type == LEX_NOT){
		gl();
		F();
		check_not();
	}else if(c_type == LEX_NUM){
		st_lex.push(LEX_TYPE_INT);
		prog.put_lex(curr_lex);
		gl();
	}else if(c_type == LEX_ID){
		Lex tmp;
		tmp = curr_lex;
		check_id();
		gl();
		prog.put_lex(tmp);
	}else if(c_type == LEX_TRUE || c_type == LEX_FALSE){
		st_lex.push(LEX_TYPE_BOOL);
		prog.put_lex(curr_lex);
		gl();
	}else if(c_type == LEX_PLUS || c_type == LEX_MINUS){
		Lex tmp;
		if(c_type == LEX_PLUS){
			tmp = Lex(LEX_UNAR_PLUS);
		}else{
			tmp = Lex(LEX_UNAR_MINUS);
		}
		gl();
		F();
		prog.put_lex(tmp);
	}else if((c_type != LEX_SEMICOLON) && (c_type != LEX_BRAC_CLOSE)) throw curr_lex;
}
