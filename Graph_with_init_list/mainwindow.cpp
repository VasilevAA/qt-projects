#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle("Построение графа");
	connect(ui->show_tree,SIGNAL(clicked(bool)),this,SLOT(button_triggered()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_triggered()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Text files (*.txt)");
	if (FileName.isEmpty())
		return;

	QFile File(FileName);

	if(File.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		ui->input->setText(File.readAll());
		File.close();
	}
	else
		QMessageBox::warning(this, "Error", QString("Не смог открыть файл %1").arg(FileName),QMessageBox::Ok);
}

void MainWindow::add_spaces_and_simplifie()
{
	str_for_work = ui->input->toPlainText();
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

}

void MainWindow::button_triggered()
{
	add_spaces_and_simplifie();
	str_for_work = str_for_work.trimmed();
	QTextStream main_stream(&str_for_work);
	int num = 1;
	graph.clear();
	while(!main_stream.atEnd())
	{
		QString temp_line = main_stream.readLine();

		if(!temp_line.isEmpty())
		{
			temp_line.append(",");
			Vertex* b = new Vertex(temp_line.left(temp_line.indexOf(" :")),num);
			graph.push_back(b);
			num++;
		}
	}

	main_stream.flush();
	main_stream.setString(&str_for_work);
	num = 0;
	while(!main_stream.atEnd())
	{
		QString temp_line = main_stream.readLine();
		temp_line = temp_line.simplified();
		if(temp_line[temp_line.size()-1] == ','
				|| temp_line == ":"
				|| temp_line.indexOf(", ,")!=-1)
			return;
		QString temp_vert = "";
		if(!temp_line.isEmpty())
		{
			temp_line.append(" ,");
			temp_line = temp_line.right(temp_line.size()- temp_line.indexOf(": ")-1).trimmed();
			if(temp_line == ",")
			{	num++;
				continue;
			}
			int c = 0;
			while ( ( c = temp_line.indexOf(",") )!= -1)
			{
				   temp_vert = temp_line.left(c).trimmed();
				   if(temp_line.isEmpty())
					   return;
				   set_vert_children(num,temp_vert);
				   temp_line = temp_line.remove(0, c+1).trimmed();
			}
			num++;
		}

	}


	Form* gr = new Form;
	gr->set_form(graph);
	gr->show();

}


void MainWindow::set_vert_children(int number, QString name)
{
	for(int i = 0; i < graph.size();i++)
	{
		if(graph[i]->name == name)
		{
			graph[number]->children.push_back(graph[i]);
			return;
		}
	}
	QMessageBox::warning(this,"Предупреждение!","<font size = 4>"
												"Вершина &lt;" + name +
												"> тупиковая. Она требует инициализации в отдельной строке. "
												"</font>");
	return;
}


void MainWindow::on_action_2_triggered()
{

	QMessageBox::about(this,tr("Синтаксис ввода"), tr("<font size = 4>Данная программа по входному списку инцидентности строит граф."
												  "<br><br>Синтаксис ввода списка:"
												  "<br>&lt;Родитель> : &lt;ребенок 1>, ... , &lt;ребенок n>"
													"<br> &lt;Вершина без детей (конечная)> : "
												  "</font>"));
}

void MainWindow::on_action_3_triggered()
{
	Form* gr = new Form;
	gr->set_form(graph);
	gr->show();
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath()+ "/picture",
													tr("Изображение (*.png)"));

	if(FileName.isEmpty())
	{
		return;
		gr->close();
	}


	gr->grab().save(FileName);
	gr->close();
}
