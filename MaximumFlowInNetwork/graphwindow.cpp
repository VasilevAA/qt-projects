#include "graphwindow.h"
#include "ui_graphwindow.h"

#include "mscene.h"
#include "node.h"
#include "edge.h"
#include "informationdialog.h"


//окно с графом
GraphWindow::GraphWindow(QWidget *parent) :
	QWidget(parent),
	processing(false),msec_delay(800),ui(new Ui::GraphWindow)
{

	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true); //удалять окно при его закрытии
	readSettings(); //прочитать настройки с геометрией окна

//так как у QWidget отсутствует слот для меню, создаем меню отдельно
//и добавим его в компановку на виджете

	//создание меню
	QMenu * file =  new QMenu("Файл");
		file->addAction("Сохранить как текст (.txt)", this, SLOT(saveTextGraph()),QKeySequence("Ctrl+S"));
		file->addAction("Сохранить как картинку (.png)",this,SLOT(savePicture()),QKeySequence("Ctrl+P"));
		file->addAction("Сохранить как форматированный граф (.grph)",this,SLOT(saveGraphWithFormat()),QKeySequence("Ctrl+Shift+S"));
			file->addSeparator();
		file->addAction("Закрыть",this,SLOT(close()));

	QMenu * hints = new QMenu("Правка");
		hints->addAction("Выделить все", this, SLOT(selectAll()),QKeySequence("Ctrl+A"));
		hints->addSeparator();
		QAction * a  = hints->addAction("Копировать");
			a->setShortcut(QKeySequence("Ctrl+C"));
		//удобней соединить слот прямо в addAction, но для этого нужен новый синтаксис
		//который введен в 5.6. Поэтому соединим простым connect с лямбда-функцией
		//которая вызывает метод из сцены.
			connect(a,&QAction::triggered,[this](){getMscene()->copySelectedNodes();});

		QAction * b = hints->addAction("Вставить");
			b->setShortcut(QKeySequence("Ctrl+V"));
			connect(b,&QAction::triggered,[this](){getMscene()->paste();});
		hints->addSeparator();
		QAction * c = hints->addAction("Назад");
			c->setShortcut(QKeySequence("Ctrl+Z"));
			connect(c,&QAction::triggered,getMscene(),&MScene::undo);
	QMenu * graph = new QMenu("Граф");
		graph->addAction("Текстовое представление",this, SLOT(showTextGraph()));
		graph->addAction("Перевести в неориентированный",this,SLOT(convertToUnoriented()));
		graph->addSeparator();
		graph->addAction("Очистить все",this, SLOT(cleanGraph()));

	QMenu * info = new QMenu("Справка");
		info->addAction("Создание графа",this,SLOT(showCreateHints()));
		info->addAction("Максимальный поток",this,SLOT(showTokHints()));

	QMenu * alg = new QMenu("Алгоритмы");
		alg->addAction("Максимальный поток",this,SLOT(maxFlowInit()));
		alg->addAction("Очистить поток",this,SLOT(stopAnim()));

	QMenuBar * menuBar = new QMenuBar; //строка с меню
		menuBar->addMenu(file); //добавляем созданные меню в строку с меню
		menuBar->addMenu(hints);
		menuBar->addMenu(graph);
		menuBar->addMenu(info);
		menuBar->addMenu(alg);

	ui->gridLayout->setMenuBar(menuBar); //устанавливаем у компоновки
										//нашего окна меню бар
	ui->main_graph->setParent(this);

	setWindowTitle("Редактор графов");
	setWindowIcon(QIcon(":/icons/graph.png"));
}

GraphWindow::~GraphWindow()
{
	delete ui;
}

void GraphWindow::stopAnim()
{

	foreach (QGraphicsItem * it, getMscene()->items()) {
		if(it->type() == Edge::Type)
		{
			//для каждого ребра остановить анимацию и обнулить текущий поток
			((Edge*)it)->setAnimating(false);
			((Edge*)it)->setCurrent(-1);
		}
	}
	clearFormat();
}

bool GraphWindow::getProcessing() const
{
	return processing;
}

