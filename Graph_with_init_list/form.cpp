#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include <QTransform>
#include <QFileDialog>
Form::Form(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Form)
{
	ui->setupUi(this);
	this->setWindowTitle("Граф");
	/*this->setGeometry(0,30,900,900)*/;
	this->setFixedSize(900,900);
}

Form::~Form()
{
	delete ui;
}


void Form::set_form(QVector<Vertex*> a)
{
	graph = a;
	segments = (2*3.14159265 )/ graph.size();
	radius = 400;
	center.setX(450);
	center.setY(450);
}

void Form::paintEvent(QPaintEvent *event)
{
	if(this->isVisible())
	{
		this->activateWindow();

		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing,true);
		painter.setPen(QPen(Qt::black,2));
		painter.setBrush(Qt::white);
		double current_angle = 0;

		for(int i = 0 ; i < graph.size(); i++)
		{

			graph[i]->set_point(QPoint(center.x()+radius*cos(current_angle),center.y()+ radius*sin(current_angle)));
			self_arr.push_back(QPoint(center.x()+(radius+40)*cos(current_angle),center.y()+ (radius+40)*sin(current_angle)));
			current_angle+= segments;
//			painter.drawPoint(QPoint(center.x()+radius*cos(current_angle),center.y()+ radius*sin(current_angle)));
		}
		paint_graph(&painter);
	}
}

void Form::paint_graph(QPainter *painter)
{
	for(int i = 0; i <graph.size();i++)
	{
		if(!graph[i]->children.isEmpty())
			for(int k = 0; k <graph[i]->children.size(); k++)
			{

				QPoint a = (graph[i])->children[k]->point;

				if(graph[i] == graph[i]->children[k])
				{
					painter->setBrush(Qt::transparent);
					painter->drawEllipse(self_arr[i],20,20);
					painter->setBrush(Qt::white);
				}
				QPoint b = graph[i]->point;
				int x = a.x()-b.x();
				int y = a.y()-b.y();
				double len = sqrt(x*x+y*y);
				double sinas = y/len;
				double cosas = x/len;
				QPoint arr_top(cosas*(len-30)+b.x(),b.y()+sinas*(len-30));
				QPoint arr_bottom(cosas*(len-50)+b.x(),b.y()+sinas*(len-50));
				QPoint first_end(arr_bottom.x()+sinas*5,arr_bottom.y()-cosas*5);
				QPoint second_end(arr_bottom.x()-sinas*5,arr_bottom.y()+cosas*5);

				painter->drawLine(b,a);
				painter->drawLine(first_end,arr_top);
				painter->drawLine(second_end,arr_top);

			}
	}
	for(int i = 0; i < graph.size(); i++)
	{
		painter->drawEllipse(graph[i]->point,30,30);
		painter->drawText(graph[i]->point.x()-30,graph[i]->point.y()-15,60,30,0x0084,graph[i]->name);
	}


}



