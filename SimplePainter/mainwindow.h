#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "m_figures.h"
#include "lay_widget.h"
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

	QWidget* back;
	lay_widget* layer;

	Ui::MainWindow *ui;


private slots:
	void set_info();

	void choose_color();
	void choose_brush();

	void on_create_triggered();
	void on_save_triggered();
	void on_canvas_triggered();
	void on_open_triggered();
};



#endif // MAINWINDOW_H
