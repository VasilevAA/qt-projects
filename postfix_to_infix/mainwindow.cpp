#include "mainwindow.h"
#include "window_with_tree.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	main_tree = NULL;
	valid = new QRegExpValidator (QRegExp("^[а-яА-ЯёЁa-z0-9A-Z+' '*-_]+$"), ui->input_field);
	ui->input_field->setValidator(valid);
	setWindowIcon(QIcon("c://tests/1.png"));
	setWindowTitle("Лабораторная работа №4");
	connect(ui->input_field,SIGNAL(textChanged(QString)),this,SLOT(button_activator()));
	connect(ui->show_tree,SIGNAL(clicked(bool)),this,SLOT(show_tree()));
	connect(ui->create_infx,SIGNAL(clicked(bool)),this,SLOT(create_bin_tree()));

	connect(ui->open, SIGNAL(triggered(bool)),this,SLOT(slotOpen()));
	connect(ui->save, SIGNAL(triggered(bool)),this,SLOT(slotSave()));
	connect(ui->prog_info, SIGNAL(triggered(bool)),this, SLOT(programm_info()));
	connect(ui->about_info, SIGNAL(triggered(bool)),this, SLOT(auth_info()));

}


MainWindow::~MainWindow()
{
	delete ui;
	node::destroy_bt(main_tree);
}

void MainWindow::button_activator()
{
	check_correction();
	if(pstfx.isEmpty())
	{
		ui->output_field->clear();
		ui->show_tree->setDisabled(1);
	}
	if(correction)
		ui->create_infx->setEnabled(1);
	else
		ui->create_infx->setDisabled(1);
}
void MainWindow::show_tree()
{
	window_with_tree* bin_wind = new window_with_tree;
	bin_wind->set_bt(main_tree);
	bin_wind->show();
}

void MainWindow::check_correction()
{
	correction = 1;
	QStack<QString> c_stack;
	QString cor_str = "";
	QString temp = "";

	pstfx.clear();
	pstfx = ui->input_field->text();
	add_spaces_and_simplifie();

	QTextStream c_stream(&pstfx);
	while(!c_stream.atEnd())
	{
		c_stream >> temp;
		if(QString("+*-/").indexOf(temp)!= -1)
		{
			if(c_stack.size()<2)
			{
				correction = 0;
				return;
			}
			cor_str+=c_stack.pop();
			cor_str+=c_stack.pop();
			c_stack.push(cor_str);
		}
		else
			c_stack.push(temp);
	}
	if(c_stack.size()!=1)
		correction = 0;
}

void MainWindow::add_spaces_and_simplifie()
{
	QChar symbs[8]{'+','*','/', '-'};
	for(int i = 0; i < 4; i++)
	{
		int a = -2;
		while((a = pstfx.indexOf(symbs[i],a+2))!=-1)
		{
			{
				pstfx.insert(a, ' ');
				pstfx.insert(a+2, ' ');
			}
		}
	}
	pstfx = pstfx.simplified();
}

void MainWindow::create_bin_tree()
{
	node::destroy_bt(main_tree);
	main_tree = NULL;
	infx.clear();

	QTextStream main_stream(&pstfx);
	QString temp = "";
	QStack<QString> main_stack;

	while(!main_stream.atEnd())
	{
		main_stream >> temp;
		if(temp == " ")
			continue;
		main_stack.push(temp);
	}
	node::new_bt(main_tree,main_stack);
	node::create_parents(main_tree, NULL);
	node::print_bt(main_tree,infx);

	infx = infx.simplified();
	ui->output_field->clear();
	ui->show_tree->setEnabled(1);
	ui->output_field->setText(infx);
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
	if(ui->output_field->toPlainText().size() == 0)
		return;
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath(),
													tr("Text Files (*.txt)"));
	if (FileName.isEmpty())
		return;
	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(ui->output_field->toPlainText());
		File.write(Data);
		File.close();
	}
}

void MainWindow::programm_info()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4>Данная программа переводит выражение из постфиксной формы в инфиксную с помощью бинарного дерева."
												  "<br>Также программа строит графическое дерево для заданного выражения."
											  "</font>"));
}

void MainWindow::auth_info()
{
	QMessageBox::about(this,tr("Об авторе"), tr("<font size = 4>Прогрмму написал:<br>Васильев А.А.<br>Группа: 6304</font>"));
}
