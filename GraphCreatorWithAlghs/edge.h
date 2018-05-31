#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsItem>

class Node;
class MScene;
class InformationDialog;

//Класс для описания рёбер графа
class Edge : public QGraphicsItem
{
	friend class Node;
	friend class MScene;
	friend class InformationDialog;
public:
	Edge(Node *sourceNode, Node *destNode, MScene *sc, int val = 1);
//Конструктор принимает вершину родителя и вершину ребенка, сцену и вес
//по умолчанию 1



	void setColor(QColor col); //установить цвет
	void removeEdge();  //удалить ребро
	void setValue(int n); //установить вес

	Node *sourceNode() const;  //получить вершину-родитель
	Node *destNode() const;  //получить вершину-ребенка
	int getValue() const;   //получить вес ребра


	int type() const Q_DECL_OVERRIDE;
	enum { Type = UserType + 2 }; //Необходимо для определения типа графического объекта

protected:
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QPainterPath shape() const Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
//Переопределенные виртуальные функции.

private:
	void adjust();//Перерисовка линий при перетасквании Node
	void addMidNode(QPointF pos); //Добавить промежуточную вершину
	void setOneArrowAtEdge(bool ori);  //функция для установки количества стрелок
//данное свойство необходимо, чтобы не отрисовывать два ребра в случае <-> стрелки


	Node *source, *dest; //Соединяемые вершины
	QPointF sourcePoint; //Координаты концов линии
	QPointF destPoint;

	int value; //вес
	bool have_one_arrow;  //имеется ли у ребра две стрелки
	QColor main_color; //цвет

	MScene* scene; //сцена для отрисовки ребра
};

#endif // EDGE_H
