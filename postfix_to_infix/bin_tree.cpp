#include "bin_tree.h"

node::node()
{
	root = "";
	left = NULL;
	right = NULL;
	parent = NULL;
}

void node::new_bt(node*& tr, QStack<QString>& stack)
{
	tr = new node;
	if(QString("+*-/").indexOf(stack.top()) != -1)
	{
		tr->root = stack.pop();
		new_bt(tr->right,stack);
		new_bt(tr->left,stack);
	}
	else
	{
		tr->root = stack.pop();
		return;
	}
}

int node::hgt(const node* tr)
{
	if(!tr)
		return 0;
	return (hgt(tr->left)>hgt(tr->right)? hgt(tr->left):hgt(tr->right)) +1;
}

void node::print_bt(node*& tr,QString& str)
{
	if(tr==NULL)
		return;
	if((tr->root == "+" || tr->root == "-")
			&& tr->parent
			&&(tr->parent->root == "*" || tr->parent->root == "/"))
		str+=" ( ";
	print_bt(tr->left,str);
	str+=" " +tr->root + " ";
	print_bt(tr->right,str);
	if((tr->root == "+" || tr->root == "-")
			&& tr->parent
			&& (tr->parent->root == "*" || tr->parent->root == "/"))
		str+=" ) ";
}

void node::create_parents(node*& tr, node* parent)
{
	if(!tr)
		return;
	create_parents(tr->left,tr);
	create_parents(tr->right,tr);
	tr->parent = parent;
}

void node::destroy_bt(node*& tr)
{
	if(!tr)
		return;
	destroy_bt(tr->left);
	destroy_bt(tr->right);
	delete tr;
}
