#ifndef M_FIGURES_H
#define M_FIGURES_H
#include "qt_libs.h"

//Класс для фигур.
class m_figures
{
private:

	QLine line;//Линия или квадрат/круг
	QRect rect;
	QVector<QPoint> curve;

	QPen pen;	//Параметры обводки и заливки
	QBrush brush;

	int what_figure; //1 - line; 2 - rect; 3 - circle; 4 - curve

public:
	m_figures(){}

	m_figures(QLine& a, QPen b)//Конструктор для линии
	{
		line = a;
		what_figure = 1;
		pen = b ;
	}

	//Конструктор для прямоугольника/эллипса
	m_figures(QRect& a,QPen b,QBrush c,int is_ellipse=0)
	{
		pen = b;
		brush = c;
		rect = a;
		if (is_ellipse==1)
			what_figure = 3;
		else
			what_figure = 2;
	}

	m_figures(QVector<QPoint> a,QPen b, QBrush c)
	{
		curve = a;
		brush = c;
		pen = b;
		what_figure = 4;
	}

	void paint_figure(QPainter*);

};






#endif // M_FIGURES_H
