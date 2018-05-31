#include "qt_libs.h"

#ifndef CREATE_DIALOG_H
#define CREATE_DIALOG_H


class create_dialog : public QDialog
{
	Q_OBJECT
private:
	QLineEdit* wid;
	QLineEdit* he;


public:
	create_dialog(QWidget* parent = 0);

	static void get_w_and_h(int &, int &);

};

#endif // CREATE_DIALOG_H

