#include <iostream>
#include <cstdlib>
#include <conio.h>
#include "fairy_tail.hpp"

using namespace std;

Fairyland world;

// количество возможных направлений
int dirs_count = 0;
// направления (Up, Right, Down, Left)
bool directions[4];				

// карта
char MAP[12][12];		

// подготовка карты
void start_map() {							
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 12; j++) {
			if (i == 1 && j == 1) {
				MAP[i][j] = '@';
			}
			else {
				if (i == 10 && j == 10) {
					MAP[i][j] = '&';
				}
				else {
					MAP[i][j] = '?';
				}
			}
		}
}

// заполнение точкой
void fill_dot(int x, int y) {
	if (MAP[x][y] != '@' && MAP[x][y] != '&') MAP[x][y] = '.';	
}

// заполнение решеткой
void fill_sharp(int x, int y) {
	if (MAP[x][y] != '@' && MAP[x][y] != '&') MAP[x][y] = '#';	
}

// изменение переменных, отвечающих за местоположение
int* change_position(Direction dir, int x, int y) {				
	switch (dir) {
		case Direction::Down:	y++; break;
		case Direction::Up:		y--; break;
		case Direction::Right:	x++; break;
		case Direction::Left:	x--; break;
	}
	int* pos = new int[2]{ x,y };
	return pos;
}

// инверсия направления
Direction reverseDir(Direction dir) {							
	Direction rid = Direction::Pass;

	switch (dir) {
		case Direction::Down:	rid = Direction::Up; break;
		case Direction::Up:		rid = Direction::Down; break;
		case Direction::Left:	rid = Direction::Right; break;
		case Direction::Right:	rid = Direction::Left; break;
	}
	return rid;
}

// установка решетки
void block_path(Direction dir, int x, int y) {					
	switch (dir) {
		case Direction::Down:	fill_sharp(x, y + 1); break;
		case Direction::Right:	fill_sharp(x + 1, y); break;
		case Direction::Up:		fill_sharp(x, y - 1); break;
		case Direction::Left:	fill_sharp(x - 1, y); break;
	}
}

// установка точки
void make_path(Direction dir, int x, int y) {					
	switch (dir) {
		case Direction::Down:	fill_dot(x, y + 1); break;
		case Direction::Right:	fill_dot(x + 1, y); break;
		case Direction::Up:		fill_dot(x, y - 1); break;
		case Direction::Left:	fill_dot(x - 1, y); break;
	}
}

// проверка направления
void check_dir(Character pers, Direction dir, int x, int y, int dir_num) {	
	/*
		1. если персонаж может пойти по направлению: увеличивается количество направлений,
		   элемент логического массива по соответствующему номеру становится истиным,
		   устанавливается точка
		2. иначе: ставится решетка и элемент логического массива ложен
	*/
	if (world.canGo(pers, dir)) {
		dirs_count++;
		directions[dir_num] = true;	
		make_path(dir, x, y);
	}
	else {
		block_path(dir, x, y);
		directions[dir_num] = false;
	}
}

// подсчет направлений
void get_dirs_count(Character pers, int x, int y) {			
	dirs_count = 0;

	check_dir(pers, Direction::Up,		x, y, 0);
	check_dir(pers, Direction::Right,	x, y, 1);
	check_dir(pers, Direction::Down,	x, y, 2);
	check_dir(pers, Direction::Left,	x, y, 3);
}

// для одного направления
Direction one_direction() {						
	Direction dir = Direction::Pass;

	if (directions[0]) dir = Direction::Up;
	if (directions[1]) dir = Direction::Right;
	if (directions[2]) dir = Direction::Down;
	if (directions[3]) dir = Direction::Left;

	return dir;
}

// функция для конвертации направления из элемента массива bool в Direction
Direction get_dir(bool direc[4]) {				
	Direction dir;

	if (direc[0]) {
		dir = Direction::Up;
		direc[0] = false;
	}
	else {
		if (direc[1]) {
			dir = Direction::Right;
			direc[1] = false;
		}
		else {
			if (direc[2]) {
				dir = Direction::Down;
				direc[2] = false;
			}
			else {
				if (direc[3]) {
					dir = Direction::Left;
					direc[3] = false;
				}
				else {
					dir = Direction::Pass;
				}
			}
		}
	}
	return dir;
}

