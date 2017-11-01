#include <assert.h>
#include <iostream>
#include <map>

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

void placeWall(Walls& walls);

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
        cout << "\n";
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
        cout << "\n";
    }
}

std::pair<int, int> step(int x, int y, char ch)
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

bool turn(int pole[9][9], int& y1, int& y2, int& x1, int& x2, int& wallsAmount, Walls& walls, int player, std::map<char, funptr> jumps)
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
        cout << '\n';
        switch (ans)
        {
        case 'm':
            while (flag)
            {
                cout << "Chose direction - left, right, down or up (l/r/d/u): ";
                cin >> ans;
                cout << '\n';
                switch (ans)
                {
                case 'l':
                case 'r':
                case 'd':
                case 'u':
                {
                    std::pair<int, int> coords = step(x1, y1, ans);
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
                        if (jumps[ans](x1, y1, walls, pole, player)) flag = false;
                    }
                    break;
                }
                default:
                    cout << "Error: wrong char!\n";
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
                cout << "Error: Player" << player << " out of walls!\n";
                break;
            }
            placeWall(walls);
            flag = false;
            break;
        default:
            cout << "Error: wrong char!\n";
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
    std::map<char, funptr> jumps = std::map<char, funptr>();
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
        player = (player + 1) % 2;
        end = turn(pole, y1, y2, x1, x2, wallsAmount[player], walls, player + 1, jumps);
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
        cout << "Error: you cant jump left!\n";
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
            cout << '\n';
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
                    cout << "Error: you cant jump down!\n";
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
                    cout << "Error: you cant jump up!\n";
                break;
            default:
                cout << "Error: wrong char!\n";
                break;
            }
        }
    }
}

bool jumpRight(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((x == 7 || !checkWall(x + 1, y, x + 2, y, walls)) && (y == 8 || !checkWall(x + 1, y, x + 1, y + 1, walls)) && (y == 0 || !checkWall(x + 1, y, x + 1, y - 1, walls)))
    {
        cout << "Error: you cant jump right!\n";
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
            cout << '\n';
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
                    cout << "Error: you cant jump down!\n";
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
                    cout << "Error: you cant jump up!\n";
                break;
            default:
                cout << "Error: wrong char!\n";
                break;
            }
        }
    }
}

bool jumpDown(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag = true;
    if ((y == 7 || !checkWall(x, y + 1, x, y + 2, walls)) && (x == 8 || !checkWall(x, y + 1, x + 1, y + 1, walls)) && (y == 0 || !checkWall(x, y + 1, x - 1, y + 1, walls)))
    {
        cout << "Error: you cant jump down!\n";
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
            cout << '\n';
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
                    cout << "Error: you cant jump right!\n";
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
                    cout << "Error: you cant jump left!\n";
                break;
            default:
                cout << "Error: wrong char!\n";
                break;
            }
        }
    }
}

bool jumpUp(int& x, int& y, Walls walls, int pole[9][9], int p)
{
    bool flag=true;
    if ((y == 1 || !checkWall(x, y - 1, x, y - 2, walls)) && (x == 8 || !checkWall(x, y - 1, x + 1, y - 1, walls)) && (y == 0 || !checkWall(x, y - 1, x - 1, y - 1, walls)))
    {
        cout << "Error: you cant jump up!\n";
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
            cout << '\n';
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
                    cout << "Error: you cant jump right!\n";
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
                    cout << "Error: you cant jump left!\n";
                break;
            default:
                cout << "Error: wrong char!\n";
                break;
            }
        }
    }
}

void placeWall(Walls& walls)
{
    int x, y;
    cout << "Chose x and y to place a wall (x y): ";
    cin >> x >> y;
    cout << '\n';

    if (walls.pos[x][y] == true || x < 0 || x > 7 || y < 0 || y > 7)
    {
        cout << "Error: wrong x or/and y!\n\n";
        placeWall(walls);
        return;
    }

    char dir;

    while (true)
    {
        cout << "Chose direction of the wall (v/h): ";
        cin >> dir;
        cout << '\n';

        switch (dir)
        {
        case 'v':
            if (!checkPlaceWall(true, x, y, walls))
            {
                cout << "Error: you can't place wall like that!\n";
                placeWall(walls);
                return;
            }
            break;
        case 'h':
            if (!checkPlaceWall(false, x, y, walls))
            {
                cout << "Error: you can't place wall like that!\n";
                placeWall(walls);
                return;
            }
            break;
        default:
            break;
        }

        switch (dir)
        {
        case 'v':
            walls.pos[x][y] = true;
            walls.hor[x][y] = false;
            break;
        case 'h':
            walls.pos[x][y] = true;
            walls.hor[x][y] = true;
            break;
        default:
            cout << "Error: wrong char!\n";
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
