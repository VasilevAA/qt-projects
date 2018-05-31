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
	node* element_for_find;
	QString str_tree;

	QValidator *valid;

	bool check_correct();
private slots:
	void create_bin_tree();
	void show_tree();

	void tree_activator();

	void find_element();
	void programm_info();
	void auth_info();
	void slotOpen();
	void slotSave();

	void remove_elem();
	void insert_elem();
};

#endif // MAINWINDOW_H
