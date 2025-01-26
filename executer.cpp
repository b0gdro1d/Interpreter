#include "executer.hpp"
#include "TID.hpp"
extern Table_Ident TID;

void Executer::execute(Poliz & prog)
{
	std::stack<int> args;
	int i, j, index =0, size = prog.get_free(), last_type;
	while(index < size){
		pc_el = prog[index];
		switch(pc_el.get_type())
		{
		case LEX_TRUE: case LEX_FALSE:{
			i = (pc_el.get_type() == LEX_TRUE)? 1 : 0;
			args.push(i);
			last_type = pc_el.get_type();
			break;
		}
		case LEX_NUM:
		case POLIZ_ADDRESS: case POLIZ_LABEL:{
			args.push(pc_el.get_val());
			last_type = pc_el.get_type();
			break;
		}
		case LEX_ID:{
			i = pc_el.get_val();
			if(TID[i].get_assign()){
				last_type = TID[i].get_type();
				args.push(TID[i].get_value());
				break;
			}else{
				std::string out = "WARN!:POLIZ: wrong identifier: ";
				out +=TID[i].get_name();
				throw out;
			}
		}
		case LEX_NOT:{
			i = !args.top(); args.pop();
			args.push(i);
			break;
		}
		case LEX_OR:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i || j);
			break;
		}
		case LEX_AND:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i && j);
			break;
		}
		case POLIZ_GO:{
			i = args.top(); args.pop();
			index = i - 1;
			break;
		}
		case POLIZ_FGO:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			if(!j) index = i-1;
			break;
		}
		case POLIZ_DELETE:{
			args.pop(); break;
		}
		case LEX_WRITE:{
			i = args.top(); args.pop();
			if((last_type == LEX_TYPE_INT) || (last_type == LEX_NUM))
				std::cout << i;
			else if (last_type == LEX_TYPE_BOOL){
				if(!i)
					std::cout << "false";
				else
					std::cout << "true";
			}else if(last_type == LEX_TRUE)
				std::cout << "true";
			else if(last_type == LEX_FALSE)
				std::cout << "false";
			std::cout << std::endl;
			break;
		}
		case LEX_READ:{
			int input;
			i = args.top(); args.pop();
			if(TID[i].get_type() == LEX_TYPE_INT){
				std::cin >> input;
				TID[i].put_value(input);
			}else if(TID[i].get_type() == LEX_TYPE_BOOL){
				std::string str;
				std::cin >> str;
				if(str == "true")
					input = 1;
				else if (str == "false")
					input = 0;
				else throw "WRONG INPUT: non-boolean";
				TID[i].put_value(input);
			}else throw "WRONG INPUT TYPE";
			TID[i].put_assign();
			break;
		}
		case LEX_PLUS:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i + j);
			break;
		}
		case LEX_UNAR_PLUS:{
			i = args.top(); args.pop();
			args.push(i);
			break;
		}
		case LEX_MINUS:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(j - i);
			break;
		}
		case LEX_UNAR_MINUS:{
			i = args.top(); args.pop();
			args.push(-i);
			break;
		}
		case LEX_TIMES:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i * j);
			break;
		}
		case LEX_DIVIDE:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			if (i){args.push(j / i); break;}
			else throw "CAN NOT DIVIDE BY ZERO";
		}
		case LEX_EQ:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i == j);
			last_type = LEX_TYPE_BOOL;
			break;
		}
		case LEX_NOTEQ:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(i != j);
			last_type = LEX_TYPE_BOOL;
			break;
		}
		case LEX_LSS:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(j < i);
			last_type = LEX_TYPE_BOOL;
			break;
		}
		case LEX_GRT:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(j > i);
			last_type = LEX_TYPE_BOOL;
			break;
		}
		case LEX_LSEQ:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(j <= i);
			last_type = LEX_TYPE_BOOL;
			break;
		}
		case LEX_GTEQ:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			args.push(j >= i);
			break;
		}
		case LEX_ASSIGN:{
			i = args.top(); args.pop();
			j = args.top(); args.pop();
			TID[j].put_value(i);
			TID[j].put_assign();
			args.push(i);
			break;
		}
		default:throw "SOMETHING WENT WRONG IN POLIZ";
		}
	index++;
	}
}