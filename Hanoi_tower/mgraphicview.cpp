#include "mgraphicview.h"

MGraphicView::MGraphicView(QWidget *parent)
	:QGraphicsView(parent)
{
	height=0;
	max_bl = 10;
	setAcceptDrops(true);
	setFixedSize(250,325);
	scene = new QGraphicsScene;
	setScene(scene);
	setTransformationAnchor(QGraphicsView::NoAnchor);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	saves = new block*[21];
}

MGraphicView::~MGraphicView(){delete scene; delete[] saves;}

void MGraphicView::create_new_task(int number_of_blocks)  //создание новой конфигурации
{
	height = 0;
	scene->addLine(-125,320,125,320);
	scene->addLine(-125,0,125,0);
	if(number_of_blocks==0) return;
	for(int i = number_of_blocks;i>0;i--)
		add_top_block(i);
}

void MGraphicView::add_top_block(int s)
{
	height++;
	saves[height-1] = new block(s,height,this,max_bl);
	scene->addItem(saves[height-1]);
}

int MGraphicView::remove_top() //удаление блока с освобождением памяти
{
	height--;
	int temp = saves[height]->get_size();
	scene->removeItem(saves[height]);
	delete saves[height];
	return temp;
}
void MGraphicView::remove_top(int a) //удаление без освобождения памяти (необходимо для ручного перетаскивания)
{
	Q_UNUSED(a);
	height--;
	scene->removeItem(saves[height]);
}

void MGraphicView::clear_blocks()
{
	while(height!=0)
		remove_top();
}

void MGraphicView::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasText() &&
		(height==0  || event->mimeData()->text().toInt() <= get_top_block()))
			event->accept();
	else
		event->ignore();
}

void MGraphicView::dragMoveEvent(QDragMoveEvent *event)
{
	if(event->mimeData()->hasText())
		event->accept();
	else
		event->ignore();
}

void MGraphicView::dragLeaveEvent(QDragLeaveEvent *event){Q_UNUSED(event);}

void MGraphicView::dropEvent(QDropEvent *event)
{
	int current_size = event->mimeData()->text().toInt();
	MGraphicView * temp = dynamic_cast<MGraphicView*>(event->mimeData()->parent());
	if (height==0 || saves[height-1]->get_size() > current_size)
	{
		add_top_block(current_size);
		temp->remove_top(1);
	}
	else
		return;
}
