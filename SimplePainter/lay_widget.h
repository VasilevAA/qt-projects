#ifndef LAY_WIDGET_H
#define LAY_WIDGET_H
#include "qt_libs.h"
#include "m_figures.h"

class lay_widget : public QWidget
{
	Q_OBJECT
public:
	lay_widget(QWidget* parent = 0);
	~lay_widget();
	void get_info(bool a, bool b, bool c, bool d,int e, int f)
	{
		is_line = a;
		is_rect = b;
		is_ellipse = c;
		is_curve = d;
		temp_style = e;
		wid = f;
	}
	void get_color(QColor a) {temp_color = a;}
	void get_brush(QColor a) {temp_brush_color = a;}
	void change_size(int w,int h,bool is_new = 0);
	bool is_empty(){return figures.isEmpty() ? true : false ;}
private:
	bool is_line;
	bool is_rect;
	bool is_ellipse;
	bool is_curve;


	int wid;//Ширина обводки


	QPoint t_pos;//Конечная точка фигуры
	QPoint first;//Стартовая точка фигуры.

	QPen temp_pen;	//Стиль, цвет обводки
	QColor temp_color;
	int temp_style;

	QBrush temp_brush;	//Цвет заливки
	QColor temp_brush_color;

	//линия
	QLine temp_line;

	//прямоуг. или эллипс
	QRect temp_rect;

	//кривая из точек
	QVector<QPoint> temp_points;

	bool  is_shift_pressed;//Зажата ли клавиша shift

	QVector<m_figures> figures;	//Все отрисованные фигуры
	QVector<m_figures> deleted_figures;//Удаленные фигуры


	void paint_all_figures(QPainter*);//Прорисовка всех фигур
	void set_pen_brush(QPainter& );//Установки ручки и кисти


protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:

	void back_action();
	void on_forward_clicked();


};

#endif // LAY_WIDGET_H
