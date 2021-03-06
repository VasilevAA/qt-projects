#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include "graphwindow.h"
#include "helpbrowser.h"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("Работа с графами");
	setWindowIcon(QIcon(":/icons/main.png"));
	window_with_graph = new GraphWindow;
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_pushButton_clicked()
{
		window_with_graph = new GraphWindow;
		if(!window_with_graph->createNewGraph(ui->plainTextEdit->toPlainText()))
			return;

		window_with_graph->resize(600,600);
		window_with_graph->show();
}

void MainWindow::on_create_graph_triggered()
{;
	window_with_graph = new GraphWindow;
	window_with_graph->show();
}

void MainWindow::on_open_graph_triggered()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...",window_with_graph->readPathSettings(), "Text files (*.txt)");
	if (FileName.isEmpty())
		return;

	window_with_graph->writePathSettings(FileName);
	QFile File(FileName);

	if(File.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		ui->plainTextEdit->setPlainText(File.readAll());
		File.close();
	}
}



void MainWindow::on_open_format_graph_triggered()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", window_with_graph->readPathSettings(), "graph file (*.grph)");
	if (FileName.isEmpty())
		return;

	window_with_graph->writePathSettings(FileName);

	window_with_graph = new GraphWindow;
	window_with_graph->createNewGraphWithFormat(FileName);
	window_with_graph->show();
}

void MainWindow::on_auth_info_triggered()
{
	QMessageBox::about(this,tr("Об авторе"), tr("<font size = 4 >Прогрмму написал:<br>Васильев А.А."
												"<br><br>СПбГЭТУ ЛЭТИ,"
												"<br>Группа: 6304"
												"<br><br>Email: the-drus@yandex.ru</font>"));
}

void MainWindow::on_program_info_triggered()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4 >Программа предназначена для работы с графами"
												"<br><br>"
												  "Для создания и редактирования графа открывается новое окно, в которм и будет производиться дальнейшая "
												  "работа с графом. Пользователь может создать граф тремя разным способами: создать пустой граф, открыть простой граф из списка или открыть граф с форматированием."
												"</font>"));
}

void MainWindow::on_action_3_triggered()
{
	InformationDialog brows(":/help","save_open_menu.htm");
	brows.exec();
}

void MainWindow::on_action_4_triggered()
{
	InformationDialog brows(":/help","syntax_hints.htm");
	brows.exec();
}

void MainWindow::on_action_2_triggered()
{
	InformationDialog brows(":/help","graph_creation.htm");
	brows.exec();
}


