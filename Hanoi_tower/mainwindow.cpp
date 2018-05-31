#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mgraphicview.h"
#include "block.h"

void set_delay(int msec) //простая фукнция задержки на msec миллисекунд
{
	QTime dieTime= QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->horizontalLayout_4->setSpacing(0);
	for(int i=0; i <3; i++)	//Добавление сцен в окно
	{
		view[i] = new MGraphicView(this);
		ui->horizontalLayout_4->addWidget(view[i]);
	}
	delay = ui->set_delay->value();
	max_bl=10;
	number_of_blocks = 3;
	start_setup();

	setWindowTitle("Ханойские башни");
	setWindowIcon(QIcon(":/icon.png"));
}

MainWindow::~MainWindow()
{
	delete ui;
	for(int k=0; k < 3; k++)
	{
		view[k]->clear_blocks(); //Освобождение выделенной памяти
		delete view[k];
	}

}

void MainWindow:: Hanoi_rec(int n, int src, int dest)
//Принимает на вход: n-кол-во блоков, src-источник,dest-приемник
{
	if(!is_solving)
		return;
	int temp = ((dest==1 && src==2) || (dest==2 && src==1))? 3:
			 ((dest==1 && src==3) || (dest==3 && src==1))? 2: 1;
//Определение промежуточного пункта, который является свободным

	if (n > 1)
	{
		Hanoi_rec(n - 1, src, temp);


		move_block(src,dest);//перемещение верхнего блока с src на dest
		Hanoi_rec(n - 1, temp, dest);
	}
	else
		move_block(src,dest);
}

void MainWindow::Hanoi_backtrack(state final_state)
{
	QVector<state> states;

	const motion moves[6]{motion(1,2),motion(2,1),motion(2,3),motion(3,2),motion(3,1),motion(1,3)};
//Всевозможные движения.

	while(1)
		for(int i = 0; i <6; i++)
			if(is_move_valid(moves[i]) )//Если движение допустимо
			{
				if(!is_solving)
					return;
				move_block(moves[i]);
				state temp(view);//state - текущее состояние пирамид

				if(temp==final_state)//Если состояние конечное - выход
					return;

				if(states.isEmpty() || states.indexOf(temp) == -1)
//Если состояний нет или состояние еще не встречалось - добавить состояние
				{
					states.push_back(temp);
					break;
				}
				states.removeOne(temp);
				move_block(moves[i].backward());
			}
}



void MainWindow::move_block(int start, int end)
{
	if(!is_solving)
		return;

	set_delay(delay);
	curr_index+=1;

	if(ui->instr_output->isChecked())
	{
		ui->instructions->appendPlainText(QString::number(curr_index) +") " + QString::number(start) + " -> " + QString::number(end) );
		return;
	}

	if(view[start-1]->get_height() ==0)
		return;
	int temp_size = view[start-1]->remove_top();
	view[end-1]->add_top_block(temp_size);
}

bool MainWindow::is_move_valid(int from, int to)
{
	if(view[from-1]->get_height() == 0) return false;
	if(view[to-1]->get_height() == 0) return true;
	if(view[from-1]->get_top_block() <
			view[to-1]->get_top_block()) return true;
	return false;
}

void MainWindow::on_solve_clicked()
{
	if(is_solving){
		is_solving = false;
		return;
	}

	int a(1);
	for(int i = 0; i <3 ; i++)
		if (view[i]->get_height() == number_of_blocks)
			a = i+1;
	int b = ui->to->currentText().toInt();

	if(a==b || view[a-1]->get_height()!= number_of_blocks)
		return;

	if(ui->instr_output->isChecked()){

		ui->instructions->clear();
		ui->instructions->appendPlainText("С "+ QString::number(a) + " на " + QString::number(b));
	}


	curr_index=0;

	solve_disabling(true);

	is_solving = true;

	if(ui->back_is_checked->isChecked())
		Hanoi_backtrack(state(b,number_of_blocks));
	else if(ui->rec_is_checked->isChecked())
		Hanoi_rec(number_of_blocks,a,b);

	is_solving = false;

	solve_disabling(false);

	ui->to->setCurrentIndex(a-1);

	ui->result->setText(QString::number(curr_index) + " оп.");
}

void MainWindow::start_setup()
{
	for(int i=0; i <3; i++)
		view[i]->clear_blocks();
	view[2]->create_new_task(0);
	view[1]->create_new_task(0);
	view[0]->create_new_task(number_of_blocks);
}


void MainWindow::on_minus_block_clicked()
{
	if(number_of_blocks==2)
		return;
	number_of_blocks--;
	start_setup();
}

void MainWindow::on_plus_block_clicked()
{
	if(number_of_blocks==max_bl)
		return;
	number_of_blocks++;
	start_setup();
}

void MainWindow::on_set_max_clicked()
{
	int k = ui->lineEdit->text().toInt();
	if(k<6 || k>20)
		return;
	max_bl = k;
	for(int i=0;i<3;i++)
		view[i]->set_max_block(max_bl);
	number_of_blocks = max_bl/2;
	start_setup();
}

