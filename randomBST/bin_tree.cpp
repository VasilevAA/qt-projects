#include "bin_tree.h"
node::node(base r)
{
	root = r;
	left =right = NULL;
	parent = NULL;
	size = 1;
	is_path = 0;
}

node* node::rotateright(node * p)
{
	node* q = p->left;
	if(!q) return p;
	p->left = q->right;
	q->right = p;
	q->size = p->size;
	fixsize(p);
	return q;
}

node* node::rotateleft(node* q)
{
	node* p = q->right;
	if(!p) return q;
	q->right = p->left;
	p->left = q;
	p->size = q->size;
	fixsize(q);
	return p;
}

int node::getsize(node* p)
{
	if(!p) return 0;
	return p->size;
}

void node::fixsize(node* p)
{
	p->size = getsize(p->left)+getsize(p->right) + 1;
}

node* node::insertroot(node * p, base k)
{
	if(!p) return new node(k);
	if(k < p->root)
	{
		p->left = insertroot(p->left,k);
		return rotateright(p);
	}
	else
	{
		p->right = insertroot(p->right,k);
		return rotateleft(p);
	}
}

node* node::insert(node* p, base k)
{
	if(!p) return new node(k);
	if(rand()%(p->size+1) ==0 )
		return insertroot(p,k);
	if(p->root > k)
		p->left = insert(p->left,k);
	else
		p->right = insert(p->right,k);

	fixsize(p);
	return p;
}

int node::hgt(const node* tr)
{
	if(!tr)
		return 0;
	return (hgt(tr->left)>hgt(tr->right)? hgt(tr->left):hgt(tr->right)) +1;
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

node* node::find_el(node * tr, base k)
{
	if(!tr) return NULL;
	if(k == tr->root)
		return tr;
	if(k < tr->root)
		return find_el(tr->left,k);
	else
		return find_el(tr->right,k);

}


void node::set_path(node* tr)
{
	if(!tr) return;
	tr->is_path = 1;
	set_path(tr->parent);
}

void node::clear_path(node* tr)
{
	if(!tr) return;
	tr->is_path = 0;
	clear_path(tr->left);
	clear_path(tr->right);
}

node* node::join(node *& p, node *q)
{
	if(!p) return q;
	if(!q) return p;
	if(rand()%(p->size+q->size) < p->size)
	{
		p->right = join(p->right,q);
		fixsize(p);
		return p;
	}
	else
	{
		q->left = join(p,q->left);
		fixsize(q);
		return q;
	}
}

node* node::remove_el(node *& p, base k)
{
	if(!p) return p;
	if(p->root == k)
	{
		node* q = join(p->left,p->right);
		delete p;
		return q;
	}
	else if(k < p->root)
		p->left = remove_el(p->left,k);
	else
		p->right = remove_el(p->right,k);
	return p;
}


void node::print_bt(const node * tr, QString& out)
{
	if(!tr) return;
	print_bt(tr->left,out);
	out+=QString::number(tr->root) + " ";
	print_bt(tr->right,out);
}
