#include <assert.h>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

const int size = 9;
const int realSize = 2 * size - 1;
const int w = 5;

bool isWall(int x0, int x1, int y0, int y1, int pole[17][17]);

bool jump(int& x, int& y, int pole[17][17], int p, char ans);

void placeWall(int pole[17][17], int p, int t, int u);

bool aZvezda(int x, int y, int pole[17][17], int p);

void output(int pole[17][17])
{
	for (int i = 0; i < realSize; i++) //Вывод поля
	{
		for (int j = 0; j < realSize; j++)
		{
			if (pole[j][i] == -1 && j % 2 == 0 && i % 2 == 0)
				cout << "0 ";
			else if (pole[j][i] == 0)
				cout << "1 ";
			else if (pole[j][i] == 1)
				cout << "2 ";
			else if (pole[j][i] == w)
				if (j % 2 == 1 && i % 2 == 1)
					cout << "  ";
				else if (i % 2 == 0)
					cout << "| ";
				else
					cout << "- ";
			else
				cout << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

pair<int, int> step(int x, int y, char ch, int shag)
{
	switch (ch)
	{
	case 'l':
		return make_pair(x - shag, y);
	case 'r':
		return make_pair(x + shag, y);
	case 'd':
		return make_pair(x, y + shag);
	case 'u':
		return make_pair(x, y - shag);
	default:
		assert(false);
		return make_pair(0, 0);
	}
}

bool inPole(int x, int y)
{
	return x >= 0 && x < realSize && y >= 0 && y < realSize;
}

bool outPole(int x, int y)
{
	return !inPole(x, y);
}

bool isPlayer(int new_x, int new_y, int coords[3][2])
{
	for (int i = 0; i < 2; i++)
	{
		if (new_x == coords[0][i] && new_y == coords[1][i])
			return true;
	}
	return false;
}

int turn(int pole[17][17], int coords[3][2], int player_id)
{
	int number = player_id + 1;
	bool flag = true;
	while (flag)
	{
		cout << "Turn of Player" << number << ", chose move or place a wall (m/w): ";
		char ans;
		cin >> ans;
		cout << endl;
		switch (ans)
		{
		case 'm':
			while (flag)
			{
				cout << "Chose direction - left, right, down or up (l/r/d/u): ";
				cin >> ans;
				cout << endl;
				switch (ans)
				{
				case 'l':
				case 'r':
				case 'd':
				case 'u':
				{
					pair<int, int> new_coords = step(coords[0][player_id], coords[1][player_id], ans, 2);
					int new_x = new_coords.first;
					int new_y = new_coords.second;
					if (outPole(new_x, new_y))
					{
						cout << "Error: you cant move here (outFiled)" << endl;
						break;
					}
					if (isWall(coords[0][player_id], coords[1][player_id], new_x, new_y, pole))
					{
						cout << "Error: you cant move here (walls)" << endl;
						break;
					}
					if (!isPlayer(new_x, new_y, coords))
					{
						pole[new_x][new_y] = player_id;
						pole[coords[0][player_id]][coords[1][player_id]] = -1;
						coords[0][player_id] = new_x;
						coords[1][player_id] = new_y;
						flag = false;
					}
					else
					{
						if (jump(coords[0][player_id], coords[1][player_id], pole, player_id, ans))
							flag = false;
					}
					break;
				}
				default:
					cout << "Error: wrong char!" << endl;
					break;
				}
			}
			if (coords[0][player_id] == 16 - player_id * 16)
				return 1; //Проверка на победу
			break;
		case 'w':
			if (coords[2][player_id] == 0)
			{
				cout << "Error: Player" << number << " out of walls!" << endl;
				break;
			}
			placeWall(pole, 1 - player_id, coords[0][1 - player_id], coords[1][1 - player_id]);
			coords[2][player_id]--;
			if (!aZvezda(coords[0][player_id], coords[1][player_id], pole, player_id))
			{
				cout << "Ops, Player" << number << " closed himself!" << endl;
				return 2;
			}
			flag = false;
			break;
		case 'p':
			flag = false;
			break;
		default:
			cout << "Error: wrong char!" << endl;
			break;
		}
	}
	return 0;
}

int main()
{
	int pole[realSize][realSize];	  //Создаём поле
	for (int i = 0; i < realSize; i++) //Заполняем -1
	{
		for (int j = 0; j < realSize; j++)
		{
			if (j % 2 == 1 || i % 2 == 1)
				pole[i][j] = -2;
			else
				pole[i][j] = -1;
		}
	}

	//Ставим фишки на начальные места
	int coords[3][2] = {{0, realSize - 1}, {(realSize - 1) / 2, (realSize - 1) / 2}, {10, 10}};
	pole[coords[0][0]][coords[1][0]] = 0;
	pole[coords[0][1]][coords[1][1]] = 1;

	output(pole);

	//Храним информации о конце игры
	int end = 0;
	int player = 0; // 0 - "первый", 1 - "второй"
	int winner = 0; // 0 - "первый", 1 - "второй"

	while (end == 0)
	{
		end = turn(pole, coords, player);
		if (end == 1)
			winner = player;
		else
			winner = (player + 1) % 2;
		player = (player + 1) % 2;
		output(pole);
	}

	cout << "Player" << winner + 1 << " wins!" << endl;
	return 0;
}

bool different_dir(char a, char b)
{
	return (((a == 'l' || a == 'r') && (b == 'u' || b == 'd')) || ((a == 'u' || a == 'd') && (b == 'l' || b == 'r')));
}

bool jump(int& x, int& y, int pole[17][17], int p, char dir)
{
	pair<int, int> coords = step(x, y, dir, 2);
	int other_x = coords.first;
	int other_y = coords.second;
	coords = step(other_x, other_y, dir, 2);
	int new_x = coords.first;
	int new_y = coords.second;
	if (inPole(new_x, new_y) && !isWall(other_x, other_y, new_x, new_y, pole))
	{
		swap(pole[new_x][new_y], pole[x][y]);
		x = new_x;
		y = new_y;
		return true;
	}
	if ((other_y > 15 || isWall(other_x, other_y, other_x, other_y + 2, pole)) && (other_y < 2 || isWall(other_x, other_y, other_x, other_y - 2, pole)))
	{
		cout << "Error: you cant jump here (" << dir << ")!" << endl;
		return false;
	}

	while (true)
	{
		const char* choise_dir = (dir == 'l' || dir == 'r') ? "down or up (d/u)" : "left or right (l/r)";
		cout << "Chose direction - " << choise_dir << ": ";
		char ans;
		cin >> ans;
		cout << endl;
		if (!different_dir(dir, ans))
		{
			cout << "Error: wrong char!" << endl;
			continue;
		}

		coords = step(other_x, other_y, ans, 2);
		new_x = coords.first;
		new_y = coords.second;
		if (outPole(new_x, new_y) || isWall(other_x, other_y, new_x, new_y, pole))
		{
			cout << "Error: you cant jump here (" << dir << ")!" << endl;
			continue;
		}
		swap(pole[new_x][new_y], pole[x][y]);
		x = new_x;
		y = new_y;
		return true;
	}
}

void placeWall(int pole[17][17], int p, int t, int u)
{
	while (true)
	{
		int x, y;
		char dir;
		cout << "Chose x, y and direction of the wall (x, y, v/h): ";
		cin >> x >> y >> dir;
		cout << endl;

		x = 2 * x + 1;
		y = 2 * y + 1;

		if (outPole(x, y))
		{
			cout << "Error: wrong x or/and y!" << endl;
			continue;
		}

		int wall[3][2];

		switch (dir)
		{
		case 'h':
			for (int i = 0; i < 3; i++)
			{
				wall[i][0] = x + i - 1;
				wall[i][1] = y;
			}
			break;
		case 'v':
			for (int i = 0; i < 3; i++)
			{
				wall[i][0] = x;
				wall[i][1] = y + i - 1;
			}
			break;
		default:
			cout << "Error: wrong char!" << endl;
			continue;
		}

		bool flag = false;
		for (int i = 0; i < 3; i++)
		{
			if (isWall(wall[i][0], wall[i][1], wall[i][0], wall[i][1], pole))
			{
				cout << "Error: wrong x/y/dir!" << endl;
				flag = true;
			}
		}
		if (flag)
			continue;

		for (int i = 0; i < 3; i++)
		{
			pole[wall[i][0]][wall[i][1]] = 5;
		}

		if (!aZvezda(t, u, pole, p))
		{
			for (int i = 0; i < 3; i++)
			{
				pole[wall[i][0]][wall[i][1]] = -1;
			}
			cout << "Error: you can't place wall like that (you are blocking the way)!" << endl;
			placeWall(pole, p, t, u);
			return;
		}

		if (dir == 'v' || dir == 'h')
			break;
	}
}

bool isWall(int x0, int y0, int x1, int y1, int pole[17][17])
{
	return pole[(x0 + x1) / 2][(y0 + y1) / 2] == w;
}

bool aZvezda(int x, int y, int pole[17][17], int p)
{
	bool field[9][9];

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			field[i][j] = false;
		}
	}

	queue<pair<int, int>> frontier;
	frontier.push(make_pair(x / 2, y / 2));
	field[x / 2][y / 2] = true;
	pair<int, int> current;
	pair<int, int> next;

	while (!frontier.empty())
	{
		current = frontier.front();

		x = current.first;
		y = current.second;

		next = step(x * 2, y * 2, 'l', 2);
		if (!field[next.first / 2][next.second / 2] && !isWall(x * 2, y * 2, next.first, next.second, pole) && next.first >= 0)
		{
			frontier.push(make_pair(next.first / 2, next.second / 2));
			field[next.first / 2][next.second / 2] = true;
		}

		next = step(x * 2, y * 2, 'u', 2);
		if (!field[next.first / 2][next.second / 2] && !isWall(x * 2, y * 2, next.first, next.second, pole) && next.second >= 0)
		{
			frontier.push(make_pair(next.first / 2, next.second / 2));
			field[next.first / 2][next.second / 2] = true;
		}

		next = step(x * 2, y * 2, 'r', 2);
		if (!field[next.first / 2][next.second / 2] && !isWall(x * 2, y * 2, next.first, next.second, pole) && next.first <= 16)
		{
			frontier.push(make_pair(next.first / 2, next.second / 2));
			field[next.first / 2][next.second / 2] = true;
		}

		next = step(x * 2, y * 2, 'd', 2);
		if (!field[next.first / 2][next.second / 2] && !isWall(x * 2, y * 2, next.first, next.second, pole) && next.second <= 16)
		{
			frontier.push(make_pair(next.first / 2, next.second / 2));
			field[next.first / 2][next.second / 2] = true;
		}

		/*int pol[9][9];
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (field[i][j])
					pol[i][j] = p + 1;
				else
					pol[i][j] = 0;
			}
		}

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				cout << pol[j][i] << " ";
			}
			cout << endl;
		}
		cout << endl;*/

		frontier.pop();

		for (int j = 0; j < 9; j++)
			if (field[8 - 8 * p][j])
			{
				// cout << "ok" << endl;
				return true;
			}
	}
	return false;
}
