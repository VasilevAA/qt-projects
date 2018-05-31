#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication ad(argc, argv);
	MainWindow w;
	w.show();

	return ad.exec();
}
