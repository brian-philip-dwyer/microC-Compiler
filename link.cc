#include "link.h"


Link::Link(string k, int d, Link* nx):key(k),data(d),next(nx){
  //cout<<"Key: "<<k<<" Index: "<<d<<endl;
}

Link::Link(const Link& t):key(t.key),data(t.data),next(t.next){}

ostream & operator<<(ostream& out,const Link& x){
  out << "("<< x.key << ", " << x.data << ")";
  return out;
}


void Link::show(Link *t){

  while(t != NULL){
    cout << "-->" << *t;
    t = t->next;
  }
  cout<<endl;
}
