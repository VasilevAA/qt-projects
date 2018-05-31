#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
#include <QWidget>
#include "mscene.h"
#include "helpbrowser.h"
#include "node.h"
#include "edge.h"
#include <QtWidgets>
class MScene;


struct tbl_wid
{
	int column,raw;
	QString value;
	tbl_wid(int a, int b, QString c)
		:column(a),raw(b),value(c)
	{}
};
namespace Ui {
class GraphWindow;
}

//класс для описания окна графа
class GraphWindow : public QWidget
{
	Q_OBJECT
public:
	explicit GraphWindow(QWidget *parent = 0);
	~GraphWindow();
	MScene * getMscene(); //получить сцену

	bool createNewGraph(QString temp); //создать граф из строки temp
	bool createNewGraphWithFormat(QString file_name); //создать граф с форматом
//из файла file_name

	void writePathSettings(QString str);
	QString readPathSettings();
private:
	QList<QString> nodes;
	QList<QStringList> child_of_nodes;
	QList<QPointF> points_of_nodes;
	QList<QColor> color_of_nodes;
	QList<int> shapes_of_nodes;
	QList<int> diametrs_of_nodes;
//значения полученные после отработки функций для создания графов
//передаются сцене.

	Ui::GraphWindow *ui;

	bool processing; //работает ли какой-либо алгоритм
	int msec_delay; //задержка между шагами алгоритма

	QString add_spaces_and_simplifie(QString str_for_work);
//обработка строк текста для дальнейшего использования

	void setFormat();
//установить форматирование

	void clearFormat();
//очистить форматирование

	void writeSettings(); //сохранение и чтение
	void readSettings();  //настроек окна

	void generate(int k,QString& res);
	QVector<QString> m_nodes;
	bool gamPyt();
	int size;

	FloydHelp * floyd_brows;

private slots:
	void savePicture(); //слоты для сохранения
	void saveTextGraph();//в различных форматах
	void saveGraphWithFormat();


	void cleanGraph(); //очистить граф
	void selectAll(); //выделить весь граф

	void showValues(bool a); //отображение весов ребер
	void setDelay(int a);  //установка задержки

	void advSet(); //отображение настроек
	void showCreateHints(); //вызов справки
	void showTextGraph(); //отобразить текстовое представление
	void setRectShape();
	void changeToolBut(int a);

	void wide_search(); //алгоритмы для работы с графами
	void deep_search();
	void floyd_alg();
		void get_tbl_wid(QString str);
		void floyd_final();
	//	void floyad_fin();

	void hamilton_alg();
protected:
	void closeEvent(QCloseEvent * event);
//обработка события закрытия окна

};


void set_delay(int msec);



class FloydCalc : public QObject
{
	Q_OBJECT
private:
	int size;

	QTableWidget * tbl;

	MScene * scene;

	InformationDialog * help;

signals:
	void finished();
	void setInt(int a);

	void sendInfo(QString a);
public:
	FloydCalc(MScene * sc)
		:scene(sc)
	{

		size = scene->nodesList()->size();
		//connect(this,&FloydCalc::finished,this,&FloydCalc::showHelp);
	}



public slots:
	QTableWidget * getWindow() {return tbl;}

	void showHelp()
	{
		emit finished();
	}


	void calculation()
	{


		int ** matrix;

		int INF = 1000000;
		matrix = new int*[size];


	//создание матрицы смежности
		for(int i = 0; i < size ; i++)
			matrix[i] = new int[size];

		for(int i = 0; i < size ; i++)
		{
			for(int j = 0; j < size; j++)
				matrix[i][j] = INF;
			matrix[i][i] = 0;
		}

		foreach (Node* node, *scene->nodesList()) {
			int a = scene->nodesList()->indexOf(node);
			foreach (Node* ch, *node->children()) {
				int b = scene->nodesList()->indexOf(ch);
				Edge* e= scene->findEdge(node,ch);
				matrix[a][b] =  e->getValue();
			}
		}

	//создание окна с прогрессом и кнопкой "стоп

	//алгоритм

		for(int k = 0; k < size; ++k)
		{
			for(int i = 0; i <size; ++i)
			{

				for(int j = 0; j < size; ++j)
				{
					matrix[i][j] = qMin(matrix[i][j],matrix[i][k] + matrix[k][j]);

				}
			}
		}


		for(int i = 0; i <size ; i++)
		{
			for(int j = 0; j < size; j++)
			{
				QString str((matrix[i][j]!=INF)? QString::number(matrix[i][j]) : "∞");

				QString a(QString::number(i) + "|" + QString::number(j) + "|" + str);

				emit sendInfo(a);
			}
		}

		for(int i = 0; i < size; i++)
			delete [] matrix[i];
		delete [] matrix;
		emit finished();
	}

};

#endif // GRAPHWINDOW_H
