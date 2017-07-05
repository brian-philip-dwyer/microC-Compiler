#include "stack.h"
#include <cstdlib>

template<class T>
Stack<T>::Stack(int n):size(n),tos(Stack<T>::EMPTY) {
  stack = new T[size];
}

template<class T>
Stack<T>::Stack():size(10),tos(-1) {
  stack = new T[size];
}

template<class T>
Stack<T>::Stack(const Stack<T>& s)  { // Copy constructor
  size = s.size;
  tos = s.tos;
  stack = new T[size];
  for (int i = 0; i <= tos; ++i) {
    stack[i] = s.stack[i];
  }
}

template<class T>
Stack<T>::~Stack() {
  delete [] stack;
}

template<class T>
void Stack<T>::push(T val) {
  if (tos >= size - 1) {
    cerr << "Stack overflow\n";
    exit(1);
  } else {
    stack[++tos] = val;
  }
}

template<class T>
T Stack<T>::pop() {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos--];
  }
}

template<class T>
T Stack<T>::peek() const {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos];
  }
}

template<class T>
int Stack<T>::getTos() {
  return tos;
}

template<class T>
int Stack<T>::getSize() {
  return size;
}



/*template<class T>
ostream& operator <<(ostream& out, const Stack<int>& s){
  out << "[ ";
  for (int i=0; i <= s.tos; i++) {
    out << s.stack[i] << " ";
  }
  out << "]";  
  return out;
  }*/

template<class T>
void Stack<T>::checkIndex(int i) const {
  if (i < 0 || i > tos) {
    cerr << "Index out of bounds.\n";
    exit(1);
  }  
}

template<class T>
T& Stack<T>::operator [](int i){
  checkIndex(i);
  return stack[i];
}

template<class T>
const T Stack<T>::operator [](int i) const {
  checkIndex(i);  
  return stack[i];
}
