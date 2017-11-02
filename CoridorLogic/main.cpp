#include <assert.h>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

const int size = 9;
const int realSize = 2 * size - 1;
const int w = 5;

typedef bool (*funptr)(int&, int&, Walls, int[9][9], int);

bool isWall(int x0, int x1, int y0, int y1, int pole[17][17]);

bool checkPlaceWall(bool ver, int x, int y, Walls walls);

bool jumpLeft(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpRight(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpDown(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpUp(int& x, int& y, Walls walls, int pole[9][9], int p);

void placeWall(int pole[][], int p, int t, int u);

bool aZvezda(int x, int y, Walls walls, int p);

void output(const Walls& walls, int pole[9][9])
{
    for (int i = 0; i < 9; i++) //Вывод поля
    {
        for (int j = 0; j < 9; j++)
        {
            cout << pole[j][i] << " ";
            if (j != 8)
            {
                if (i != 0 && i != 8)
                {
                    if ((walls.pos[j][i] && !walls.hor[j][i]) || (walls.pos[j][i - 1] && !walls.hor[j][i - 1]))
                        cout << "| ";
                    else
                        cout << "  ";
                }
                else if (i == 0)
                {
                    if (walls.pos[j][i] && !walls.hor[j][i])
                        cout << "| ";
                    else
                        cout << "  ";
                }
                else
                {
                    if (walls.pos[j][i - 1] && !walls.hor[j][i - 1])
                        cout << "| ";
                    else
                        cout << "  ";
                }
            }
        }
        cout << endl;
        for (int j = 0; j < 9; j++)
        {
            if (i == 8)
                break;
            if (j != 0 && j != 8)
            {
                if ((walls.pos[j][i] && walls.hor[j][i]) || (walls.pos[j - 1][i] && walls.hor[j - 1][i]))
                    cout << "-   ";
                else
                    cout << "    ";
            }
            else if (j == 0)
            {
                if (walls.pos[j][i] && walls.hor[j][i])
                    cout << "-   ";
                else
                    cout << "    ";
            }
            else
            {
                if (walls.pos[j - 1][i] && walls.hor[j - 1][i])
                    cout << "-   ";
                else
                    cout << "    ";
            }
        }
        cout << endl;
    }
}

pair<int, int> step(int x, int y, char ch)
{
    switch (ch)
    {
    case 'l':
        return make_pair(x - 2, y);
    case 'r':
        return make_pair(x + 2, y);
    case 'd':
        return make_pair(x, y + 2);
    case 'u':
        return make_pair(x, y - 2);
    default:
        assert(false);
        return make_pair(0, 0);
    }
}

bool outPole(int new_x, int new_y)
{
    return !(new_x >= 0 && new_x < realSize && new_y >= 0 && new_y < realSize);
}

bool isPlayer(int new_x, int new_y, int coords[3][2])
{
    for (int i = 0; i < 2; i++)
        if (new_x == coords[i][0] && new_y == coords[i][1])
            return true;
    return false;
}

int turn(int pole[9][9], int& coords[3][2], int player_id, std::map<char, funptr> jumps)
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
                    pair<int, int> coords = step(coords[0][player_id], coords[1][player_id], ans);
                    int new_x = coords.first;
                    int new_y = coords.second;
                    if (outPole(new_x, new_y))
                    {
                        cout << "Error: you cant move here (outFiled)" << endl;
                        break;
                    }
                    if (!isWall(coords[0][player_id], coords[1][player_id], new_x, new_y, pole))
                    {
                        cout << "Error: you cant move here (walls)" << endl;
                        break;
                    }
                    if (!isPlayer(new_x, new_y, coords))
                    {
                        pole[new_x][new_y] = player_id;
                        pole[x1][y1] = -1;
                        x1 = new_x;
                        y1 = new_y;
                        flag = false;
                    }
                    else
                    {
                        if (jumps[ans](coords[0][player_id], coords[1][player_id], pole, player_id))
                            flag = false;
                    }
                    break;
                }
                default:
                    cout << "Error: wrong char!" << endl;
                    break;
                }
            }
            if (x1 == 16 - player_id * 16)
                return true; //Проверка на победу
            break;
        case 'w':
            if (coords[2][player_id] == 0)
            {
                cout << "Error: Player" << number << " out of walls!" << endl;
                break;
            }
            placeWall(walls, 2 - player_id, x2, y2);
            wallsAmount--;
            if (!aZvezda(x1, y1, walls, player))
            {
                cout << "Ops, Player" << player << " closed himself!" << endl;
                if (player == 2)
                {
                    swap(x1, x2);
                    swap(y1, y2);
                }
                return true;
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
    if (player == 2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }
    return false;
}

