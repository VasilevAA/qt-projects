#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QValidator>
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
	void add_spaces_and_simplifie();
	void inf_to_postf();

	QValidator *valid;
	QString pstfx;
	QString inp_str;
	bool is_expr_correct;

private slots:
	void main_action();
	void activate_button();
	void programm_info();
	void auth_info();
	void slotOpen();
	void slotSave();
};
#endif // MAINWINDOW_H
