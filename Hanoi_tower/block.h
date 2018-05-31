//Класс для описания блоков, из которых состоит пирамида

#ifndef BLOCK_H
#define BLOCK_H
#include <QtWidgets>

class MGraphicView;

class block : public QGraphicsItem
{
public:
	block();

	block(int s, int l, MGraphicView * par, int m);
//Конструктор, которому на вход подается: размер блока s (отн. величина);
//уровень блока l (чем меньше, тем ниже);
//указатель на GraphicView, в котором находится блок par;
//макс. количество блоков в одной пирамиде m.

	int get_size() const {return size;}
	//Функция возвращает размер блока.
	QRect get_rect()
	{
		QRect a(QPoint(x_pos,y_pos),QPoint(x_pos+m_widht,y_pos+m_height));
		return a;
	}


private:
	int m_height;	//Высота блока в пикс.
	int m_widht;	//Ширина блока в пикс.

	int level;		//Уровень блока
	int size;		//Размер блока

	int x_pos;		//Координаты блока в сцене
	int y_pos;


	int max_bl{10}; //макс. количество блоков в пирамиде
	QColor color;	//Цвет блока
	MGraphicView * parent; //Виджет, в котором находится данный блок

	void paint_mask(QPainter* painter);
//Функция, необходимая для отрисовки объекта при его перетаскивании.

	void get_color();
//Функция, которая задает цвет данного блока, используя его size.

protected:
	QRectF boundingRect() const;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
//Переопределенные методы QGraphicsItem для корректного Drag and Drop
};

#endif // BLOCK_H
