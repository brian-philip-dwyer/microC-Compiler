#CSE 109
#Brian Dwyer
#bpd219
#Makefile for Compiler Part Two

OBJ = microc.o parser.o lexer.o token.o symbolTable.o stack.o hashTable.o link.o hash.o
OPTS = -g -c -Wall -Werror

microc: $(OBJ)
	g++ -o microc $(OBJ)

microc.o: microc.cc parser.h lexer.h token.h
	g++ $(OPTS) microc.cc

parser.o: parser.h parser.cc lexer.h symbolTable.h stack.h  
	g++ $(OPTS) parser.cc

lexer.o: lexer.h lexer.cc token.h
	g++ $(OPTS) lexer.cc

token.o: token.cc token.h
	g++ $(OPTS) token.cc

symbolTable.o: symbolTable.cc symbolTable.h stack.h hashTable.h
	g++ $(OPTS) symbolTable.cc

stack.o: stack.cc stack.h
	g++ $(OPTS) stack.cc

hashTable.o: hashTable.cc hashTable.h hash.h link.h
	g++ $(OPTS) hashTable.cc 

link.o: link.cc link.h
	g++ $(OPTS) link.cc

hash.o: hash.cc hash.h
	g++ $(OPTS) hash.cc

clean:
	rm -f *.o *~ *.gch *# microc *.asm
