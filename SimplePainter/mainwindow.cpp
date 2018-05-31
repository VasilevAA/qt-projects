#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "m_figures.h"
#include "lay_widget.h"
#include "create_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setMinimumSize(500,500);//минимальный размер окна

	//Установка "белого" фона окна

	//Установка цвета панели инструментов
	ui->groupBox->setAutoFillBackground(1);
	ui->groupBox->setPalette(QPalette(QColor(200,200,200)));

	//Установка эскизов для текущих цветов ручки и заливки
	ui->current_color->setPalette(QPalette(Qt::black));
	ui->current_brush->setPalette(QPalette(Qt::transparent));

	//Установка стартовой точки фигуры в ноль

	back = new lay_widget(this);
	back->setGeometry(7,77,1282,722);
	back->setPalette(Qt::black);

	layer = new lay_widget(this);
	ui->forward->click();
	//ui->verticalLayout_2->addWidget(a);
	connect(ui->forward,SIGNAL(clicked(bool)),layer,SLOT(on_forward_clicked()));

	connect(ui->width, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_info()));

	connect(ui->color_button, SIGNAL(clicked(bool)),this,SLOT(choose_color()));

	connect(ui->back,SIGNAL(clicked(bool)),layer,SLOT(back_action()));

	connect(ui->style, SIGNAL(currentIndexChanged(QString)),this,SLOT(set_info()));

	connect(ui->brush, SIGNAL(clicked(bool)),this,SLOT(choose_brush()));

	connect(ui->line,SIGNAL(clicked(bool)),this,SLOT(set_info()));
	connect(ui->rect,SIGNAL(clicked(bool)),this,SLOT(set_info()));
	connect(ui->ellipse,SIGNAL(clicked(bool)),this,SLOT(set_info()));
	connect(ui->curve,SIGNAL(clicked(bool)),this,SLOT(set_info()));
}

MainWindow::~MainWindow()
{
	delete layer;
	delete ui;
}

void MainWindow::set_info()
{
			layer->get_info(ui->line->isChecked(),ui->rect->isChecked(),
							ui->ellipse->isChecked(),ui->curve->isChecked(),
							ui->style->currentIndex()+1,ui->width->currentText().toInt());
		this->update();
}

void MainWindow::choose_color()
{
	QColor a = QColorDialog::getColor(Qt::white,nullptr,QString("Цвет линии"),QColorDialog::ColorDialogOptions(0x00000001));
	layer->get_color(a);
	ui->current_color->setPalette(QPalette(a));
}

void MainWindow::choose_brush()
{
	QColor a = QColorDialog::getColor(Qt::white,nullptr,QString("Выбор заливки"),QColorDialog::ColorDialogOptions(0x00000001));
	layer->get_brush(a);
	ui->current_brush->setPalette(QPalette(a));
	this->update();
}




void MainWindow::on_create_triggered()
{
	if(!layer->is_empty())
	{
		QMessageBox::StandardButton
		reply = QMessageBox::question(this,"оп","Рисунок будет утерян. Хотите сохранить?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
		{
			on_save_triggered();
			return;
		}
	}
	int a, b;
	create_dialog::get_w_and_h(a,b);

	if(a==-12)
		return;
	else
	{
		layer->change_size(a,b,1);
		back->setGeometry(7,77,a+2,b+2);
	}

}

void MainWindow::on_save_triggered()
{

	QString FileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::homePath()+ "/picture",
													tr("Изображение (*.png)"));
	if(FileName.isEmpty())
		return;
	layer->grab().save(FileName);
}

void MainWindow::on_canvas_triggered()
{
	int a, b;
	create_dialog::get_w_and_h(a,b);
	if(a==-12)
		return;
	layer->change_size(a,b);
	back->setGeometry(7,77,a+2,b+2);

}

void MainWindow::on_open_triggered()
{
	QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", QDir::homePath(), "Изображения (*.png *.jpg)");
	if(FileName.isEmpty())
		return;
	QBrush br(Qt::TexturePattern);
	QImage image(FileName);
	br.setTextureImage(image);

	layer->change_size(image.width(),image.height());
	back->setGeometry(7,77,image.width()+2,image.height()+2);

	QPalette plt;
	plt.setBrush(QPalette::Background, br);
	layer->setPalette(plt);

}
