//Brian Dwyer
//CSE 109
//bpd219
//Header file for the symbol table.

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "hashTable.h"
#include "stack.cc"
#include <iostream>
#include <string>
#include <cstring>

class SymbolTable {

  
public:

  SymbolTable();
  ~SymbolTable();

  void enterScope();
  void exitScope();
  int addSymbol(string sym);
  int getUniqueSymbol(string sym);
  
  
private:

  int index;
  Stack<HashTable> symbolTable;
  
};


#endif
