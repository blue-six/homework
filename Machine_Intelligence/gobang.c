/* 
���ߣ�wpy

�㷨˼·���ԣ�https://github.com/lihongxun945/myblog/issues/11
ai���ִ������ԣ�https://github.com/lihongxun945/gobang
������ֽ���ԣ�https://github.com/lihongxun945/gobang
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

void show_chessboard();                                       //��ʾ���
void put(Loc *l, char name);                                  //����
void input(Loc *l);                                           //����������
Loc *AI();                                                    //AI�������ӷ�λ
int win(Loc *l);                                              //�ж��Ƿ���һ����ʤ
int has_neighbors(Loc *l, int n, int count);                  //�ж���Χ�������
int evaluate(char name);                                      //�������
int _evaluate(Loc *l, char name, int dir);                    //���㵥��λ�õļ�ֵ
int count_score(int count, int empty, int block);             //���ؼ�ֵ
char h_start();                                               //��ֿ�ʼ
int ab_cut(int deep, Loc *l, int alpha, int beta, char name); //��֦
P *generate(char name);                                       //��������λ��
void remove_(Loc *l);                                         //�Ƴ�����
int fix_score(int score);                                     //���ƴ��

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
            printf("��һ�ʤ!\n");
            break;
        }
        if (win(AI()))
        {
            show_chessboard();
            printf("�������ʤ!\n");
            break;
        }
    }
    return 1;
}

//��ֿ�ʼ
char h_start()
{
    char c;
    printf("���������ַ�: c:����� h:���\n");
    while (1)
    {
        scanf("\n%c", &c);
        if (c == 'c' || c == 'h')
            break;
        else
            printf("�������������!����������:\n");
    }
    return c;
}

//��ʾ���
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
    printf("���: #    �����: +\n");
}

//����������
void input(Loc *l)
{
    int x, y;
    printf("���������ӵ�λ�ã�\n(x y)---- 1 a\n");
    while (1)
    {
        scanf("%ud", &x);
        scanf("\n%c", (char *)&y);
        x--, y &= 0xff, y = y - 'a';
        DEBUG &&printf("����Ϊ:x =%3d  y =%3d\n", x, y);
        if (x >= 0 && x < 15 && y >= 0 && y < 15) //����Ƿ���Ϸ�Χ
            break;
        else
            printf("�������������!����������:\n");
    }
    l->x = x, l->y = y;
}

//����
void put(Loc *l, char name)
{
    DEBUG &&printf("����λ��Ϊ:x =%3d  y =%3d\n", l->x, l->y);
    CS[l->x][l->y] = name;
}

//�Ƴ�����
void remove_(Loc *l)
{
    DEBUG &&printf("�Ƴ�����λ��Ϊ:x =%3d  y =%3d\n", l->x, l->y);
    CS[l->x][l->y] = 0;
}

//�ж��Ƿ���һ����ʤ
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

//�ж���Χ�������
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

//AI�������ӷ�λ
Loc *AI()
{
    static Loc l = {0, 0};
    return &l;
}

//�������
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

//���㵥��λ�õļ�ֵ
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

//���ؼ�ֵ
int count_score(int count, int empty, int block)
{

    //û�п�λ
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
        //��1���ǿ�λ
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
        //�ڶ����ǿ�λ
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

//��֦
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

//��������λ��
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

//���ƴ��
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