void GraphWindow::selectAll()
{
	foreach (QGraphicsItem * node, getMscene()->items()) {
		node->setSelected(true); //выделить каждый элемент
	}
}

void GraphWindow::savePicture() //сохранение как .png картинки
{
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
													tr("Изображение (*.png)"));
	if(FileName.isEmpty())
		return;
	if(ui->main_graph->getMscene()->nodesList()->isEmpty())
		return;
	QImage image(ui->main_graph->sceneRect().size().toSize(),QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);

	painter.setRenderHint(QPainter::Antialiasing);
	ui->main_graph->getMscene()->render(&painter);
	image.save(FileName);
}


void GraphWindow::showTextGraph()
{
	QString a;
	foreach (Node* node, *(ui->main_graph->getMscene()->nodesList())){
		a << *node;
		a.append("\n");
	}

	InformationDialog * out = new InformationDialog(a,this);
	out->show(); //вывод строки со списком инцидентности

}


void GraphWindow::saveTextGraph() //сохранение в текстовом виде
{
	QString a;
	foreach (Node* node, *(ui->main_graph->getMscene()->nodesList())){
		a << *node;
		a.append("\n");}

	if(a.size() == 0)
		return;

	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
													tr("text(*.txt)"));
	if (FileName.isEmpty())
		return;

	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(a);
		File.write(Data);
		File.close();
	}
}

void GraphWindow::saveGraphWithFormat() //сохранение файла с форматированием (цвет, позиция, значения вершин и пропускных способностей)
{
	QString a;
	//заполнение строки основной информацией
	foreach (Node* node, *(ui->main_graph->getMscene()->nodesList())){
		a << *node;
		a.append(" &%& " + QString::number(node->scenePos().x()) +
				 " " + QString::number(node->scenePos().y())
				 +" &%& " + QString::number(node->getColor().red()) +
				 " " + QString::number(node->getColor().green()) +
				 " " + QString::number(node->getColor().blue())

				 +" &%& ");
		foreach(Node* ch,*node->children())
		{
			Edge * e = getMscene()->findEdge(node,ch);
			a.append(e
						? QString::number(e->getValue()) + " "
						: "0 ");

		}
		a.append("\n");
	}

	if(a.size() == 0)
		return;

	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),readPathSettings(),
													tr("Graph File(*.grph)"));
	if (FileName.isEmpty())
		return;

	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(a);
		File.write(Data);
		File.close();
	}
}

MScene* GraphWindow::getMscene(){
	return ui->main_graph->getMscene();
}

void GraphWindow::cleanGraph() //очистить граф
{
	//удалить все вершины (все связанные ребра удалятся тоже)
	foreach (Node* node, *(getMscene()->nodesList()))
		node->remove();

}

bool GraphWindow::createNewGraphWithFormat(QString file_name)
{ //форматированный граф
	color_of_nodes.clear();
	values_of_edges.clear();
	points_of_nodes.clear();

	QString temp;
	QFile File(file_name);
	if(File.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		temp = File.readAll();
		File.close();
	}
	 else
		return false;

	QString for_next;

	QTextStream stream(&temp);
	qreal a,b, c;
	while(!stream.atEnd())
	{
		QString temp2 =add_spaces_and_simplifie( stream.readLine(220));
		if (temp2.isEmpty())
			continue;
		QStringList list1 = temp2.split("&%&");
		for_next.append(list1.first().trimmed() + "\n");

		QTextStream temp3(&list1[1]);
		temp3 >> a >> b;
		points_of_nodes << QPointF(a,b);

		temp3.setString(&list1[2]);
		temp3 >> a >> b >> c;
		color_of_nodes << QColor(a,b,c);

		QString ee = list1[3].trimmed();
		values_of_edges << ee.split(" ");
		QString ss;
		foreach (QString a, values_of_edges.back()) {
			ss.append(a + " ");
		}

	}
	if(!createNewGraph(for_next))
		return false;
	setFormat();
	return true;

}

