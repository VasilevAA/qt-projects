#ifndef STATE_AND_MOTION_H
#define STATE_AND_MOTION_H
//Данные структуры необходимы для упрощения работы функции Hanoi_backtrack

//Структура хранит текущее состояние всех пирамид
struct state{
	state(){}

	state(int num, int number_of_blocks){	//Конструктор для финальной позиции
		heights[num-1] = number_of_blocks;
		top_blocks[num-1] = 1;
	}
	state(MGraphicView ** view){	//Конструктор для текущей позиции
		for(int i=0; i <3;i++)
		{
			heights[i] = view[i]->get_height();
			top_blocks[i] = view[i]->get_top_block();
		}
	}
	int heights[3]{0};	//Высоты 3-х пирамид
	int top_blocks[3]{0};	//Верхние блоки 3-х пирамид
	bool operator==(const state& other)//Оператор сравнения
	{
		for(int i = 0; i <3; i++)
			if(heights[i] != other.heights[i] ||
				top_blocks[i] != other.top_blocks[i])
				return false;
		return true;
	}
};

//Структура для храненния перемещение из from в to
struct motion{
	motion(int a, int b) : from(a),to(b){} //Конструктор
	motion backward() const { return motion(to,from);}//возвращает перемещение назад.
	int from;
	int to;
};
#endif // STATE_AND_MOTION_H
