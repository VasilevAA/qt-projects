#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "bin_tree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

	node* main_tree;
	QString pstfx;
	QString infx;

	bool correction;
	QValidator *valid;

	void check_correction();
	void add_spaces_and_simplifie();
private slots:
	void create_bin_tree();
	void show_tree();
	void button_activator();

	void programm_info();
	void auth_info();
	void slotOpen();
	void slotSave();

};

#endif // MAINWINDOW_H
