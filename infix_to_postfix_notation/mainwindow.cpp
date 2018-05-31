#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mstack.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	valid = new QRegExpValidator (QRegExp("^[а-яА-ЯёЁa-z0-9A-Z+\( ) ' '%*-_]+$"), this);
	ui->input_text->setValidator(valid);
	setMinimumSize(350,120);
	setWindowIcon(QIcon("c://tests/1.png"));
	setWindowTitle("Лабораторная работа №3");

	connect(ui->open, SIGNAL(triggered(bool)),this,SLOT(slotOpen()));
	connect(ui->save, SIGNAL(triggered(bool)),this,SLOT(slotSave()));
	connect(ui->input_text, SIGNAL(textChanged(QString)), this, SLOT(activate_button()));
	connect(ui->action_button, SIGNAL(clicked(bool)),this,SLOT(main_action()));
	connect(ui->prog_info, SIGNAL(triggered(bool)),this, SLOT(programm_info()));
	connect(ui->about_info, SIGNAL(triggered(bool)),this, SLOT(auth_info()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete valid;
}

void MainWindow::main_action()
{
	ui->output->clear();
	ui->output->setPlainText(pstfx);
}

void MainWindow::activate_button()
{
	inf_to_postf();
	if(is_expr_correct)
		ui->action_button->setEnabled(1);
	else
		ui->action_button->setDisabled(1);
}

void MainWindow::inf_to_postf()
{
	inp_str.clear();
	inp_str = ui->input_text->text();
	is_expr_correct = 1;
	add_spaces_and_simplifie();
	int a = 0;
	if(inp_str.isEmpty())
	{
		ui->output->clear();
		is_expr_correct = 0;
		return;
	}
	for(int i = 0; i<inp_str.size(); i++)
	{
		if(inp_str[i] == '(') a++;
		if(inp_str[i] == ')') a--;
	}
	if ( a!=0
	|| (!inp_str[inp_str.size()-1].isLetterOrNumber() && inp_str[inp_str.size()-1] != ')'))
	{
		is_expr_correct = 0;
		return;
	}

	QTextStream main_stream(&inp_str);
	QString temp = "";
	pstfx.clear();
	MStack<QString> main_stack;

	bool is_last_tocken_symbol = 0;
	while(!main_stream.atEnd())
	{
		main_stream >> temp;
		if(temp == " ")
			continue;
		if (temp == "+" || temp == "-"){
			if (!is_last_tocken_symbol){
				is_expr_correct = 0;
				return;}
			is_last_tocken_symbol = 0;
			while((!main_stack.Empty()))
			{
				if(main_stack.top() == "(")
					break;
				pstfx+=" "+main_stack.pop();
			}
			main_stack.push(temp);
		}
		else if(temp == "*" || temp == "/" || temp == "%" )
		{
			if (!is_last_tocken_symbol){
				is_expr_correct = 0;
				return;}
			is_last_tocken_symbol = 0;
			while((!main_stack.Empty()))
			{
				if(main_stack.top() == "(" || main_stack.top() == "+" || main_stack.top()== "-")
					break;
				pstfx+=" "+main_stack.pop();
			}
			main_stack.push(temp);
		}
		else if(temp == "(")
		{
			if (is_last_tocken_symbol){
				is_expr_correct = 0;
				return;}
			is_last_tocken_symbol = 0;
			main_stack.push(temp);
		}
		else if (temp == ")")
		{
			if (!is_last_tocken_symbol){
				is_expr_correct = 0;
				return;}
			is_last_tocken_symbol = 1;
			while(main_stack.top() != "(")
			{
				pstfx+=" "+main_stack.pop();
			}
			main_stack.pop();
		}
		else
		{
			if (is_last_tocken_symbol){
				is_expr_correct = 0;
				return;}
			is_last_tocken_symbol = 1;
			pstfx+=" "+temp+" ";
		}
	}
	while(!main_stack.Empty())
		pstfx+=" "+main_stack.pop();
	pstfx = pstfx.simplified();
}

void MainWindow::add_spaces_and_simplifie()
{
	QChar symbs[8]{'+','*','/','(',')', '-','%','^'};
	for(int i = 0; i < 8; i++)
	{
		int a = -2;
		while((a = inp_str.indexOf(symbs[i],a+2))!=-1)
		{
		//	if (i < 5||(i==5 && inp_str[a+1] ==' '))
			{
				inp_str.insert(a, ' ');
				inp_str.insert(a+2, ' ');
			}
		}
	}
	inp_str = inp_str.simplified();
}

void MainWindow::slotOpen()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Text files (*.txt)");
	if (FileName.isEmpty())
		return;

	QFile File(FileName);

	if(File.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		ui->input_text->setText(File.readAll());
		File.close();
	}
	else
		QMessageBox::warning(this, "Error", QString("Не смог открыть файл %1").arg(FileName),QMessageBox::Ok);
}

void MainWindow::slotSave()
{
	if(ui->output->toPlainText().size() == 0)
		return;
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath(),
													tr("Text Files (*.txt)"));
	if (FileName.isEmpty())
		return;
	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(ui->output->toPlainText());
		File.write(Data);
		File.close();
	}
}

void MainWindow::programm_info()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4>Данная программа переводит выражение из инфиксной формы в постфиксную."
												  "</font>"));
}

void MainWindow::auth_info()
{
	QMessageBox::about(this,tr("Об авторе"), tr("<font size = 4>Прогрмму написал:<br>Васильев А.А.<br>Группа: 6304</font>"));
}
