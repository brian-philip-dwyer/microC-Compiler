//Brian Dwyer
//CSE 109
//bpd219
//.cc file for the symbol table.

#include "symbolTable.h"
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>


//Given in CourseSite
string itos(int i) { stringstream ss; ss << i; string res = ss.str(); return res;}


//Constructor for SymbolTable:
SymbolTable::SymbolTable() {
  index = 1;
}

//Destructor for SymbolTable:
SymbolTable::~SymbolTable() {}


//Pushes a new hash table/trie onto the stack.
void SymbolTable::enterScope() { 
  symbolTable.push(HashTable(1000));
}


//Pops the top hash table/trie off the stack.
void SymbolTable::exitScope() {
  symbolTable.pop();
}


//Stores variable and corresponding int in the hash table at the top of the stack.
int SymbolTable::addSymbol(string symbol) {
  
  HashTable upperVarScope = symbolTable.peek();  
  if(upperVarScope.contains(symbol)) {
    return 0;
  }
  upperVarScope.put(symbol, index++);

  //Return a 1 if the symbol is added correctly.
  return 1;
}


//Returns the int corresponding to the symbol.
int SymbolTable::getUniqueSymbol(string symbol) {

  for(int i = symbolTable.getTos(); i >= 0; i--) {  
    HashTable& currentScope = symbolTable[i];
    if(currentScope.contains(symbol)) {
      cout<<"\n*****BD calls the shots!!!!*****\n"<<endl;
      return currentScope.get(symbol);
    }
  }

  return 0;
}