void GraphWindow::setFormat()
{//установить форматирование
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * node = getMscene()->nodesList()->at(i);
		node->setColor(color_of_nodes[i]);
		node->setPos(points_of_nodes[i]);
		int k =0;
		foreach (Node * ch, *node->children()) {
			Edge * e = getMscene()->findEdge(node,ch);
			if(e)
				e->setValue(values_of_edges[i].at(k).toInt());
			k++;
		}
		node->update();
	}
}

bool GraphWindow::createNewGraph(QString temp)
{
	nodes.clear();
	child_of_nodes.clear();
	cleanGraph();
	temp  = temp.trimmed();
	QTextStream stream(&temp);
	while(!stream.atEnd())
	{
		QString line = add_spaces_and_simplifie(stream.readLine(220));
		if(line.isEmpty()) continue;

		QStringList list = line.split(":");
		if(list.first() == list.last()) return false;

		QString t = list.first().trimmed();
		nodes << t;

		QString t2 = list.last().trimmed();
		QStringList list_of_children = t2.split(" , ",QString::SkipEmptyParts);

		child_of_nodes << list_of_children;
	}
	ui->main_graph->getMscene()->createGraphWithText(nodes,child_of_nodes);
	return true;
}

QString GraphWindow::add_spaces_and_simplifie(QString str_for_work)
{
	QChar symbs[2]{':',','};
	for(int i = 0; i < 2; i++)
	{
		int a = -2;
		while((a = str_for_work.indexOf(symbs[i],a+2))!=-1)
		{
			{
				str_for_work.insert(a, ' ');
				str_for_work.insert(a+2, ' ');
			}
		}
	}
	str_for_work = str_for_work.simplified();
	return str_for_work;
}

void GraphWindow::showCreateHints()
{
//показать подскаку по созданию графа
	InformationDialog* brows = new InformationDialog(":/help","graph_creation.htm");
	brows->show();
}

void GraphWindow::showTokHints()
{
//подсказка по алгоритму
	InformationDialog *brows = new InformationDialog(":/help","tok.htm");
	brows->show();
}


void GraphWindow::convertToUnoriented()
{
	//создать ребро в другую сторону
	foreach (Node * node, *getMscene()->nodesList()) {
		foreach(Node * child_node,*node->children())
			if(!Node::is_two_nodes_connected(child_node,node))
				getMscene()->addItem(new Edge(child_node, node,getMscene()));
	}
}

void GraphWindow::clearFormat()
{
//очистить форматирование (цвета и стиль линий)
	foreach (QGraphicsItem * it, getMscene()->items()) {
			if(it->type() == Node::Type)
				((Node*)it)->setColor(Qt::white);

			if(it->type() == Edge::Type)
			{
				((Edge*)it)->setColor(Qt::black);
				((Edge*)it)->setStyle(Qt::SolidLine);
				((Edge*)it)->setZValue(-1);
			}
	}
}

void GraphWindow::closeEvent(QCloseEvent * event)
{
	if (processing)
	{
//если работает алгоритм - спросить, завершить ли его
		int b = QMessageBox::question(this,"?","Алгоритм не завершил работу. Завершить принудительно?",QMessageBox::Yes,QMessageBox::No);
		if(b == QMessageBox::Yes)
		{
			processing = false;
			paused = false;
			msec_delay = 1;
		}
		event->ignore();
		return;
	}
//если алгоритм не работает - очистить анимацию и дать время на удаление.
	hide();
	stopAnim();
	writeSettings();
	MScene::setDelay(1000);

	event->accept();

}

