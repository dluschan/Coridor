#include <iostream>

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

bool checkWall(bool ver, int c1, int c2, int z, Walls walls);

bool checkPlaceWall(bool ver, int x, int y, Walls walls);

void jumpLeft(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p);

void jumpRight(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p);

void jumpDown(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p);

void jumpUp(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p);

void placeWall(Walls& walls);

void Output(const Walls& walls, int pole[9][9])
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
                    if (walls.pos[j][i] && !walls.hor[j][i] || walls.pos[j][i - 1] && !walls.hor[j][i - 1])
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
        for (int j = 0; j < 8; j++)
        {
            if (j != 0 && j != 8)
            {
                if (walls.pos[j][i] && walls.hor[j][i] || walls.pos[j - 1][i] && walls.hor[j - 1][i])
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

void Turn(int pole[9][9], int& y1, int& y2, int& x1, int x2, int& wallsAmount1, Walls& walls, bool& flag, int& end, int player)
{
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
                    if (x1 - 1 != x2 || y1 != y2)
                    {
                        if (x1 != 0 && checkWall(false, x1, x1 - 1, y1, walls))
                        {
                            pole[x1 - 1][y1] = player;
                            pole[x1][y1] = 0;
                            x1--;
                            flag = false;
                        }
                        else
                            cout << "Error: you cant move left!\n";
                    }
                    else if (checkWall(false, x1, x1 - 1, y1, walls))
                        jumpLeft(x1, y1, walls, ans, pole, flag, player);
                    else
                        cout << "Error: you cant move left!\n";
                    break;
                case 'r':
                    if (x1 + 1 != x2 || y1 != y2)
                    {
                        if (x1 != 8 && checkWall(false, x1, x1 + 1, y1, walls))
                        {
                            pole[x1 + 1][y1] = player;
                            pole[x1][y1] = 0;
                            x1++;
                            flag = false;
                        }
                        else
                            cout << "Error: you cant move right!\n";
                    }
                    else if (checkWall(false, x1, x1 + 1, y1, walls))
                        jumpRight(x1, y1, walls, ans, pole, flag, player);
                    else
                        cout << "Error: you cant move right!\n";
                    break;
                case 'd':
                    if (y1 + 1 != y2 || x1 != x2)
                    {
                        if (y1 != 8 && checkWall(true, y1, y1 + 1, x1, walls))
                        {
                            pole[x1][y1 + 1] = player;
                            pole[x1][y1] = 0;
                            y1++;
                            flag = false;
                        }
                        else
                            cout << "Error: you cant move down!\n";
                    }
                    else if (checkWall(true, y1, y1 + 1, x1, walls))
                        jumpDown(x1, y1, walls, ans, pole, flag, player);
                    else
                        cout << "Error: you cant move down!\n";
                    break;
                case 'u':
                    if (y1 - 1 != y2 || x1 != x2)
                    {
                        if (y1 != 0 && checkWall(true, y1, y1 - 1, x1, walls))
                        {
                            pole[x1][y1 - 1] = player;
                            pole[x1][y1] = 0;
                            y1--;
                            flag = false;
                        }
                        else
                            cout << "Error: you cant move up!\n";
                    }
                    else if (checkWall(true, y1, y1 - 1, x1, walls))
                        jumpUp(x1, y1, walls, ans, pole, flag, player);
                    else
                        cout << "Error: you cant move up!\n";
                    break;
                default:
                    cout << "Error: wrong char!\n";
                    break;
                }
            }
            for (int j = 0; j < 9; j++)
                if (pole[16 - 8 * player][j] == player)
                    end = player; //Проверка на победу
            break;
        case 'w':
            if (wallsAmount1 == 0)
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
}

int main()
{
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
    pole[8][4] = 2;
    int x2 = 8;
    int y2 = 4;

    //Храним колличество стенок каждого игрока
    int wallsAmount1 = 10;
    int wallsAmount2 = 10;

    //Создём данные хранящие положения стенок
    Walls walls;

    Output(walls, pole);

    //Храним информации о конце игры
    int end = 0;

    bool flag = true;

    while (end == 0)
    {
        // Player1 ходит
        Turn(pole, y1, y2, x1, x2, wallsAmount1, walls, flag, end, 1);

        if (end == 1)
            break; //Если Player1 победил
        flag = true;

        Output(walls, pole);

        // Player2 ходит
        Turn(pole, y2, y1, x2, x1, wallsAmount2, walls, flag, end, 2);

        flag = true;

        Output(walls, pole);
    }
    cout << "Player" << end << " wins!" << endl;
    return 0;
}

