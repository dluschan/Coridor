#include <assert.h>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

struct Walls
{
    bool pos[8][8];
    bool hor[8][8];
    Walls()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                pos[i][j] = false;
            }
        }
    }
};

typedef bool (*funptr)(int&, int&, Walls, int[9][9], int);

// std::map<char, funptr> jumps = std::map<char, funptr>();

bool checkWall(int x0, int x1, int y0, int y1, Walls walls);

bool checkPlaceWall(bool ver, int x, int y, Walls walls);

bool jumpLeft(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpRight(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpDown(int& x, int& y, Walls walls, int pole[9][9], int p);

bool jumpUp(int& x, int& y, Walls walls, int pole[9][9], int p);

void placeWall(Walls& walls, int p, int t, int u);

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
        return make_pair(x - 1, y);
    case 'r':
        return make_pair(x + 1, y);
    case 'd':
        return make_pair(x, y + 1);
    case 'u':
        return make_pair(x, y - 1);
    default:
        assert(false);
        return make_pair(0, 0);
    }
}

bool turn(int pole[9][9], int& y1, int& y2, int& x1, int& x2, int& wallsAmount, Walls& walls, int& player, std::map<char, funptr> jumps)
{
    if (player == 2)
    {
        swap(x1, x2);
        swap(y1, y2);
    }
    bool flag = true;
    while (flag)
    {
        cout << "Turn of Player" << player << ", chose move or place a wall (m/w): ";
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
                    pair<int, int> coords = step(x1, y1, ans);
                    int new_x = coords.first;
                    int new_y = coords.second;
                    if (!(new_x >= 0 && new_x <= 8 && new_y >= 0 && new_y <= 8) || !checkWall(x1, y1, new_x, new_y, walls))
                    {
                        cout << "Error: you cant move here" << endl;
                        break;
                    }
                    if (new_x != x2 || new_y != y2)
                    {
                        pole[new_x][new_y] = player;
                        pole[x1][y1] = 0;
                        x1 = new_x;
                        y1 = new_y;
                        flag = false;
                    }
                    else
                    {
                        if (jumps[ans](x1, y1, walls, pole, player))
                            flag = false;
                    }
                    break;
                }
                default:
                    cout << "Error: wrong char!" << endl;
                    break;
                }
            }
            for (int j = 0; j < 9; j++)
                if (pole[16 - 8 * player][j] == player)
                {
                    if (player == 2)
                    {
                        swap(x1, x2);
                        swap(y1, y2);
                    }
                    return true; //Проверка на победу
                }
            break;
        case 'w':
            if (wallsAmount == 0)
            {
                cout << "Error: Player" << player << " out of walls!" << endl;
                break;
            }
            placeWall(walls, 3-player, x2, y2);
            wallsAmount--;
            if (!aZvezda(x1, y1, walls, player))
            {
                cout << "Ops, Player" << player << " closed himself!" << endl;
                if (player == 2)
                {
                    swap(x1, x2);
                    swap(y1, y2);
                }
                player = 3 - player;
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

    int pole[9][9];				//Создаём поле
    for (int i = 0; i < 9; i++) //Заполняем нулями
    {
        for (int j = 0; j < 9; j++)
        {
            pole[i][j] = 0;
        }
    }

    //Ставим фишки на начальные места
    pole[0][4] = 1;
    int x1 = 0;
    int y1 = 4;
    // int x[2]={0, 8};
    // int y[2]={4, 8};
    pole[8][4] = 2;
    int x2 = 8;
    int y2 = 4;

    //Храним колличество стенок каждого игрока
    int wallsAmount[2] = {10, 10};

    //Создём данные хранящие положения стенок
    Walls walls;

    output(walls, pole);

    //Храним информации о конце игры
    bool end = false;
    int player = 1; // 0 - "первый", 1 - "второй"

    while (!end)
    {
        player = (player + 1) % 2+1;
        end = turn(pole, y1, y2, x1, x2, wallsAmount[player], walls, player, jumps);
        player--;
        output(walls, pole);
    }

    cout << "Player" << player + 1 << " wins!" << endl;
    return 0;
}