void GraphWindow::maxFlowInit()
{
	stopAnim(); //остановка всей анимации

	size = getMscene()->nodesList()->size();
	if(size<=1)
		return;

	int st = getMscene()->nodesList()->indexOf(getMscene()->findNode("S"));
	if(st==-1) //нет истока
	{
		QMessageBox::information(this,"!","Нет обозначенного истока S",QMessageBox::Ok);
		return;
	}

	int fin = getMscene()->nodesList()->indexOf(getMscene()->findNode("T"));
	if(fin==-1) //нет стока
	{
		QMessageBox::information(this,"!","Нет обозначенного стока T",QMessageBox::Ok);
		return;
	}

	(*getMscene()->nodesList())[st]->setColor(QColor(180,0,0));
	(*getMscene()->nodesList())[fin]->setColor(QColor(240,0,0));

	ui->gridLayout->menuBar()->setDisabled(true); //отключить меню
	msec_delay=800; //задержка на стандартные 800 мсек.
	processing = true; //алгоритм работает
	paused = true;

	//виджет для управления алгоритмом
	QWidget * setup_widget = new QWidget(this);

	 //создание элементов управления
		setup_widget->setAttribute(Qt::WA_DeleteOnClose);
		setup_widget->setWindowModality(Qt::NonModal);

		setup_widget->setGeometry(5,45,150,80);

		//создание слайдера для регулировки скорости
			QSlider * slider = new QSlider(Qt::Horizontal,setup_widget);

			slider->setRange(0,2000);
			slider->setValue(msec_delay);
			slider->setTickInterval(200);

			connect(slider,&QSlider::valueChanged,this,[=](int val){msec_delay =  val;});

		 //создание кнопки стоп для остановки алгоритма
			QPushButton *stop_but = new QPushButton("Стоп!",setup_widget);
			stop_but->setFixedWidth(50);
			connect(stop_but,&QPushButton::clicked,this,&GraphWindow::close);

			QPushButton * pause_play_but = new QPushButton(setup_widget);
			pause_play_but->setIcon(QIcon(":/player/play.png"));
			pause_play_but->setCheckable(true);

			QPushButton * one_step_but = new QPushButton(setup_widget);
			one_step_but->setIcon(QIcon(":/player/forward.png"));
			one_step_but->setToolTip("Следующий шаг");

			connect(one_step_but,&QPushButton::clicked,[=]() //лямбда для одного шага
												{
													msec_delay = 1;
													paused = false;
													QTimer::singleShot(1,[=](){paused = true; msec_delay = slider->value();});
												});

			connect(pause_play_but,&QPushButton::toggled,[=](bool ch) //лямбда переключения плеера
												{
													paused = !paused;
													one_step_but->setEnabled(ch ? 0 : 1);
													pause_play_but->setIcon(ch ? QIcon(":/player/pause.png") : QIcon(":/player/play.png"));
													pause_play_but->setToolTip(!ch ?  "Продолжить" :"Остановить");
												});

			QHBoxLayout * hl = new QHBoxLayout;
			hl->setMargin(0);
			hl->addWidget(stop_but);
			hl->addWidget(pause_play_but);
			hl->addWidget(one_step_but);

		QVBoxLayout * vl = new QVBoxLayout(setup_widget);
		vl->addLayout(hl);
		vl->addWidget(slider);
		vl->setMargin(0);
		setup_widget->setLayout(vl);

	setup_widget->show();

	capacity = new int *[size];
	for(int i = 0; i < size; i++)
		capacity[i] = new int[size];

	flow = new int *[size];
	for(int i = 0; i < size; i++)
		flow[i] = new int[size];

	for(int i =0; i < size; i++)
	{
		for(int j =0 ; j < size; j++)
		{
			capacity[i][j] = 0;
			flow[i][j] = 0;
		}
	}

	pred = new int[size];

	//заполнение матрицы пропускных способностей
	foreach (Node* n, *getMscene()->nodesList()) {
		int a = getMscene()->nodesList()->indexOf(n);
		foreach (Node* z, *getMscene()->nodesList()) {
			int b = getMscene()->nodesList()->indexOf(z);
			Edge* e = getMscene()->findEdge(n,z);
			if(e)
				capacity[a][b] = e->getValue();
		}
	}

	int m_flow= max_flow(st,fin); //вычисление макс. потока

	if(processing) //если алгоритм не был прерван
	{

		QMessageBox::information(this,"Максимальный поток","Максимальный поток в сток Т - " + QString::number(m_flow),QMessageBox::Ok);
		processing = false;
		foreach (QGraphicsItem * it, getMscene()->items()) {
			if(it->type() == Edge::Type)
			{
				Edge* tmp = ((Edge*)it);
				if(tmp->getCurrent() == 0) //если через ребро нет потока
				{	//окрасить в серый пунктирной линией
					tmp->setStyle(Qt::DotLine);
					tmp->setColor(Qt::lightGray);
					tmp->setZValue(-4);
				}
			}
		}

	}
	else //если алгоритм прерван
	{
			QMessageBox::information(this,"!","Сток в вершину T не найден",QMessageBox::Ok);
			stopAnim();
	}
	setup_widget->close();

	ui->gridLayout->menuBar()->setDisabled(false);

	for(int i = 0; i < size; i++)
		delete capacity[i];
	delete capacity;

	for(int i = 0; i < size; i++)
		delete flow[i];
	delete flow;

	delete pred;
}

