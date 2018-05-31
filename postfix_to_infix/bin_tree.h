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

struct node
{
	QString root;
	node* left;
	node* right;

	node* parent;
	node();
	static void new_bt(node*&, QStack<QString>&);
	static void print_bt(node*&, QString&);
	static void create_parents(node*&, node*);
	static void destroy_bt(node*&);
	static int hgt(const node*);
};

#endif // BIN_TREE_H
