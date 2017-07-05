/*J Femister
  CSE 109
*/
#ifndef STACK_H
#define STACK_H
#include <iostream>
using namespace std;

/* 
   A templated implementation of a stack.
*/

template<class T>
class Stack {
public:
  Stack(int n);
  Stack();
  Stack(const Stack&);
  ~Stack();
  void push(T val);
  T pop();
  T peek() const;
  int getTos();
  int getSize();
  bool isEmpty() const { return tos == EMPTY; } 
  //friend ostream& operator <<(ostream& out,const Stack<T>& s);
  T& operator [](int i);
  const T operator [](int i) const;
private:
  T *stack;
  int size;
  int tos;
  static const int EMPTY = -1;
  void checkIndex(int i) const;

};
#endif