bool GraphWindow::bfs(int start,int end)
{
	QVector<bool> is_visited(size);
	QVector<int> qq;

	qq.push_back(start);
	is_visited[start] = true;
	pred[start] = -1;   // Специальная метка для начала пути

	while(!qq.isEmpty() && processing)  // Пока хвост не совпадёт с головой
	{
		// вершина u пройдена
		int u = qq.first();
		qq.pop_front();
		is_visited[u] = true;
		for(int v = 0; v < size; v++ ) // Смотрим смежные вершины
		{
			// Если не пройдена и не заполнена
			if(!is_visited[v] && capacity[u][v] > flow[u][v]) {
				qq.push_back(v);
				is_visited[v] = true;
				pred[v] = u; // Путь обновляем
			}
		}
	}

	if(is_visited[end]) // Если конечная вершина, дошли - возвращаем 0
	  return true;
	else
		return false;
}

int GraphWindow::max_flow(int source, int stock)
{
	int maxflow=0;            // Изначально нулевой
	while(bfs(source,stock) && processing)             // Пока сеществует путь
	{
		int delta = 10000000;
		for(int u = stock; pred[u] >= 0 && processing; u = pred[u]) // Найти минимальный поток в пути
			delta=qMin(delta, ( capacity[pred[u]][u] - flow[pred[u]][u] ) );

		for(int u = stock; pred[u] >= 0 && processing; u=pred[u]) // По алгоритму Форда-Фалкерсона
		{


			Node * a = (*getMscene()->nodesList())[pred[u]];
			Node * b = (*getMscene()->nodesList())[u];
			Edge * e = getMscene()->findEdge(a,b);

			flow[pred[u]][u] += delta;
			flow[u][pred[u]] -= delta;

			{//Графическое отображение

				//окрасить задействованные вершины
				if(pred[u] != source && pred[u] != stock)
						a->setColor(QColor(255,50,50));
				  if(u != source && u != stock)
						b->setColor(QColor(255,50,50));

				  e->setColor(QColor(180,0,0));//окрасить ребро
				  if(!e->isAnimating()) //анимировать
					  e->setAnimating(true);
				  e->setSelected(true); //выделить (жирным)

				if(!e->setCurrent(flow[pred[u]][u])) //прервать алгоритм
					processing = false;				//при отриц. потоке


				while(paused) //пауза
					QApplication::processEvents();

					MScene::setDelay(msec_delay); //задержка между шагами


				e->setSelected(false);  //снять выделение
				e->setColor(Qt::black); //вернуть цвет ребру

				if(pred[u] != source && pred[u] != stock) //вернуть цвет вершинам
						a->setColor(Qt::white);
				if(u != source && u != stock)
						b->setColor(Qt::white);
			}
		}
		maxflow += delta;                       // Повышаем максимальный поток
	}
	return maxflow;
}

void GraphWindow::writePathSettings(QString str)
{
	QFileInfo inf(str);
	QString b = inf.absoluteDir().absolutePath();
	QSettings settings("LETI", "Graph application");
	settings.setValue("path",b);
}

QString GraphWindow::readPathSettings()
{
	QSettings settings("LETI", "Graph application");
	QString str = settings.value("path").toString();
	return str;
}

void GraphWindow::writeSettings()
{
	QSettings settings("LETI", "Graph application");
	settings.setValue("geometry",saveGeometry());
}

//установка настроек
void GraphWindow::readSettings()
{
	QSettings settings("LETI", "Graph application");
	restoreGeometry(settings.value("geometry").toByteArray());
}

