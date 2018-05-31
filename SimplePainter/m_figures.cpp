#include "m_figures.h"


//Функция отрисовки фигуры.
void m_figures::paint_figure(QPainter* painter)
{
	painter->setPen(pen);
	painter->setBrush(brush);

	switch (what_figure)
	{
	case 1:
		painter->drawLine(line);
		break;

	case 2:
		painter->drawRect(rect);
		break;

	case 3:
		painter->drawEllipse(rect);
		break;
	case 4:
		for (int i = 0; i < (curve.size()-1); i++)
			painter->drawLine(curve[i],curve[i+1]);

		break;
	default:
		break;
	}
}


