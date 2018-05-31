#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
namespace Ui {
class MainWindow;
}

class Vertex
{
public:
	Vertex(){};
	Vertex(QString a,int b){
		name = a;
		number = b;}

	void set_child(Vertex* a){
		children.push_back(a);}

	void set_point(QPoint a){
		point = a;}


	QString name;
	int number;
	QVector<Vertex*> children;
	int level;
	QPoint point;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_action_triggered();
	void button_triggered();


	void on_action_2_triggered();

	void on_action_3_triggered();

private:
	QVector<Vertex*> graph;

	QString str_for_work;
	QVector<QString> ver;

	Ui::MainWindow *ui;


	void add_spaces_and_simplifie();

	void set_vert_children(int,QString);

};



#endif // MAINWINDOW_H
