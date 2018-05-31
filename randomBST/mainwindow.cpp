#include "mainwindow.h"
#include "window_with_tree.h"
#include "ui_mainwindow.h"
#include <QShortcut>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	element_for_find = NULL;
	ui->setupUi(this);
	main_tree = NULL;
	valid = new QRegExpValidator (QRegExp("^[0-9 ]+$"), ui->input_field);
	ui->input_field->setValidator(valid);
	ui->el_for_search->setValidator(valid);

	setMinimumSize(414,164);
	setWindowIcon(QIcon("c://tests/1.png"));
	setWindowTitle("Курсовая работа");

	QShortcut *show_key = new QShortcut(this);
	show_key->setKey(Qt::CTRL+Qt::Key_S);
	QShortcut* remove_key = new QShortcut(this);
	remove_key->setKey(Qt::CTRL + Qt::Key_R);
	QShortcut* insert_key = new QShortcut(this);
	insert_key->setKey(Qt::CTRL + Qt::Key_I);
	connect(show_key,SIGNAL(activated()),this,SLOT(show_tree()));
	connect(remove_key,SIGNAL(activated()),this,SLOT(remove_elem()));
	connect(insert_key,SIGNAL(activated()),this,SLOT(insert_elem()));

	connect(ui->show_tree,SIGNAL(clicked(bool)),this,SLOT(show_tree()));
	connect(ui->create_tree,SIGNAL(clicked(bool)),this,SLOT(create_bin_tree()));
	connect(ui->el_for_search,SIGNAL(textChanged(QString)),this, SLOT(find_element()));
	connect(ui->remove_el,SIGNAL(clicked(bool)),this,SLOT(remove_elem()));
	connect(ui->input_field,SIGNAL(textChanged(QString)),this,SLOT(tree_activator()));
	connect(ui->insert,SIGNAL(clicked(bool)),this,SLOT(insert_elem()));

	connect(ui->open, SIGNAL(triggered(bool)),this,SLOT(slotOpen()));
	connect(ui->save, SIGNAL(triggered(bool)),this,SLOT(slotSave()));
	connect(ui->prog_info, SIGNAL(triggered(bool)),this, SLOT(programm_info()));
	connect(ui->about_info, SIGNAL(triggered(bool)),this, SLOT(auth_info()));

}

MainWindow::~MainWindow()
{
	delete ui;
	delete valid;
	node::destroy_bt(main_tree);
}

void MainWindow::tree_activator()
{

	bool ch = check_correct();
	if(str_tree.size() ==0 || !ch)
	{
		node::destroy_bt(main_tree);
		main_tree = NULL;

		ui->show_tree->setDisabled(1);
		ui->remove_el->setDisabled(1);
		ui->insert_status->clear();
		ui->find_remove_status->clear();
		ui->insert->setDisabled(1);
		ui->el_for_search->setDisabled(1);

		ui->tree_status->setText("Дерева в памяти нет!");
		ui->tree_status->setStyleSheet("color: red");
		ui->create_tree->setDisabled(1);
	}
	else
		ui->create_tree->setEnabled(1);
}

bool MainWindow::check_correct()
{
	str_tree.clear();
	str_tree = (ui->input_field->text());
	//str_tree = str_tree.simplified();
	str_tree = " " + str_tree + " ";
	QTextStream main_stream(&str_tree);
	QString temp = "";

	while(!main_stream.atEnd())
	{
		main_stream >> temp;
		if(temp == " ")
			continue;
		temp = " " + temp + " ";
		if(str_tree.count(temp) >1)
			return false;
	}
	str_tree = str_tree.simplified();
	return true;
}

void MainWindow::insert_elem()
{
	QString name = ui->el_for_search->text();
	main_tree = node::insert(main_tree,name.toInt());

	str_tree+= " "+name;
	ui->input_field->setText(str_tree);
	node::create_parents(main_tree,NULL);

	ui->el_for_search->clear();
	ui->insert_status->setText("Элемент "+name+ " добавлен!");
}

