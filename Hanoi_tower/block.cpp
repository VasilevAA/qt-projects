#include "block.h"
#include "mgraphicview.h"

block::block() : QGraphicsItem(){}

block::block(int s, int l, MGraphicView* par, int m)
	:QGraphicsItem()
{
	parent = par;
	size = s;
	level = l;

	m_height = 300/m; //определение высоты блока с опорой на высоту сцены (320).

	int min_w = 200/m; //определение ширины минимального блока (все остальные блоки кратны).

	m_widht = min_w*size; //ширина данного блока

	y_pos = 320 - level*m_height;
	x_pos = -min_w*size/2;

	get_color(); //задание цвета
	setCursor(Qt::OpenHandCursor);
}

void block::get_color()	//Определение цвета по размеру блока
{
	switch (size%10+1){
	case 1: color = Qt::white; break;
	case 2:	color = Qt::blue; break;
	case 3: color = Qt::red; break;
	case 4: color = Qt::yellow; break;
	case 5: color = Qt::cyan; break;
	case 6: color = Qt::magenta; break;
	case 7: color = Qt::green; break;
	case 8: color = Qt::gray; break;
	case 9: color = Qt::darkRed; break;
	case 10: color = Qt::darkCyan; break;
	}
}

QRectF block::boundingRect() const //Определение ограничивающего прямоугольника
{
	return QRectF(x_pos,y_pos,m_widht,m_height);
}

void block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option); Q_UNUSED(widget);
	painter->setPen(QPen(Qt::black));
	painter->setBrush(color); //Установка цвета заливки
	painter->drawRect(x_pos,y_pos,m_widht,m_height);//Отрисовка прямоугольника и текста в нем
	painter->drawText(x_pos,y_pos,m_widht,m_height,Qt::AlignVCenter | Qt::AlignRight,QString::number(size)+" ");
}
void block::paint_mask(QPainter* painter)
{
//Отрисовка фигуры с текстом при перетаскивании
	painter->setPen(QPen(Qt::black));
	painter->setBrush(color);
	painter->drawRect(0,0,m_widht,m_height);
	painter->drawText(0,0,m_widht,m_height,Qt::AlignVCenter | Qt::AlignRight,QString::number(size)+" ");
}

void block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
	setCursor(Qt::ClosedHandCursor);//изменение курсора при зажатии
}

void block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (QLineF(event->screenPos(),event->buttonDownScenePos(Qt::LeftButton)).length() < QApplication::startDragDistance()
			|| parent->get_height() !=level //Если "схваченный" блок не верхний - drag прекращается
		)
		return;
	setVisible(0); //скрыть блок
	QMimeData * mime = new QMimeData;
	mime->setParent(parent);	//В mime записывается родитель-виджет блока и размер
	mime->setText(QString::number(size));

	QPixmap pixmap(m_widht+1,m_height+1);	//Установка картинки, отображаемой при
	QPainter painter(&pixmap);				//перетаскивании блока.
	paint_mask(&painter);
	painter.end();

	QDrag* drag = new QDrag(event->widget());	//Создание Drag и установка
	drag->setMimeData(mime);					//данных и картинки
	drag->setPixmap(pixmap);

	drag->setHotSpot(QPoint(0,0));	//Установка точки перетаскивания

	drag->exec();   //Вызов непосредственно drag

	setVisible(1);  //Отобразить блок
	setCursor(Qt::OpenHandCursor);	//Установка курсора открытой руки

}

void block::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	Q_UNUSED(event);
	setCursor(Qt::OpenHandCursor);//Курсор открытой руки
}