// функция для определения направленя относительно последнего
Direction checkDir(Direction dir1, Direction dir2, Direction LastDir) {		
	/*
		1. устанавливаем значение dir по умолчанию Pass
		2. если reverseDir(dir1) == dir2 (т.е. направления параллельны), dir = LastDir, иначе
		3. если одно из направление является последним, другое присваивается dir
	*/
	Direction dir = Direction::Pass;

	if (reverseDir(dir1) == dir2) {
		dir = LastDir;
	}
	else {
		if (reverseDir(dir1) == LastDir) {
			dir = dir2;
		}
		else {
			if (reverseDir(dir2) == LastDir) {
				dir = dir1;
			}
		}
	}
	return dir;
}

// функция для 2 направлений
Direction two_directions(Direction LastDir) {	
	/*
		1. определяются 2 направления
		2. результатом становиться направление, отличное от последнего
	*/
	
	Direction dir1 = get_dir(directions);
	Direction dir2 = get_dir(directions);

	Direction dir = checkDir(dir1, dir2, LastDir);
	
	return dir;
}

// рандомное направление
Direction randomDir(Direction dir1, Direction dir2) { 
	Direction dir;
	
	int a = rand();

	if (a % 2 == 0) {
		dir = dir2;
	}
	else {
		dir = dir1;
	}
	return dir;
}

// функция для трех направлений
Direction three_directions(Direction LastDir) { 
	/*
		1. определяются 3 направления
		2. проверяются первые 2
		3. если оба не являются последними направлениями,
		   то любой из них становится следующим направлением
		4. иначе определяется какое из 2 направлений не является последним, 
		   и уже выбирается любое из этого направления и оставшигося
	*/
	Direction dir1 = get_dir(directions);
	Direction dir2 = get_dir(directions);
	Direction dir3 = get_dir(directions);

	Direction checkedDir = checkDir(dir1, dir2, LastDir);

	Direction dir;

	if (checkedDir == Direction::Pass) {
		dir = dir1;
	}
	else {
		if (checkedDir == dir1) {
			dir = dir3;
		}
		else {
			dir = dir2;
		}
	}
	return dir;
}

// для 4 направлений
Direction four_directions(Direction LastDir) { 
	/*
		1. берутся 2 направления из четырех дважды
		2. выбирается одно из них случайным образом
	*/
	Direction dir1 = two_directions(LastDir);
	Direction dir2 = two_directions(LastDir);

	Direction dir = randomDir(dir1, dir2);

	return dir;
}

// функция для определения конечного выбора направления
Direction path(Character pers, Direction LastDir, int x, int y) { 
	/*
		1. считается количество напрвлений
		2. в зависимости от количества выбирается функция
	*/
	Direction dir;
	get_dirs_count(pers, x, y);

	switch (dirs_count) {
		case 1: dir = one_direction(); break;
		case 2: dir = two_directions(LastDir); break;
		case 3: dir = three_directions(LastDir); break;
		case 4: dir = four_directions(LastDir); break;
	}
	return dir;
}

// основная функция
int fairyland_meeting() {	
	/*
		1. заполняется карта
		2. обьявление переменных для координат и направлений
		3. начало цикла
		4. применение функции для определения с направлением
		5. запись направления в последнее
		6. изменение координат в соответствии с направлением
		7. условие окончания цикла
		8. удаление динамических массивов
		9. возвращение количества ходов
	*/
	start_map();

	int* ivan_xy = new int[2]{ 1, 1 };
	int* elena_xy = new int[2]{ 10, 10 };

	Direction DirecIvan;
	Direction DirecElena;

	Direction LastDirIvan = Direction::Up;
	Direction LastDirElena = Direction::Down;

	do {
		DirecIvan = path(Character::Ivan, LastDirIvan, ivan_xy[0], ivan_xy[1]);
		DirecElena = path(Character::Elena, LastDirElena, elena_xy[0], elena_xy[1]);

		LastDirIvan = DirecIvan;
		LastDirElena = DirecElena;

		ivan_xy = change_position(DirecIvan, ivan_xy[0], ivan_xy[1]);
		elena_xy = change_position(DirecElena, elena_xy[0], elena_xy[1]);

	} while (!world.go(DirecIvan, DirecElena));

	delete[] ivan_xy;
	delete[] elena_xy;

	return world.getTurnCount();
}

int main() 
{
	setlocale(LC_ALL, "rus");

	int steps_count = fairyland_meeting();

	cout << "Встреча произошла за " << steps_count << " ходов" << endl;
	cout << endl;	cout << "=====Карта мира=====" << endl;

	for (int i = 1; i < 11; i++) {
		cout << "     ";
		for (int j = 1; j < 11; j++) {
			if (j == 10) cout << MAP[j][i] << endl;
			else cout << MAP[j][i];
		}
	}
	_getch();
	return 0;
}
