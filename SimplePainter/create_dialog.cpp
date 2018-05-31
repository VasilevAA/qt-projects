#include "create_dialog.h"
#include "qt_libs.h"

create_dialog::create_dialog(QWidget* parent) : QDialog(parent, Qt::WindowTitleHint)
{
	setWindowTitle("Новый рисунок");
	setModal(1);
	setFixedSize(minimumSize());
	wid = new QLineEdit;
	he = new QLineEdit;
	QValidator* valid = new QRegExpValidator (QRegExp("^[0-9]+$"));

	wid->setValidator(valid);
	he->setValidator(valid);
	wid->setText("1280");
	he->setText("720");
	wid->setPlaceholderText("Ширина");
	he->setPlaceholderText("Высота");
	QPushButton* OkButton = new QPushButton("&Ok");
	QPushButton* CancelButton = new QPushButton("&Cancel");

	QGridLayout *  lay = new QGridLayout;
	lay->addWidget(wid,0,0,1,2);
	lay->addWidget(he,1,0,1,2);
	lay->addWidget(OkButton,2,0);
	lay->addWidget(CancelButton,2,1);
	setLayout(lay);

	connect(OkButton,SIGNAL(clicked()),SLOT(accept()));
	connect(CancelButton,SIGNAL(clicked()),SLOT(reject()));
}

void create_dialog::get_w_and_h(int& wid, int& hei)
{
	create_dialog* temp = new create_dialog();
	temp->show();
	if(temp->exec() == QDialog::Accepted)
	{
		wid = temp->wid->text().toInt();
		hei = temp->he->text().toInt();
	}
	else
		wid = -12;
}