bool jumpLeft(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((x == 1 || !checkWall(x - 1, y, x - 2, y, walls)) && (y == 8 || !checkWall(x - 1, y, x - 1, y + 1, walls)) && (y == 0 || !checkWall(x - 1, y, x - 1, y - 1, walls)))
    {
        cout << "Error: you cant jump left!" << endl;
        return !flag;
    }
    if (x != 1 && checkWall(x - 1, y, x - 2, y, walls))
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
                if (y != 8 && checkWall(x - 1, y, x - 1, y + 1, walls))
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
                if (y != 0 && checkWall(x - 1, y, x - 1, y - 1, walls))
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
    if ((x == 7 || !checkWall(x + 1, y, x + 2, y, walls)) && (y == 8 || !checkWall(x + 1, y, x + 1, y + 1, walls)) && (y == 0 || !checkWall(x + 1, y, x + 1, y - 1, walls)))
    {
        cout << "Error: you cant jump right!" << endl;
        return !flag;
    }
    if (x != 7 && checkWall(x + 1, y, x + 2, y, walls))
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
                if (y != 8 && checkWall(x + 1, y, x + 1, y + 1, walls))
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
                if (y != 0 && checkWall(x + 1, y, x + 1, y - 1, walls))
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
    if ((y == 7 || !checkWall(x, y + 1, x, y + 2, walls)) && (x == 8 || !checkWall(x, y + 1, x + 1, y + 1, walls)) && (y == 0 || !checkWall(x, y + 1, x - 1, y + 1, walls)))
    {
        cout << "Error: you cant jump down!" << endl;
        return !flag;
    }
    if (y != 7 && checkWall(x, y + 1, x, y + 2, walls))
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
                if (x != 8 && checkWall(x, y + 1, x + 1, y + 1, walls))
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
                if (y != 0 && checkWall(x, y + 1, x - 1, y + 1, walls))
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
    if ((y == 1 || !checkWall(x, y - 1, x, y - 2, walls)) && (x == 8 || !checkWall(x, y - 1, x + 1, y - 1, walls)) && (y == 0 || !checkWall(x, y - 1, x - 1, y - 1, walls)))
    {
        cout << "Error: you cant jump up!" << endl;
        return !flag;
    }
    if (y != 1 && checkWall(x, y - 1, x, y - 2, walls))
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
                if (x != 8 && checkWall(x, y - 1, x + 1, y - 1, walls))
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
                if (y != 0 && checkWall(x, y - 1, x - 1, y - 1, walls))
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

void placeWall(Walls& walls, int p, int t, int u)
{
    int x, y;
    cout << "Chose x and y to place a wall (x y): ";
    cin >> x >> y;
    cout << endl;

    if (walls.pos[x][y] == true || x < 0 || x > 7 || y < 0 || y > 7)
    {
        cout << "Error: wrong x or/and y!\n" << endl;
        placeWall(walls, p, t, u);
        return;
    }

    char dir;

    while (true)
    {
        cout << "Chose direction of the wall (v/h): ";
        cin >> dir;
        cout << endl;

        switch (dir)
        {
        case 'v':
            if (!checkPlaceWall(true, x, y, walls))
            {
                cout << "Error: you can't place wall like that!" << endl;
                placeWall(walls, p, t, u);
                return;
            }
            else
            {
                walls.pos[x][y] = true;
                walls.hor[x][y] = false;
                if (!aZvezda(t, u, walls, p))
                {
                    walls.pos[x][y] = false;
                    cout << "Error: you can't place wall like that (you are blocking the way)!" << endl;
                    placeWall(walls, p, t, u);
                    return;
                }
            }
            break;
        case 'h':
            if (!checkPlaceWall(false, x, y, walls))
            {
                cout << "Error: you can't place wall like that!" << endl;
                placeWall(walls, p, t, u);
                return;
            }
            else
            {
                walls.pos[x][y] = true;
                walls.hor[x][y] = true;
                if (!aZvezda(t, u, walls, p))
                {
                    walls.pos[x][y] = false;
                    cout << "Error: you can't place wall like that (you are blocking the way)!" << endl;
                    placeWall(walls, p, t, u);
                    return;
                }
                //cout << "ok" << endl;
            }
            break;
        default:
            cout << "Error: wrong char!" << endl;
            break;
        }

        if (dir == 'v' || dir == 'h')
            break;
    }
}

bool checkWall(int x0, int y0, int x1, int y1, Walls walls)
{
    bool ver = x0 - x1 == 0;
    int c = (ver) ? min(y0, y1) : min(x0, x1);
    int z = (ver) ? x0 : y0;
    if (!ver)
    {
        if (z != 0 && z != 8)
        {
            if ((walls.pos[c][z - 1] && !walls.hor[c][z - 1]) || (walls.pos[c][z] && !walls.hor[c][z]))
                return false;
            return true;
        }
        else if (z == 0)
        {
            if (walls.pos[c][z] && !walls.hor[c][z])
                return false;
            return true;
        }
        else
        {
            if (walls.pos[c][z - 1] && !walls.hor[c][z - 1])
                return false;
            return true;
        }
    }
    else
    {
        if (z != 0 && z != 8)
        {
            if ((walls.pos[z - 1][c] && walls.hor[z - 1][c]) || (walls.pos[z][c] && walls.hor[z][c]))
                return false;
            return true;
        }
        else if (z == 0)
        {
            if (walls.pos[z][c] && walls.hor[z][c])
                return false;
            return true;
        }
        else
        {
            if (walls.pos[z - 1][c] && walls.hor[z - 1][c])
                return false;
            return true;
        }
    }
}

bool checkPlaceWall(bool ver, int x, int y, Walls walls)
{
    if (!ver)
    {
        if (x != 0)
            if (walls.pos[x - 1][y] == true && walls.hor[x - 1][y])
                return false;
        if (x != 7)
            if (walls.pos[x + 1][y] == true && walls.hor[x + 1][y])
                return false;
    }
    else
    {
        if (y != 0)
            if (walls.pos[x][y - 1] == true && !walls.hor[x][y - 1])
                return false;
        if (y != 7)
            if (walls.pos[x][y + 1] == true && !walls.hor[x][y + 1])
                return false;
    }
    return true;
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
    pair<int, int> current = frontier.front();;
    pair<int, int> next;

    while (!frontier.empty())
    {
        current = frontier.front();

        x = current.first;
        y = current.second;

        next = step(x, y, 'l');
        //cout << frontier.front().first << " " << frontier.front().second << " " << x << " " << y << " " << frontier.empty() << endl;
        if (!pole[next.first][next.second] && checkWall(x, y, next.first, next.second, walls) && next.first >= 0)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'u');
        if (!pole[next.first][next.second] && checkWall(x, y, next.first, next.second, walls) && next.second >= 0)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'r');
        if (!pole[next.first][next.second] && checkWall(x, y, next.first, next.second, walls) && next.first <= 8)
        {
            frontier.push(make_pair(next.first, next.second));
            pole[next.first][next.second] = true;
        }

        next = step(x, y, 'd');
        if (!pole[next.first][next.second] && checkWall(x, y, next.first, next.second, walls) && next.second <= 8)
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
                //cout << "ok" << endl;
                return true;
            }
    }
    return false;
}
