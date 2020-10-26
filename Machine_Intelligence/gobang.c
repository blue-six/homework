/* 
作者：wpy

算法思路来自：https://github.com/lihongxun945/myblog/issues/11
ai部分代码借鉴自：https://github.com/lihongxun945/gobang
棋局评分借鉴自：https://github.com/lihongxun945/gobang
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define CS chessboard
#define MAX FIVE * 10
#define MIX -FIVE * 10

typedef struct location
{
    unsigned int x : 8, y : 8;
} Loc;

typedef struct points
{
    unsigned int x : 8, y : 8;
    int score;
    struct points *next;
} P;

enum score
{
    ONE = 10,
    TWO = 100,
    THREE = 1000,
    FOUR = 100000,
    FIVE = 10000000,
    BLOCKED_ONE = 1,
    BLOCKED_TWO = 10,
    BLOCKED_THREE = 100,
    BLOCKED_FOUR = 10000
};

char chessboard[15][15] = {0};
int hum_score[15][15] = {0}, com_score[15][15] = {0};

void show_chessboard();                                       //显示棋局
void put(Loc *l, char name);                                  //落子
void input(Loc *l);                                           //获得玩家输入
Loc *AI();                                                    //AI计算落子方位
int win(Loc *l);                                              //判断是否有一方获胜
int has_neighbors(Loc *l, int n, int count);                  //判断周围落子情况
int evaluate(char name);                                      //评估棋局
int _evaluate(Loc *l, char name, int dir);                    //估算单个位置的价值
int count_score(int count, int empty, int block);             //返回价值
char h_start();                                               //棋局开始
int ab_cut(int deep, Loc *l, int alpha, int beta, char name); //剪枝
P *generate(char name);                                       //生成落子位置
void remove_(Loc *l);                                         //移除棋子
int fix_score(int score);                                     //完善打分

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
            show_chessboard();
            printf("玩家获胜!\n");
            break;
        }
        if (win(AI()))
        {
            show_chessboard();
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

//移除棋子
void remove_(Loc *l)
{
    DEBUG &&printf("移除棋子位置为:x =%3d  y =%3d\n", l->x, l->y);
    CS[l->x][l->y] = 0;
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
    return &l;
}

//评估棋局
int evaluate(char name)
{
    int com_max = 0, hum_max = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (CS[i][j] == 'c')
                com_max += fix_score(com_score[i][j]);
            else if (CS[i][j] == 'h')
                hum_max += fix_score(hum_score[i][j]);
        }
    }
    return name == 'c' ? com_max - hum_max : hum_max - com_max;
}

//估算单个位置的价值
int _evaluate(Loc *l, const char name, const int dir)
{
    int value = 0, count = 1, block = 0, empty = -1;
    const int x = l->x, y = l->y;
    char now;
    if (dir == -1 || dir == 0)
    {
        for (int i = y + 1;; i++)
        {
            if (i > 14)
            {
                block++;
                break;
            }
            now = CS[x][i];
            if (now == name)
                count++;
            else if (now == 0)
            {
                if (empty == -1 && i < 14 && CS[x][i + 1] == name)
                    empty = count;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        for (int i = y - 1;; i--)
        {
            if (i < 0)
            {
                block++;
                break;
            }
            now = CS[x][i];
            if (now == name)
                count++, empty != -1 && empty++;
            else if (now == 0)
            {
                if (empty == -1 && i > 0 && CS[x][i - 1] == name)
                    empty = 0;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        value += count_score(count, empty, block);
    }
    if (dir == -1 || dir == 1)
    {
        count = 1, block = 0, empty = -1;
        for (int i = x + 1;; i++)
        {
            if (i > 14)
            {
                block++;
                break;
            }
            now = CS[i][y];
            if (now == name)
                count++;
            else if (now == 0)
            {
                if (empty == -1 && i < 14 && CS[i + 1][y] == name)
                    empty = count;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        for (int i = x - 1;; i--)
        {
            if (i < 0)
            {
                block++;
                break;
            }
            now = CS[i][y];
            if (now == name)
                count++, empty != -1 && empty++;
            else if (now == 0)
            {
                if (empty == -1 && i > 0 && CS[i - 1][y] == name)
                    empty = 0;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        value += count_score(count, empty, block);
    }
    if (dir == -1 || dir == 2)
    {
        count = 1, block = 0, empty = -1;
        for (int i = 1;; i++)
        {
            int px = x + i, py = y + i;
            if (px > 14 || py > 14)
            {
                block++;
                break;
            }
            now = CS[px][py];
            if (now == name)
                count++;
            else if (now == 0)
            {
                if (empty == -1 && px < 14 && py < 14 && CS[px + 1][py + 1] == name)
                    empty = count;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        for (int i = 1;; i++)
        {
            int px = x - i, py = y - i;
            if (px < 0 || py < 0)
            {
                block++;
                break;
            }
            now = CS[px][py];
            if (now == name)
                count++, empty != -1 && empty++;
            else if (now == 0)
            {
                if (empty == -1 && px > 0 && py > 0 && CS[px - 1][py - 1] == name)
                    empty = 0;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        value += count_score(count, empty, block);
    }
    if (dir == -1 || dir == 3)
    {
        count = 1, block = 0, empty = -1;
        for (int i = 1;; i++)
        {
            int px = x + i, py = y - i;
            if (px > 14 || py < 0)
            {
                block++;
                break;
            }
            now = CS[px][py];
            if (now == name)
                count++;
            else if (now == 0)
            {
                if (empty == -1 && px < 14 && py > 0 && CS[px + 1][py - 1] == name)
                    empty = count;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        for (int i = 1;; i++)
        {
            int px = x - i, py = y + i;
            if (px < 0 || py > 14)
            {
                block++;
                break;
            }
            now = CS[px][py];
            if (now == name)
                count++, empty != -1 && empty++;
            else if (now == 0)
            {
                if (empty == -1 && px > 0 && py < 14 && CS[px - 1][py + 1] == name)
                    empty = 0;
                else
                    break;
            }
            else
            {
                block++;
                break;
            }
        }
        value += count_score(count, empty, block);
    }
    return value;
}

//返回价值
int count_score(int count, int empty, int block)
{

    //没有空位
    if (empty <= 0)
    {
        if (count >= 5)
            return FIVE;
        if (block == 0)
        {
            switch (count)
            {
            case 1:
                return ONE;
            case 2:
                return TWO;
            case 3:
                return THREE;
            case 4:
                return FOUR;
            }
        }

        if (block == 1)
        {
            switch (count)
            {
            case 1:
                return BLOCKED_ONE;
            case 2:
                return BLOCKED_TWO;
            case 3:
                return BLOCKED_THREE;
            case 4:
                return BLOCKED_FOUR;
            }
        }
    }
    else if (empty == 1 || empty == count - 1)
    {
        //第1个是空位
        if (count >= 6)
        {
            return FIVE;
        }
        if (block == 0)
        {
            switch (count)
            {
            case 2:
                return TWO / 2;
            case 3:
                return THREE;
            case 4:
                return BLOCKED_FOUR;
            case 5:
                return FOUR;
            }
        }

        if (block == 1)
        {
            switch (count)
            {
            case 2:
                return BLOCKED_TWO;
            case 3:
                return BLOCKED_THREE;
            case 4:
                return BLOCKED_FOUR;
            case 5:
                return BLOCKED_FOUR;
            }
        }
    }
    else if (empty == 2 || empty == count - 2)
    {
        //第二个是空位
        if (count >= 7)
        {
            return FIVE;
        }
        if (block == 0)
        {
            switch (count)
            {
            case 3:
                return THREE;
            case 4:
            case 5:
                return BLOCKED_FOUR;
            case 6:
                return FOUR;
            }
        }

        if (block == 1)
        {
            switch (count)
            {
            case 3:
                return BLOCKED_THREE;
            case 4:
                return BLOCKED_FOUR;
            case 5:
                return BLOCKED_FOUR;
            case 6:
                return FOUR;
            }
        }

        if (block == 2)
        {
            switch (count)
            {
            case 4:
            case 5:
            case 6:
                return BLOCKED_FOUR;
            }
        }
    }
    else if (empty == 3 || empty == count - 3)
    {
        if (count >= 8)
        {
            return FIVE;
        }
        if (block == 0)
        {
            switch (count)
            {
            case 4:
            case 5:
                return THREE;
            case 6:
                return BLOCKED_FOUR;
            case 7:
                return FOUR;
            }
        }

        if (block == 1)
        {
            switch (count)
            {
            case 4:
            case 5:
            case 6:
                return BLOCKED_FOUR;
            case 7:
                return FOUR;
            }
        }

        if (block == 2)
        {
            switch (count)
            {
            case 4:
            case 5:
            case 6:
            case 7:
                return BLOCKED_FOUR;
            }
        }
    }
    else if (empty == 4 || empty == count - 4)
    {
        if (count >= 9)
        {
            return FIVE;
        }
        if (block == 0)
        {
            switch (count)
            {
            case 5:
            case 6:
            case 7:
            case 8:
                return FOUR;
            }
        }

        if (block == 1)
        {
            switch (count)
            {
            case 4:
            case 5:
            case 6:
            case 7:
                return BLOCKED_FOUR;
            case 8:
                return FOUR;
            }
        }

        if (block == 2)
        {
            switch (count)
            {
            case 5:
            case 6:
            case 7:
            case 8:
                return BLOCKED_FOUR;
            }
        }
    }
    else if (empty == 5 || empty == count - 5)
        return FIVE;
    return 0;
}

//剪枝
int ab_cut(int deep, Loc *l, int alpha, int beta, char name)
{
    int value = evaluate(name), score;
    P *points;
    Loc ll;
    if (deep <= 0 || value >= FIVE || value <= -FIVE)
        return value;
    points = generate(name);
    for (P *p = points->next; p != NULL; p = p->next)
    {
        ll.x = p->x, ll.y = p->y;
        put(&ll, name);
        score = -ab_cut(deep - 1, &ll, -beta, -alpha, name == 'c' ? 'h' : 'c');
        remove_(&ll);
        if (score > alpha)
            alpha = score;
        if (score > beta)
        {
            return MAX - 1;
        }
    }
    return alpha;
}

//生成落子位置
P *generate(char name)
{
    P *head = (P *)malloc(sizeof(P)), *p = head;
    head->next = NULL;
    Loc a;
    for (int i = 0; i < 15; i++)
    {
        a.x = i;
        for (int j = 0; j < 15; j++)
        {
            if (CS[i][j] == 0)
            {
                a.y = j;
                if (!has_neighbors(&a, 1, 1))
                    continue;
                if (!has_neighbors(&a, 2, 2))
                    continue;
            }
        }
    }
    return p;
}

void add()
{
    return;
}

//完善打分
int fix_score(int score)
{
    if (score < FOUR && score >= BLOCKED_FOUR)
    {

        if (score >= BLOCKED_FOUR && score < (BLOCKED_FOUR + THREE))
            return THREE;
        else if (score >= BLOCKED_FOUR + THREE && score < BLOCKED_FOUR * 2)
            return FOUR;
        else
            return FOUR * 2;
    }
    return score;
}
