#ifndef MSCENE_H
#define MSCENE_H
#include <QtWidgets>

class GraphWindow;
class MGraphView;
class Node;
class Edge;
//Класс со сценой
class MScene : public QGraphicsScene
{
	friend class GraphWindow;
	friend class MGraphView;
	friend class Node;
	friend class Edge;
public:
	MScene(QObject* par = 0);
	~MScene();

	void addNode(QPointF position, QString value = "");
//добавить вершину в заданной точке с заданным значением

	void addEdge(Node* source, Node* dest, int val=1);
//добавить ребро к заданным вершинам с заданным весом

	void setDiametr(int d);
//установить диаметр для вершин

	void setOrientated(bool b);
//сделать граф ориентированным/ неориентированным

	void setArrowSize(int d);
//установить размер стрелок

	void setShowEdge(bool a);
//включить/выключить отображение весов ребер

	void setRectShape(int a);

	int diametr() const;
	int arrSize() const {return arrow_size;}
	bool shouldShowValues() const;
	bool isOriented() const;
	int nodeShape() const;

	QList<Node*> * nodesList(); //список всех вершин
	Node* findNode(QString val); //найти вершину по значению
	Edge* findEdge(Node* source,Node* dest); // найти ребро по вершинам
	MGraphView * getMGraph(); //виджет



private:
	void setSizeOfSelectedNodes(int s);
	void setColorOfSelectedNodes(QColor c); //Установка цвета выделенных вершин
	void setShapeOfSelectedNodes(int shape);
	void createGraphWithText(QList<QString> & nodes, QList<QStringList> & child_of_nodes);
//создание графа

	QList<Node*> all_nodes; //все вершины на сцене
	QList<QGraphicsItem*> deleted; //все удаленные со сцены объекты (удаляются позже)
	MGraphView * parent; // виджет

	Node* first;
	QGraphicsItem * second;

	bool show_edge_values; //показывать ли веса ребер
	bool graph_oriented; //ориентированный ли граф

	int node_diametr; //диаметр вершин
	int arrow_size; //размер стрелки
	int node_shape;

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
//Переопределенные методы

private slots:
	void connectTwoNodes();
//соединить две вершины с помощью мыши

public slots:
	void removeSelectedNodes();//удаление выделенных объектов
};

//Q_DECLARE_METATYPE(scena);
#endif // MSCENE_H
