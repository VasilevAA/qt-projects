#ifndef FORM_H
#define FORM_H
#include "mainwindow.h"
#include <QWidget>
#include <QPaintEvent>
namespace Ui {
class Form;
}

class Form : public QWidget
{
	Q_OBJECT

public:
	explicit Form(QWidget *parent = 0);
	~Form();
	void set_form(QVector<Vertex*>);
private:

	double segments;
	int radius;
	QPoint center;
	QVector<Vertex*> graph;
	QVector<QPoint> self_arr;


	Ui::Form *ui;

	void paint_graph(QPainter* painter);

protected:
	void paintEvent(QPaintEvent* event);

};

#endif // FORM_H
