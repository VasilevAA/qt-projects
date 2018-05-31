#include "lay_widget.h"
#include "mainwindow.h"

lay_widget::lay_widget(QWidget* parent) : QWidget(parent)
{
	is_line = 1; is_curve = 0; is_rect = 0; is_ellipse = 0;
	first.setX(0);
	first.setY(0);

	wid = 2;	//Установка начальной ширины линии

	//Установка стартового цвета и стиля ручки
	temp_color = Qt::black;
	temp_style = Qt::PenStyle(1);

	//Установка стартовой заливки
	temp_brush_color =  Qt::transparent;

	this->setGeometry(8,78,1280,720);

	this->setAutoFillBackground(1);
	this->setPalette(QPalette(Qt::white));



}
lay_widget::~lay_widget()
{
	figures.clear();
	deleted_figures.clear();
}

void lay_widget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);



	QPainter painter(this);

	//установка сглаживания
	painter.setRenderHints(QPainter::RenderHint(0x08),true);

	paint_all_figures(&painter);//прорисовка всех фигур из стека
	//painter.setCompositionMode(QPainter::CompositionMode_Xor);
	set_pen_brush(painter);//установка ручки и заливки для текущей фигуры


	//прорисовка текущей фигуры в зависимости от выделенного флажка
	if( is_line && !temp_line.isNull())
		painter.drawLine(temp_line);

	if(is_rect)
		painter.drawRect(temp_rect);

	if(is_ellipse)
		painter.drawEllipse(temp_rect);

	if(is_curve)
		for (int i = 0; i < (temp_points.size()-1); i++)
			painter.drawLine(temp_points[i],temp_points[i+1]);

}

//функция прорисовки всех фигур из стека
void lay_widget::paint_all_figures(QPainter *painter)
{
	if(!figures.isEmpty())
		for(int i = 0; i < figures.size(); i++)
			figures[i].paint_figure(painter);
}

//обработка нажатия левой кнопки мыши
void lay_widget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton )
			first = event->pos();
}

//обработка движения мыши
void lay_widget::mouseMoveEvent(QMouseEvent *event)
{

	//распознавание зажатия клавиши Shift
	is_shift_pressed = Qt::ShiftModifier & QApplication::keyboardModifiers();

	//Если отмечен флажок линии и зажата лкм
	if(event->buttons() == Qt::LeftButton && is_line)
	{
		t_pos = event->pos(); //конечная точка - текущая точка

		//при зажатом shift рисуется вертикальная, диагональная или горизонтальная линия
		if(is_shift_pressed)
		{
			int X,Y;

			int x = abs(t_pos.x()-first.x());
			int y = abs(t_pos.y() - first.y());
			//в зависимости от близости к оси рисуется одна из линий
			//горизонтальная
			if(2*y <= x)
			{
				X = t_pos.x();
				Y = first.y();
			}

			//вертикальная
			else if(2*x <= y)
			{
				X = first.x();
				Y = t_pos.y();
			}

			//диагональная
			else
			{
				if (t_pos.x() >= first.x())
					X = first.x()+x;
				else
					X = first.x()-x;

				if(t_pos.y()>=first.y())
					Y = first.y()+x;
				else
					Y = first.y()-x;
			}
			temp_line.setPoints(first,QPoint(X,Y));

		}
		//если Shift не зажат - устанавливается обычная линия
		else
			temp_line.setPoints(first,t_pos);

		this->update();
	}

	//если отмечен флажок прямоугольника или эллипса и зажата лкм
	if(event->buttons() == Qt::LeftButton  && (is_rect || is_ellipse))
	{
		t_pos = event->pos(); //конечная точка - текущая точка
		temp_rect.setBottomLeft(first);

		//если зажат shift
		if(is_shift_pressed)
		{
			int X,Y;
			int mimi = std::min(abs(t_pos.x()-first.x()),abs(t_pos.y() - first.y())) ;

			if(is_rect)
			{
				if (t_pos.x() >= first.x())
					X = first.x()+mimi-2;
				else
					X = first.x()-mimi;

				if(t_pos.y()>=first.y())
					Y = first.y()+mimi;
				else
					Y = first.y()-mimi+2;
			}
			else
			{
				if (t_pos.x() >= first.x())
					X = first.x()+mimi;
				else
					X = first.x()-mimi;

				if(t_pos.y()>=first.y())
					Y = first.y()+mimi;
				else
					Y = first.y()-mimi;
			}
			temp_rect.setTopRight(QPoint(X,Y));
		}

		//shift не зажат
		else
			temp_rect.setTopRight(t_pos);

				this->update();
	}

	if(event->buttons() == Qt::LeftButton && is_curve)
	{
		temp_points.push_back(event->pos());
		this->update();
	}
}

//обработка отпускания лкм
void lay_widget::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);

	//если выбрана линия
	if(!temp_line.isNull() && is_line)
	{
		m_figures b(temp_line, temp_pen);//создание фигуры-линии
		figures.push_back(b);			//заносим фигуру в стек
		temp_line.setLine(0,0,0,0);//обнуляем нач. точку
		this->update();
	}
	//аналогично обработка прямоугольника и эллипса
	if(!temp_rect.isNull() && is_rect)
	{
		m_figures b(temp_rect, temp_pen,temp_brush);
		figures.push_back(b);
		temp_rect.setTopLeft(QPoint(0,-30));
		temp_rect.setBottomRight(QPoint(0,-30));
		this->update();
	}

	if(!temp_rect.isNull() && is_ellipse)
	{
		m_figures b(temp_rect, temp_pen,temp_brush, 1);
		figures.push_back(b);
		temp_rect.setTopLeft(QPoint(0,-30));
		temp_rect.setBottomRight(QPoint(0,-30));
		this->update();
	}
	if(is_curve)
	{
		m_figures b(temp_points,temp_pen,temp_brush);
		figures.push_back(b);
		temp_points.clear();
		this->update();
	}
}

//функция-слот отмены последнего действия
void lay_widget::back_action()
{
	if(figures.size()==0)
		return;

	deleted_figures.push_back(figures.last());//сохранение удаляемой фигуры
	figures.pop_back();//удаление отрисовываемой фигуры
	this->update();
}

//функция-слот повтора действия
void lay_widget::on_forward_clicked()
{
	if(deleted_figures.size()==0)
		return;

	figures.push_back(deleted_figures.last());//извлечение удаленой фигуры
	deleted_figures.pop_back(); //удаление из стека удаленных фигур
	this->update();
}


//фунция для установки ручки и заливки
void lay_widget::set_pen_brush(QPainter& painter)
{
	temp_brush.setColor(temp_brush_color);
	temp_brush.setStyle(Qt::BrushStyle(1));
	painter.setBrush(temp_brush);

	temp_pen.setColor(temp_color);
	temp_pen.setStyle(Qt::PenStyle(temp_style));
	temp_pen.setWidth(wid);
	temp_pen.setCapStyle(Qt::RoundCap);
	temp_pen.setJoinStyle(Qt::MiterJoin);
	painter.setPen(temp_pen);
}

void lay_widget::change_size(int w, int h, bool is_new)
{
	if(is_new)
	{
		figures.clear();
		deleted_figures.clear();
	}
	this->setGeometry(8,78,w,h);
	this->update();

}

