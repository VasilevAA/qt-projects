#ifndef WINDOW_WITH_TREE_H
#define WINDOW_WITH_TREE_H
#include "bin_tree.h"

namespace Ui {
class window_with_tree;
}

class window_with_tree : public QWidget
{
	Q_OBJECT

public:
	explicit window_with_tree(QWidget *parent = 0);
	~window_with_tree();

	void set_bt(node*);

private:
	Ui::window_with_tree *ui;

	node* tree;

	int levels;
	double scale;
	double tr_width;

	double x_start;
	double y_start;
	double x_offset;
	double y_offset;

	double WIDTH;
	double HEIGHT;

	void paint_bt(QPainter*, double,double,int, node*);
protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
};

#endif // WINDOW_WITH_TREE_H