void jumpLeft(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p)
{
    if ((x == 1 || !checkWall(false, x - 1, x - 2, y, walls)) && (y == 8 || !checkWall(true, y, y + 1, x - 1, walls)) && (y == 0 || !checkWall(true, y, y - 1, x - 1, walls)))
    {
        cout << "Error: you cant jump left!\n";
        return;
    }
    if (x != 1 && checkWall(false, x - 1, x - 2, y, walls))
    {
        pole[x - 2][y] = p;
        pole[x][y] = 0;
        x -= 2;
        flag = false;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - down or up (d/u): ";
            cin >> tmp;
            cout << '\n';
            switch (tmp)
            {
            case 'd':
                if (y != 8 && checkWall(true, y, y + 1, x - 1, walls))
                {
                    pole[x - 1][y + 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y++;
                    flag = false;
                }
                else
                    cout << "Error: you cant jump down!\n";
                break;
            case 'u':
                if (y != 0 && checkWall(true, y, y - 1, x - 1, walls))
                {
                    pole[x - 1][y - 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y--;
                    flag = false;
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

void jumpRight(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p)
{
    if ((x == 7 || !checkWall(false, x + 1, x + 2, y, walls)) && (y == 8 || !checkWall(true, y, y + 1, x + 1, walls)) && (y == 0 || !checkWall(true, y, y - 1, x + 1, walls)))
    {
        cout << "Error: you cant jump left!\n";
        return;
    }
    if (x != 7 && checkWall(false, x + 1, x + 2, y, walls))
    {
        pole[x + 2][y] = p;
        pole[x][y] = 0;
        x += 2;
        flag = false;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - down or up (d/u): ";
            cin >> tmp;
            cout << '\n';
            switch (tmp)
            {
            case 'd':
                if (y != 8 && checkWall(true, y, y + 1, x + 1, walls))
                {
                    pole[x + 1][y + 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y++;
                    flag = false;
                }
                else
                    cout << "Error: you cant jump down!\n";
                break;
            case 'u':
                if (y != 0 && checkWall(true, y, y - 1, x + 1, walls))
                {
                    pole[x + 1][y - 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y--;
                    flag = false;
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

void jumpDown(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p)
{
    if ((y == 7 || !checkWall(true, y + 1, y + 2, x, walls)) && (x == 8 || !checkWall(false, x, x + 1, y + 1, walls)) && (y == 0 || !checkWall(false, x, x + 1, y + 1, walls)))
    {
        cout << "Error: you cant jump left!\n";
        return;
    }
    if (y != 7 && checkWall(true, y + 1, y + 2, x, walls))
    {
        pole[x][y + 2] = p;
        pole[x][y] = 0;
        y += 2;
        flag = false;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - right or left (r/l): ";
            cin >> tmp;
            cout << '\n';
            switch (tmp)
            {
            case 'r':
                if (x != 8 || checkWall(false, x, x + 1, y + 1, walls))
                {
                    pole[x + 1][y + 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y++;
                    flag = false;
                }
                else
                    cout << "Error: you cant jump down!\n";
                break;
            case 'l':
                if (y != 0 || checkWall(false, x, x + 1, y + 1, walls))
                {
                    pole[x - 1][y + 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y++;
                    flag = false;
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

void jumpUp(int& x, int& y, Walls walls, char& tmp, int pole[9][9], bool& flag, int p)
{
    if ((y == 1 || !checkWall(true, y - 1, y - 2, x, walls)) && (x == 8 || !checkWall(false, x, x + 1, y - 1, walls)) && (y == 0 || !checkWall(false, x, x + 1, y - 1, walls)))
    {
        cout << "Error: you cant jump left!\n";
        return;
    }
    if (y != 7 && checkWall(true, y + 1, y + 2, x, walls))
    {
        pole[x][y - 2] = p;
        pole[x][y] = 0;
        y -= 2;
        flag = false;
    }
    else
    {
        while (flag)
        {
            cout << "Chose direction to jump - right or left (r/l): ";
            cin >> tmp;
            cout << '\n';
            switch (tmp)
            {
            case 'r':
                if (x != 8 || checkWall(false, x, x + 1, y - 1, walls))
                {
                    pole[x + 1][y - 1] = p;
                    pole[x][y] = 0;
                    x++;
                    y--;
                    flag = false;
                }
                else
                    cout << "Error: you cant jump down!\n";
                break;
            case 'l':
                if (y != 0 || checkWall(false, x, x + 1, y - 1, walls))
                {
                    pole[x - 1][y - 1] = p;
                    pole[x][y] = 0;
                    x--;
                    y--;
                    flag = false;
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

bool checkWall(bool ver, int c1, int c2, int z, Walls walls)
{
    int c = 0;
    if (c1 < c2)
        c = c1;
    else
        c = c2;
    if (!ver)
    {
        if (z != 0 && z != 8)
        {
            if (walls.pos[c][z - 1] && !walls.hor[c][z - 1] || walls.pos[c][z] && !walls.hor[c][z])
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
            if (walls.pos[z - 1][c] && walls.hor[z - 1][c] || walls.pos[z][c] && walls.hor[z][c])
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
