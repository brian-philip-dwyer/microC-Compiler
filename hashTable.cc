#include "hashTable.h"
#include <cstring>
#include <string>

HashTable::HashTable(int n):size(n){
  table = new Link*[n];
  for(int j = 0; j < size; j++)
    table[j] = NULL;
}

HashTable::HashTable():size(50){
  table = new Link*[50];
  for(int j = 0; j < size; j++)
    table[j] = NULL;
}

HashTable::HashTable(const HashTable& t):size(t.size){
  table = new Link*[size];
  for(int j = 0; j < size; j++)
    table[j] = t.table[j];
}

HashTable::~HashTable(){ // TODO: Fix to delete all links
  
  /*for(int j = 0; j < size; j++)
    if(table[j] != NULL)
      delete table[j];
      delete [] table;*/  
}


HashTable& HashTable::put(string key, int value){
  int h;
  h = Hash(key).hash() % size;
  Link* link = new Link(key, value, table[h]);
  table[h] = link;
  link->show(table[h]);
  return *this;
}


bool HashTable::contains(string k) const{
  int h;
  h = Hash(k).hash() % size;
  Link *temp;
  temp = table[h];
  temp->show(temp);
  while(temp != NULL && temp->key != k)
    temp = temp->next;
  return temp != NULL;
}


const int HashTable::get(string str) const{
  int h;
  Link *temp;
  h = Hash(str).hash() % size;
  temp = table[h];
  while(temp != NULL && temp->key != str)
    temp = temp->next;
  if (temp == NULL) return NULL;
  else return temp->data;
}


void HashTable::remove(string str){
  int h;
  Link *tempa, *tempb;
  h = Hash(str).hash() % size;
  if(table[h]->key == str){
    tempa = table[h];
    table[h] = table[h]->next;
  }
  else{
    tempb = table[h];
    while(tempb->next->key != str)
      tempb = tempb->next;
    tempa = tempb->next;
    tempb->next = tempb->next->next;
  }
  delete tempa;
}

