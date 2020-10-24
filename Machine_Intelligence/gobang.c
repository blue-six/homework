#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define CS chessboard

typedef struct location
{
    unsigned int x : 8, y : 8;
} Loc;

char chessboard[15][15] = {0};

void show_chessboard();                      //显示棋局
void put(Loc *l, char name);                 //落子
void input(Loc *l);                          //获得玩家输入
Loc *AI();                                   //AI计算落子方位
int win(Loc *l);                             //判断是否有一方获胜
int has_neighbors(Loc *l, int n, int count); //判断周围落子情况
int evaluate(char name);                     //评估棋局
int _evaluate(Loc *l, char name);            //估算单个位置的价值
char h_start();                              //棋局开始

int main()
{
    Loc l = {7, 7};
    if (h_start() == 'c')
        put(&l, 'c');
    while (1)
    {
        show_chessboard();
        input(&l), put(&l, 'h');
        if (win(&l))
        {
            printf("玩家获胜!\n");
            break;
        }
        if (win(AI()))
        {
            printf("计算机获胜!\n");
            break;
        }
    }
    return 1;
}

//棋局开始
char h_start()
{
    char c;
    printf("请输入先手方: c:计算机 h:玩家\n");
    while (1)
    {
        scanf("\n%c", &c);
        if (c == 'c' || c == 'h')
            break;
        else
            printf("输入的数据有误!请重新输入:\n");
    }
    return c;
}

//显示棋局
void show_chessboard()
{
    printf("    a  b  c  d  e  f  g  h  i  j  k  l  m  n  o\n");
    for (int i = 0; i < 15; i++)
    {
        printf("%3d ", i + 1);
        for (int j = 0; j < 15; j++)
        {
            if (CS[i][j] == 'h')
                printf("#  ");
            else if (CS[i][j] == 'c')
                printf("+  ");
            else
                printf("   ");
        }
        printf("\n");
    }
    printf("玩家: #    计算机: +\n");
}

//获得玩家输入
void input(Loc *l)
{
    int x, y;
    printf("请输入落子的位置：\n(x y)---- 1 a\n");
    while (1)
    {
        scanf("%ud", &x);
        scanf("\n%c", (char *)&y);
        x--, y &= 0xff, y = y - 'a';
        DEBUG &&printf("输入为:x =%3d  y =%3d\n", x, y);
        if (x >= 0 && x < 15 && y >= 0 && y < 15) //检查是否符合范围
            break;
        else
            printf("输入的数据有误!请重新输入:\n");
    }
    l->x = x, l->y = y;
}

//落子
void put(Loc *l, char name)
{
    DEBUG &&printf("落子位置为:x =%3d  y =%3d\n", l->x, l->y);
    CS[l->x][l->y] = name;
}

//判断是否有一方获胜
int win(Loc *l)
{
    char c = CS[l->x][l->y];
    short x = l->x, y = l->y, count = 0;
    for (short i = x - 5; i < x + 5; i++)
    {
        if (i > 14)
            break;
        if (i < 0)
            continue;
        else if (CS[i][y] == c)
            count++;
        else if (i < x)
            count = 0;
        else
            break;
    }
    if (count >= 5)
        return 1;
    count = 0;
    for (short i = y - 5; i < y + 5; i++)
    {
        if (i > 14)
            break;
        if (i < 0)
            continue;
        else if (CS[x][i] == c)
            count++;
        else if (i < y)
            count = 0;
        else
            break;
    }
    if (count >= 5)
        return 1;
    count = 0;
    for (short i = x - 4, j = y - 4; i < x + 5 && j < y + 5; i++, j++)
    {
        if (i > 14 || j > 14)
            break;
        if (i < 0 || j < 0)
            continue;
        else if (CS[i][j] == c)
            count++;
        else if (i < x)
            count = 0;
        else
            break;
    }
    if (count >= 5)
        return 1;
    count = 0;
    for (short i = x - 4, j = y + 4; i < x + 5 && j > y - 5; i++, j--)
    {
        if (i > 14 || j < 0)
            break;
        if (i < 0 || j > 14)
            continue;
        else if (CS[i][j] == c)
            count++;
        else if (i < x)
            count = 0;
        else
            break;
    }
    if (count >= 5)
        return 1;
    return 0;
}

//判断周围落子情况
int has_neighbors(Loc *l, int n, int count)
{
    for (short i = l->x - n; i <= l->x + n; i++)
    {
        if (i < 0 || i > 14)
            continue;
        for (short j = l->y - n; j <= l->y + n; j++)
        {
            if (j < 0 || j > 14)
                continue;
            else if (CS[i][j] != 0 && --count <= 0)
                return 1;
        }
    }
    return 0;
}

//AI计算落子方位
Loc *AI()
{
    static Loc l = {0, 0};
}