int main()
{
    map<char, funptr> jumps = map<char, funptr>();
    jumps['l'] = jumpLeft;
    jumps['r'] = jumpRight;
    jumps['d'] = jumpDown;
    jumps['u'] = jumpUp;

    int pole[realSize][realSize];	  //Создаём поле
    for (int i = 0; i < realSize; i++) //Заполняем нулями
    {
        for (int j = 0; j < realSize; j++)
        {
            pole[i][j] = -1;
        }
    }

    //Ставим фишки на начальные места
    int coords[3][2] = {{0, realSize - 1}, {(size - 1) / 2, (size - 1) / 2}, {10, 10}};
    // int y[2] = {(size - 1) / 2, (size - 1) / 2};
    pole[x[0]][y[0]] = 1;
    pole[x[1]][y[1]] = 2;

    output(pole);

    //Храним информации о конце игры
    int end = 0;
    int player = 0; // 0 - "первый", 1 - "второй"
    int winner = 0; // 0 - "первый", 1 - "второй"

    while (!end)
    {
        end = turn(pole, coords, walls, player, jumps);
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

bool jumpLeft(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((x == 1 || !isWall(x - 1, y, x - 2, y, walls)) && (y == 8 || !isWall(x - 1, y, x - 1, y + 1, walls)) && (y == 0 || !isWall(x - 1, y, x - 1, y - 1, walls)))
    {
        cout << "Error: you cant jump left!" << endl;
        return !flag;
    }
    if (x != 1 && isWall(x - 1, y, x - 2, y, walls))
    {
        pole[x - 2][y] = p;
        pole[x][y] = 0;
        x -= 2;
        flag = false;
        return !flag;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - down or up (d/u): ";
            char tmp;
            cin >> tmp;
            cout << endl;
            switch (tmp)
            {
            case 'd':
                if (y != 8 && isWall(x - 1, y, x - 1, y + 1, walls))
                {
                    pole[x - 1][y + 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y++;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump down!" << endl;
                break;
            case 'u':
                if (y != 0 && isWall(x - 1, y, x - 1, y - 1, walls))
                {
                    pole[x - 1][y - 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y--;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump up!" << endl;
                break;
            default:
                cout << "Error: wrong char!" << endl;
                break;
            }
        }
    }
    assert(false);
    return !flag;
}

bool jumpRight(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((x == 7 || !isWall(x + 1, y, x + 2, y, walls)) && (y == 8 || !isWall(x + 1, y, x + 1, y + 1, walls)) && (y == 0 || !isWall(x + 1, y, x + 1, y - 1, walls)))
    {
        cout << "Error: you cant jump right!" << endl;
        return !flag;
    }
    if (x != 7 && isWall(x + 1, y, x + 2, y, walls))
    {
        pole[x + 2][y] = p;
        pole[x][y] = 0;
        x += 2;
        flag = false;
        return !flag;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - down or up (d/u): ";
            char tmp;
            cin >> tmp;
            cout << endl;
            switch (tmp)
            {
            case 'd':
                if (y != 8 && isWall(x + 1, y, x + 1, y + 1, walls))
                {
                    pole[x + 1][y + 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y++;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump down!" << endl;
                break;
            case 'u':
                if (y != 0 && isWall(x + 1, y, x + 1, y - 1, walls))
                {
                    pole[x + 1][y - 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y--;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump up!" << endl;
                break;
            default:
                cout << "Error: wrong char!" << endl;
                break;
            }
        }
    }
    assert(false);
    return !flag;
}

bool jumpDown(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((y == 7 || !isWall(x, y + 1, x, y + 2, walls)) && (x == 8 || !isWall(x, y + 1, x + 1, y + 1, walls)) && (y == 0 || !isWall(x, y + 1, x - 1, y + 1, walls)))
    {
        cout << "Error: you cant jump down!" << endl;
        return !flag;
    }
    if (y != 7 && isWall(x, y + 1, x, y + 2, walls))
    {
        pole[x][y + 2] = p;
        pole[x][y] = 0;
        y += 2;
        flag = false;
        return !flag;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - right or left (r/l): ";
            char tmp;
            cin >> tmp;
            cout << endl;
            switch (tmp)
            {
            case 'r':
                if (x != 8 && isWall(x, y + 1, x + 1, y + 1, walls))
                {
                    pole[x + 1][y + 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y++;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump right!" << endl;
                break;
            case 'l':
                if (y != 0 && isWall(x, y + 1, x - 1, y + 1, walls))
                {
                    pole[x - 1][y + 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y++;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump left!" << endl;
                break;
            default:
                cout << "Error: wrong char!" << endl;
                break;
            }
        }
    }
    assert(false);
    return !flag;
}

bool jumpUp(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((y == 1 || !isWall(x, y - 1, x, y - 2, walls)) && (x == 8 || !isWall(x, y - 1, x + 1, y - 1, walls)) && (y == 0 || !isWall(x, y - 1, x - 1, y - 1, walls)))
    {
        cout << "Error: you cant jump up!" << endl;
        return !flag;
    }
    if (y != 1 && isWall(x, y - 1, x, y - 2, walls))
    {
        pole[x][y - 2] = p;
        pole[x][y] = 0;
        y -= 2;
        flag = false;
        return !flag;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - right or left (r/l): ";
            char tmp;
            cin >> tmp;
            cout << endl;
            switch (tmp)
            {
            case 'r':
                if (x != 8 && isWall(x, y - 1, x + 1, y - 1, walls))
                {
                    pole[x + 1][y - 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y--;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump right!" << endl;
                break;
            case 'l':
                if (y != 0 && isWall(x, y - 1, x - 1, y - 1, walls))
                {
                    pole[x - 1][y - 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y--;
                    flag = false;
                    return !flag;
                }
                else
                    cout << "Error: you cant jump left!" << endl;
                break;
            default:
                cout << "Error: wrong char!" << endl;
                break;
            }
        }
    }
    assert(false);
    return !flag;
}

void placeWall(int pole[17][17], int p, int t, int u)
{
    while (true)
    {
        int x, y;
        char dir;
        cout << "Chose x and y to place a wall (x y) and Chose direction of the wall (v/h): ";
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
        case 'v':
            for (int i = 0; i < 3; i++)
            {
                wall[i][0] = x - i - 1;
                wall[i][1] = y;
            }
            break;
        case 'h':
            for (int i = 0; i < 3; i++)
            {
                wall[i][0] = x;
                wall[i][1] = y - i - 1;
            }
            break;
        default:
            cout << "Error: wrong char!" << endl;
            continue;
        }

        for (int i = 0; i < 3; i++)
        {
            if (isWall(wall[i][0], wall[i][1], wall[i][0], wall[i][1], pole))
            {
                cout << "Error: wrong x/y/dir!" << endl;
                continue;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            pole[wall[i][0]][wall[i][1]] = 5;
        }

        if (dir == 'v' || dir == 'h')
            break;
    }
}

bool isWall(int x0, int y0, int x1, int y1, int pole[17][17])
{
    return pole[(x0 + x1) / 2][(y0 + y1) / 2] == w;
}

bool aZvezda(int x, int y, Walls walls, int p)
{
    bool pole[9][9];

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            pole[i][j] = false;
        }
    }

    queue<pair<int, int>> frontier;
    frontier.push(make_pair(x, y));
    pole[x][y] = true;
    pair<int, int> current = frontier.front();
    pair<int, int> next;

    while (!frontier.empty())
    {
        current = frontier.front();

        x = current.first;
        y = current.second;

        next = step(x, y, 'l');
        // cout << frontier.front().first << " " << frontier.front().second << " " << x << " " << y << " " << frontier.empty() << endl;
        if (!pole[next.first][next.second] && isWall(x, y, next.first, next.second, walls) && next.first >= 0)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'u');
        if (!pole[next.first][next.second] && isWall(x, y, next.first, next.second, walls) && next.second >= 0)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'r');
        if (!pole[next.first][next.second] && isWall(x, y, next.first, next.second, walls) && next.first <= 8)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'd');
        if (!pole[next.first][next.second] && isWall(x, y, next.first, next.second, walls) && next.second <= 8)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        /*int pol[9][9];
        for (int i=0; i<9;i++)
        {
            for (int j=0; j<9;j++)
            {
                if (pole[i][j]) pol[i][j]=p;
                else pol[i][j]=0;
            }
        }
        output(walls, pol);
        cout << endl;*/

        frontier.pop();

        for (int j = 0; j < 9; j++)
            if (pole[16 - 8 * p][j])
            {
                // cout << "ok" << endl;
                return true;
            }
    }
    return false;
}
