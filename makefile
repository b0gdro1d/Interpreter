all: 
	g++ lex_analyzer.cpp scanner.cpp parser.cpp poliz.cpp executer.cpp interpreter.cpp main.cpp TID.cpp

clean:
	rm a.out