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

void show_chessboard();                      //��ʾ���
void put(Loc *l, char name);                 //����
void input(Loc *l);                          //����������
Loc *AI();                                   //AI�������ӷ�λ
int win(Loc *l);                             //�ж��Ƿ���һ����ʤ
int has_neighbors(Loc *l, int n, int count); //�ж���Χ�������
int evaluate(char name);                     //�������
int _evaluate(Loc *l, char name);            //���㵥��λ�õļ�ֵ
char h_start();                              //��ֿ�ʼ

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
            printf("��һ�ʤ!\n");
            break;
        }
        if (win(AI()))
        {
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
}