void MainWindow::on_set_delay_valueChanged(int value)
{
	Q_UNUSED(value);
		delay = ui->set_delay->value();
		ui->label_5->setText("Задержка\n" + QString::number(delay) + " мс.");
}

void MainWindow::on_instr_output_stateChanged(int arg1)
{
	Q_UNUSED(arg1);
	if(ui->instr_output->isChecked())
		ui->solve->setText("Вывод инструкций");
	else
		ui->solve->setText("Автоигра");
}

void MainWindow::solve_disabling(bool is_working)
{
	ui->plus_block->setDisabled(is_working);
	ui->minus_block->setDisabled(is_working);
	ui->set_max->setDisabled(is_working);
	ui->instr_output->setDisabled(is_working);
	ui->lineEdit->setDisabled(is_working);
	ui->back_is_checked->setDisabled(is_working);
	ui->rec_is_checked->setDisabled(is_working);
	ui->to->setDisabled(is_working);
	ui->menu->setDisabled(is_working);
	ui->instructions->setDisabled(is_working);

	ui->menu_2->setDisabled(is_working);
	for(int i = 0; i < 3; i++)
		view[i]->setDisabled(is_working);
	if(is_working)
		ui->solve->setText("Стоп!");
	else if(ui->instr_output->isChecked())
		ui->solve->setText("Вывод инструкций");
	else
		ui->solve->setText("Автоигра");
}


//
//Удобства
//
void MainWindow::on_programm_info_triggered()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4>Данная программа предназначена для визуализации задачи о Ханойских башнях."
												  "<br>Пользователь может самостоятельно решить задачу: для этого необходимо перемещать"
												  " блоки с одной пирамиды на другую зажатием ЛКМ"
												  "<br><br>Также предоставляется возможность воспользоваться автоигрой."
												  " Для этого необходимо:<br>"
												  "1) Выбрать алгоритм (рекурсия или перебор)<br>"
												  "2) Выбрать конечную пирамиду<br>"
												  "3) При необходимости поставить галочку \"Только инструкции\" для вывода только последовательности дейтсвий<br>"
												  "4) При необходимости выставить задержку перемещений при автоигре  с помощью слайдера (по умолчанию стоит 120 мс)<br>"
												  "5) Нажать кнопку \"Автоигра\"\/\"Вывод инструкций\"<br><br>"
												  "Для начала автоигры необходимо чтобы пирамида находилась "
												  "в изначальной своей форме (то есть все блоки должны быть на старте)<br><br>"
												  "Автоигру можно остановить в любое время, также в любой момент автоигры можно изменить скорость задержки.</font>"));
}

void MainWindow::on_auth_info_triggered()
{
	QMessageBox::about(this,tr("Об авторе"), tr("<font size = 4>Прогрмму написал:<br>Васильев А.А."
												"<br><br>СПбГЭТУ ЛЭТИ,"
												"<br>Группа: 6304"
												"<br><br>Email: the-drus@yandex.ru</font>"));
}

void MainWindow::keyPressEvent(QKeyEvent *event) //горячие клавиши
{
	if(is_solving)
		return;
	switch (event->key()) {
	case Qt::Key_PageUp:
		on_plus_block_clicked();
		break;
	case Qt::Key_PageDown:
		on_minus_block_clicked();
		break;
	case Qt::Key_Return: case Qt::Key_Enter:
		on_set_max_clicked();
		break;
	case Qt::Key_F4:
		if(ui->back_is_checked->isChecked())
			ui->rec_is_checked->setChecked(1);
		else ui->back_is_checked->setChecked(1);
		break;
	case Qt::Key_F1:
		ui->to->setCurrentIndex(0);
		on_solve_clicked();
		break;
	case Qt::Key_F2:
		ui->to->setCurrentIndex(1);
		on_solve_clicked();
		break;
	case Qt::Key_F3:
		ui->to->setCurrentIndex(2);
		on_solve_clicked();
	default:
		break;
	}
}


void MainWindow::on_slotSave_triggered()
{
	if(ui->instructions->toPlainText().size() == 0)
		return;
	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath(),
													tr("Text Files (*.txt)"));
	if (FileName.isEmpty())
		return;
	QFile File(FileName);
	if(File.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QByteArray Data;
		Data.append(ui->instructions->toPlainText());
		File.write(Data);
		File.close();
	}
}

void MainWindow::on_hot_keys_triggered()
{
	QMessageBox::about(this,tr("О программе"), tr("<font size = 4>В программе предусмотрены следующие горячие клавиши:<br>"
												  "PageDown/PageUp : уменьшение или увеличение количества блоков<br>"
												  "Enter : установить максимальное число блоков на сцене (нажать Ок)<br>"
												  "F1, F2, F3 : автоигра перемещение на 1, 2 или 3 позицию соответственно<br>"
												  "F4 : смена алгоритма автоигры"
												  "</font>"));

}


