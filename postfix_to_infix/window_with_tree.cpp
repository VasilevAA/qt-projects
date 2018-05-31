#include "window_with_tree.h"
#include "ui_window_with_tree.h"
#include "mainwindow.h"


window_with_tree::window_with_tree(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::window_with_tree)
{
	ui->setupUi(this);


	setWindowFlags(Qt::Tool);
	setWindowTitle("Дерево");
}

window_with_tree::~window_with_tree()
{
	delete ui;
	delete this;

}

void window_with_tree::set_bt(node * tr)
{
	tree = tr;
	levels = node::hgt(tr);

	WIDTH = 34;
	HEIGHT = 30;
	tr_width = (pow(2,levels-1)*WIDTH);
	x_start = tr_width/2+4;
	y_start = 10;

	scale = 1;
	QPalette pal;
	pal.setBrush(this->backgroundRole(),QBrush(QColor(220,240,220)));
	this->setPalette(pal);
	this->resize(tr_width+4,levels*(HEIGHT+40));
}

void window_with_tree::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	if(this->isVisible())
	{
		this->activateWindow();
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing,true);
		painter.setPen(QPen(Qt::black,1));
		painter.setBrush(Qt::white);
		painter.scale(scale,scale);

		paint_bt(&painter, x_start,y_start, levels, tree);
	}

}

void window_with_tree::paint_bt(QPainter * painter, double x, double y, int levels, node* tree)
{
	int left_top_x = x - WIDTH/2;
	int left_top_y = y;
	int bot_x = x;
	int bot_y = y+HEIGHT;

	painter->drawEllipse(left_top_x,left_top_y,WIDTH-4,HEIGHT);
	painter->drawText(left_top_x,left_top_y,WIDTH-4,HEIGHT,0x0084,tree->root);
	if(levels>=2 && tree->left)
	{
		int new_top_x = bot_x - pow(2,levels)*WIDTH/8;
		int new_top_y = bot_y + 40;
		painter->drawLine(bot_x-2,bot_y,new_top_x-2,new_top_y);
		paint_bt(painter,new_top_x,new_top_y,levels-1,tree->left);
	}
	if(levels>=2 && tree->right)
	{
		int new_top_x = bot_x + pow(2,levels)*WIDTH/8;
		int new_top_y = bot_y + 40;
		painter->drawLine(bot_x-2,bot_y,new_top_x-2,new_top_y);
		paint_bt(painter,new_top_x,new_top_y,levels-1,tree->right);
	}
}


void window_with_tree::wheelEvent(QWheelEvent *event)
{
	double delta = event->delta()/120;
	if(delta<0)
		scale-=0.02;
	else
		scale+=0.02;
	this->update();
}

void window_with_tree::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton)
	{
	x_start = x_offset+event->x()/scale;
	y_start = y_offset+event->y()/scale;
	this->update();
	}
	if(event->buttons() == Qt::RightButton)
	{

			WIDTH += (1.0*event->x()-x_offset)/30;
		this->update();
	}
}

void window_with_tree::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton)
	{
		x_offset = x_start-event->x()/scale;
		y_offset = y_start-event->y()/scale;
		this->setCursor(Qt::ClosedHandCursor);
	}
	else if(event->buttons() == Qt::RightButton)
	{
		x_offset = event->x();
		this->setCursor(Qt::SizeHorCursor);
	}
}

void window_with_tree::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
	this->setCursor(Qt::OpenHandCursor);
}

