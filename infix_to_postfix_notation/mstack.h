#ifndef MSTACK_H
#define MSTACK_H
#include "mainwindow.h"

template<class base>
class MStack
{
private:
	struct elem{
		base value;
		elem* prev;
	} * head;
public:
	MStack(): head(NULL){}
	~MStack(){ while(!Empty())	base tmp = pop();}
	bool Empty(){ return !head ? 1:0;}
	base top(){ return Empty()? 0:head->value;}
	base pop(){
		if(Empty())
			return 0;
		base ret = head->value;
		elem * del = head;
		head = head->prev;
		delete del;
		return ret;
	}
	void push(base val){
		elem * n_head = new elem;
		n_head->value = val;
		n_head->prev = head;
		head = n_head;
	}
};
#endif // MSTACK_H
