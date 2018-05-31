#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>


class Edge;
class MGraphView;
class MScene;

//Класс для определения вершин графа
class Node : public QGraphicsItem
{
	friend class Edge;
	friend class MScene;
	friend QString& operator<<(QString & str, const Node & node);
//оператор вывода вершины в строку
public:
	Node(MScene *graphWidget,  QString val = "");
//конструктор вершины с заданным значением

	void setColor(QColor col);//установить цвет
	void setValue(QString val);//установить значение
	void removeNode(); //удалить данную вершину
	void setLoop();  //установить петлю
	void removeLoop();//удалить петлю
	void setShape(int shape);
	void setDiametr(int diam);

	QString get_val() const;//извлечение значения
	QColor get_color() const;

	QList<Edge *> * edges(); //списки ребер и детей
	QList<Node *> * children();

	static bool is_two_nodes_connected(Node* src, Node* dst);
//соединены ли две вершины

	enum { Type = UserType + 1 }; //Тип "вершина"
	int nodeShape() const;
	int Diametr() const;
	int type() const Q_DECL_OVERRIDE;
	enum {Circle =0, Rhomb, Rectangle};
protected:
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QPainterPath shape() const Q_DECL_OVERRIDE;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
	//переопределенные методы

private:
	void addChild(Node * child); //Добавить ребенка к вершине
	void addEdge(Edge *edge); //Добавить ребро


	QList<Edge *> edgeList; //Список ребер
	QList<Node *> childrenList; //Смежные вершины

	MScene * scene;   //сцена в которых находится вершина

	QColor main_color; //цвет
	QString value;	 //значение
	int shape_type;
	int diametr;
};

#endif // NODE_H
