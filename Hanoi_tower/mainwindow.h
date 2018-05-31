#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mgraphicview.h"
#include "state_and_motion.h"
#include <QMainWindow>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_minus_block_clicked();
//Обработка кнопки (-) - уменьшение блоков в пирамиде

	void on_plus_block_clicked();
//Обработка кнопки (+) - увеличение блоков в пирамиде

	void on_set_max_clicked();
//Обработка кнопки(ОК) - изменение макс. возможного размера пирамиды

	void on_solve_clicked();
//Обработка кнопки (решить) - автоматическое решение задачи

	void on_programm_info_triggered();

	void on_auth_info_triggered();

	void on_slotSave_triggered();

	void on_hot_keys_triggered();


	void on_set_delay_valueChanged(int value);

	void on_instr_output_stateChanged(int arg1);

private:


	MGraphicView* view[3]; //3 сцены для 3-х пирамид
	int number_of_blocks;  //Текущее количество блоков в пирамиде
	int max_bl;		//Максимально возможный размер пирамиды (м.б. изменен)

	void start_setup();
//Функция первоначальной настройки 3-х сцен:
//очистка от блоков, вызов функции отрисовки новой пирамиды.

	void move_block(motion m){ move_block(m.from,m.to);}//перегруженная под motion
	void move_block(int start, int end);
//Функция перемещает верхний блок из пирамиды start
//на пирамиду end

	bool is_move_valid(motion m){return is_move_valid(m.from,m.to);} //Перегрузка под motion
	bool is_move_valid(int from, int to);
//Функция определяющая, является ли перемещение с from на to корректным

	void Hanoi_rec(int n, int src, int dest);
//Функция решающая задачу о Ханойских башнях (рекурсия):
//n - Количество блоков;
//src	- пирамида с кольцами (стартовая);
//dest - место пирамиды после окончания перемещения.

void Hanoi_backtrack(state final_state);
//Функция решающая задачу перебором.
//final_state - финальное состояние


//
//для интерфейса

void solve_disabling(bool a);
//выключение интерфейса во время автоигры

	bool is_solving{0}; //запущена ли автоигра в данный момент

	int curr_index;  //номер текущего перемещения в автоигре

	int delay{0};	//задержка между перемещениями в автоигре.

protected:
	void keyPressEvent(QKeyEvent *event); //переопределенный метод для горячих клавиш


	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
