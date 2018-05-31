#ifndef BIN_TREE_H
#define BIN_TREE_H
#include <QStack>
#include <QValidator>
#include <QMainWindow>
#include <QWidget>
#include <Qpainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

typedef int base;

struct node
{
	base root;
	node* left;
	node* right;

	bool is_path;
	int size;
	node* parent;
	node(base);

	static node* insert(node*,base);
	static int getsize(node*);
	static void fixsize(node*);
	static node* rotateright(node*);
	static node* rotateleft(node*);
	static node* insertroot(node*,base);

	static node* join(node*&, node*);
	static node* remove_el(node*&, base);

	static node* find_el(node*, base);
	static void set_path(node*);
	static void clear_path(node*);

	static void create_parents(node*&, node*);
	static void destroy_bt(node*&);
	static int hgt(const node*);
	static void print_bt(const node*, QString &);
};

#endif // BIN_TREE_H
