#include "hash.h"

Hash::Hash(string sym) {
  
  int tmp = 0;
  for(size_t i = 0; i < sym.length(); i++) {
    char x = sym.at(i);
    tmp += int(x);
  }
  key = abs(tmp);
}


int Hash::hash() const{
  return key;
}