void MainWindow::remove_elem()
{
	QString name = QString::number(element_for_find->root);
	main_tree = node::remove_el(main_tree,element_for_find->root);
	node::create_parents(main_tree,NULL);

	str_tree = " " + str_tree + " ";
	str_tree.replace(" "+name+" "," ");
	str_tree = str_tree.simplified();
	ui->input_field->setText(str_tree);

	ui->el_for_search->clear();
	ui->find_remove_status->setText("Элемент "+ name+" удален!");

}

void MainWindow::find_element()
{
	if(ui->el_for_search->text().indexOf(" ")!=-1)
		ui->el_for_search->setText(ui->el_for_search->text().remove(" "));
	if(ui->el_for_search->text().size() == 0)
	{
		ui->find_remove_status->clear();
		ui->remove_el->setDisabled(1);
		ui->insert_status->clear();
		element_for_find = NULL;
		node::clear_path(main_tree);
		return;
	}
	element_for_find = node::find_el(main_tree,(ui->el_for_search->text()).toInt());
	if(element_for_find == NULL)
	{
		ui->find_remove_status->setText("Элемент не найден!");
		ui->find_remove_status->setStyleSheet("color: red");

		ui->insert_status->setStyleSheet("color: green");
		ui->insert_status->setText("Можно добавить");

		ui->insert->setEnabled(1);
		ui->remove_el->setDisabled(1);

		node::clear_path(main_tree);
	}
	else
	{
		ui->find_remove_status->setText("Элемент найден!");
		ui->find_remove_status->setStyleSheet("color: green");
		ui->remove_el->setEnabled(1);
		ui->insert_status->clear();

		ui->insert->setDisabled(1);
		node::clear_path(main_tree);
		node::set_path(element_for_find);
	}
}

void MainWindow::show_tree()
{
	window_with_tree* bin_wind = new window_with_tree;
	bin_wind->set_bt(main_tree);
	bin_wind->show();
}


void MainWindow::create_bin_tree()
{
	node::destroy_bt(main_tree);
	main_tree = NULL;


	QTextStream main_stream(&str_tree);
	QString temp = "";

	while(!main_stream.atEnd())
	{
		main_stream >> temp;
		if(temp == " ")
			continue;
		main_tree = node::insert(main_tree,temp.toInt());
	}
	node::create_parents(main_tree,NULL);

	ui->show_tree->setEnabled(1);
	ui->el_for_search->setEnabled(1);
	ui->tree_status->setText("Дерево загружено!");
	ui->tree_status->setStyleSheet("color: green");
}

void MainWindow::slotOpen()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Text files (*.txt)");
	if (FileName.isEmpty())
		return;

	QFile File(FileName);

	if(File.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		ui->input_field->setText(File.readAll());
		File.close();
	}
	else
		QMessageBox::warning(this, "Error", QString("Не смог открыть файл %1").arg(FileName),QMessageBox::Ok);
}

void MainWindow::slotSave()
{
	if(!main_tree)
		return;
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath(),
													tr("Text Files (*.txt)"));
	if (FileName.isEmpty())
		return;
	QString out = "";
	node::print_bt(main_tree,out);
	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(out);
		File.write(Data);
		File.close();
	}
}

void MainWindow::programm_info()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4>Данная программа по заданной последовательности строит рандомизированное дерево поиска."
												  "<br>В программе реализовано добавление новых элементов, поиск и удаление раннее добавленных элементов."
												  "<br>Также в программе предусмотрена графическая демонстрация построенного дерева и отображение алгоритма поиска."
												  "</font>"));
}

void MainWindow::auth_info()
{
	QMessageBox::about(this,tr("Об авторе"), tr("<font size = 4>Прогрмму написал:<br>Васильев А.А.<br>Группа: 6304</font>"));

}
