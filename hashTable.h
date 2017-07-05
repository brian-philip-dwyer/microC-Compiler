#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "hash.h"
#include "link.h"
#include <iostream>
#include <string>

using namespace std;

class HashTable{
public:
  HashTable(int n);
  HashTable();
  HashTable(const HashTable &t);
  ~HashTable();

  HashTable& put(string key, int value);

  bool contains(string key) const;

  const int get(string key) const;

  void remove(string key);

private:
  int size;
  Link **table; 
	       
};

#endif
