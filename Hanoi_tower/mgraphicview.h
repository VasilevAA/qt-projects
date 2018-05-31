//Класс, описывающий сцену (всего 3 сцены), на которой
//будет отображаться пирамида из блоков block.

#ifndef MGRAPHICVIEW_H
#define MGRAPHICVIEW_H
#include <QtWidgets>
#include "block.h"

class block;

class MGraphicView : public QGraphicsView
{
public:
	MGraphicView(QWidget * parent = 0);
	~MGraphicView();

	void create_new_task(int number_of_blocks);
//Функция, инициализирующая пирамиду на данной сцене:
//создает массив блоков. number_of_blocks - кол-во блоков.

	void clear_blocks();
//Функция, очищающая сцену от нарисованных блоков.

	void set_max_block(int m) { max_bl = m;}
//Установка максимального количества блоков в сцене.

	int get_height(){return height;}
//Возвращает высоту пирамиды данной сцены

	int get_top_block(){return (height!=0)? saves[height-1]->get_size():0;}
//Функция возвращает размер верхнего блока в пирамиде (или ноль, если блоков нет).

	void add_top_block(int s);
//Функция добавляющая сверху один блок размером s

	int remove_top();
	void remove_top(int a);
//Функция удаляющая самый верхний блок.


private:
	QGraphicsScene * scene;	//Сцена
	block ** saves;	//Массив блоков
	int max_bl;		//Макс. кол-во блоков в сцене
	int height;		//Высота пирамиды

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);

//Переопределенные функции, для работы Drag and Drop

};

#endif // MGRAPHICVIEW_H
