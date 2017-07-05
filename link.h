#ifndef LINK__H
#define LINK_H
#include <iostream>
#include <fstream>
using namespace std;


class Link{
public:
  string key;  //used to identify the Links; should be unique
  int data;  //the actual information
  Link *next;
  
  /*Set key=k, data=d, next=nx*/
  Link(string k, int d = 0, Link *nx = NULL);

  /*Make a copy of the link t. Note that we copy the pointer, thus not
  a deep copy
  */
  Link(const Link & t);

  void show(Link*t);
  /*Print out in the form  (2, 2.7)  */
  friend ostream& operator <<(ostream& out, const Link& t);
};
#endif
