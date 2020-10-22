#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct path
{
    unsigned int x : 8, y : 8, slope : 8;
    unsigned int deep : 9, value : 9;
    unsigned int weight : 9;
    unsigned int closed : 1;
    struct path *next, *parent;
} P;

void createMaze();                         //��������Թ�
void createFreeMaze();                     //�����Զ����Թ�
void createWall();                         //�����Թ���ǽ
int visit_aimless_dfs(int row2, int col2); //äĿ����
int visit_aimless_bfs(int row2, int col2); //äĿ����
int visit_A_star(int row2, int col2);      //A*����
int get_value(int x, int y);               //ȡ��hֵ
P *get(P *head);                           //ȡ��Ȩֵ��С�Ľڵ�
void add(P *head, P *n);                   //��ӽڵ�
P *new_p(int x, int y, int deep);          //�����½ڵ�
int refresh(P *head, P *n);
int main_1(void);
int main_2(void);

int startI = 1,
    startJ = 1; // ���
int success = 0;
//�Թ�����
int maze[100][100];
int row = 0;
int col = 0;
//�Թ�����2����ǽ�ڣ�0����ͨ��

int main(void)
{
    main_1();
}

int main_1(void)
{
    int i, j;

    printf("�������Թ�����row(0<row<100)��");
    scanf("%d", &row);
    printf("�������Թ�����col(0<col<100)��");
    scanf("%d", &col);

    createWall(); //�����Թ���ǽ

    int choice;
    printf("��ѡ�񴴽�����Թ������Զ����Թ���1Ϊ����Թ���2Ϊ�Զ����Թ���:");
    scanf("%d", &choice);
    if (choice == 1)
    {
        createMaze(); //�����Թ�
    }
    else if (choice == 2)
    {
        printf("\n�������Զ����Թ���ǽ�ں�ͨ����2����ǽ�ڣ�0����ͨ��\n");
        createFreeMaze();
    }

    printf("\n��ʾ�Թ���\n");
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            if (maze[i][j] == 2)
                printf("##");
            else
                printf("  ");
        }
        printf("\n");
    }

    if (visit_A_star(startI, startJ) == 0)
    {
        printf("\nû���ҵ����ڣ�\n");
    }
    else
    {
        printf("\n��ʾ·����\n");
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (maze[i][j] == 2)
                    printf("##");
                else if (maze[i][j] == 1)
                    printf("����");
                else
                    printf("  ");
            }
            printf("\n");
        }
    }

    system("pause");

    return 0;
}

int main_2(void)
{
    int i, j;
    row = 20;
    col = 20;

    while (1)
    {
        createWall(); //�����Թ���ǽ
        createMaze(); //�����Թ�
        // printf("\n��ʾ�Թ���\n");
        // for (i = 0; i < row; i++)
        // {
        //     for (j = 0; j < col; j++)
        //     {
        //         if (maze[i][j] == 2)
        //             printf("##");
        //         else
        //             printf("  ");
        //     }
        //     printf("\n");
        // }

        if (visit_A_star(startI, startJ) == 0)
        {
            printf("\nû���ҵ����ڣ�\n");
            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (maze[i][j] == 2)
                        printf("##");
                    else if (maze[i][j] == 1)
                        printf("����");
                    else if (maze[i][j] == 6)
                        printf("kk");
                    else if (maze[i][j] == 5)
                        printf("ww");
                    else
                        printf("  ");
                }
                printf("\n");
            }
        }
        else
        {
            printf("\n��ʾ·����\n");
            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (maze[i][j] == 2)
                        printf("##");
                    else if (maze[i][j] == 1)
                        printf("����");
                    else if (maze[i][j] == 6)
                        printf("k ");
                    else if (maze[i][j] == 5)
                        printf("w ");
                    else
                        printf("  ");
                }
                printf("\n");
            }
        }

        system("pause");
    }

    return 0;
}

//�����Թ���ǽ
void createWall()
{
    //�����Թ���ǽ,��һ�С���һ�С����һ�С����һ�о�Ϊǽ��
    for (int i = 0; i < col; i++) //��һ��
        maze[0][i] = 2;
    for (int i = 1; i < row; i++) //��һ��
        maze[i][0] = 2;
    for (int i = 1; i < col; i++) //���һ��
        maze[row - 1][i] = 2;
    for (int i = 1; i < row - 1; i++) //���һ��
        maze[i][col - 1] = 2;
}

//��������Թ�
void createMaze()
{
    srand(time(0));
    for (int i = 1; i < row - 1; i++)
    {
        for (int j = 1; j < col - 1; j++)
        {
            if ((rand() % 100 + 1) % 4 == 0)
                maze[i][j] = 2;
            else
                maze[i][j] = 0;
        }
    }
    maze[1][1] = 0;
    maze[row - 2][col - 2] = 0;
}

//�����Զ����Թ�
void createFreeMaze()
{
    for (int i = 1; i < row - 1; i++)
    {
        //��һ�У���һ��Ϊ���
        if (i == 1)
        {
            printf("�Թ���%d�У���%d��  ", i, col - 3);
            for (int j = 2; j < col - 1; j++)
                scanf("%d", &maze[i][j]);
        }
        //���һ��,���һ��Ϊ����
        else if (i == row - 2)
        {
            printf("�Թ���%d�У���%d��", i, col - 3);
            for (int j = 1; j < col - 2; j++)
                scanf("%d", &maze[i][j]);
        }
        else
        {
            printf("�Թ���%d�У���%d��", i, col - 2);
            for (int j = 1; j < col - 1; j++)
                scanf("%d", &maze[i][j]);
        }
    }
    maze[1][1] = 0;             //���Ϊͨ��
    maze[row - 2][col - 2] = 0; //����Ϊͨ��
}

int visit_aimless_dfs(int row2, int col2)
{
    if (maze[row2][col2] != 0)
        return 0;
    maze[row2][col2] = 1;
    if (row2 == row - 2 && col2 == row - 2)
        return 1;
    if (visit_aimless_dfs(row2, col2 + 1) || visit_aimless_dfs(row2 + 1, col2) ||
        visit_aimless_dfs(row2 - 1, col2) || visit_aimless_dfs(row2, col2 - 1))
        return 1;
    maze[row2][col2] = 0;
    return 0;
}

int visit_A_star(int row2, int col2)
{
    P *map[row][col];
    memset(map, 0, row * col * sizeof(P *));
    char x, y, flg = 0;
    int deep;
    P *open_list = (P *)malloc(sizeof(P)), *p, *q;
    map[row2][col2] = p = open_list->next = (P *)malloc(sizeof(P));
    p->x = row2, p->y = col2, p->deep = 0, p->next = p->parent = NULL;
    p->slope = (p->y / p->x) - abs(col - col2) / abs(row - row2);
    p->value = get_value(p->x, p->y), p->weight = p->deep + p->value;
    while (open_list->next != NULL)
    {
        p = get(open_list);
        p->closed = 1;
        if (p->x == row - 2 && p->y == col - 2 && (++flg))
            break;
        x = p->x, y = p->y, deep = p->deep;
        if (maze[x + 1][(short)y] != 2)
        {
            if (map[x + 1][(short)y] == 0)
            {
                q = map[x + 1][(short)y] = new_p(x + 1, y, deep + 1);
                add(open_list, q);
                q->parent = map[(short)x][(short)y];
            }
            else
            {
                q = map[x + 1][(short)y];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[(short)x][(short)y];
                }
            }
        }
        if (maze[(short)x][y + 1] != 2)
        {
            if (map[(short)x][y + 1] == 0)
            {
                q = map[(short)x][y + 1] = new_p(x, y + 1, deep + 1);
                add(open_list, q);
                q->parent = map[(short)x][(short)y];
            }
            else
            {
                q = map[(short)x][y + 1];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[(short)x][(short)y];
                }
            }
        }
        if (maze[x - 1][(short)y] != 2)
        {
            if (map[x - 1][(short)y] == 0)
            {
                q = map[x - 1][(short)y] = new_p(x - 1, y, deep + 1);
                add(open_list, q);
                q->parent = map[(short)x][(short)y];
            }
            else
            {
                q = map[x - 1][(short)y];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[(short)x][(short)y];
                }
            }
        }
        if (maze[(short)x][y - 1] != 2)
        {
            if (map[(short)x][y - 1] == 0)
            {
                q = map[(short)x][y - 1] = new_p(x, y - 1, deep + 1);
                add(open_list, q);
                q->parent = map[(short)x][(short)y];
            }
            else
            {
                q = map[(short)x][y - 1];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[(short)x][(short)y];
                }
            }
        }
    }
    if (flg)
    {
        for (q = p; q != NULL; q = q->parent)
            maze[q->x][q->y] = 1;
    }
    int n = 0, m = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] != 0)
            {
                ++m;
                if (map[i][j]->closed && ++n)
                    maze[i][j] = maze[i][j] == 0 ? 6 : maze[i][j];
                else
                    maze[i][j] = maze[i][j] == 0 ? 5 : maze[i][j];
                free(map[i][j]);
            }
        }
    }
    printf("δ��չ���:%d  �ҵ��Ľڵ�/�ܽ��:%d/%d", m - n, m, (row - 2) * (col - 2));
    if (flg)
        return 1;
    return 0;
}

void add(P *head, P *n)
{
    P *p = head, *q = p;
    if (head->next == NULL)
    {
        head->next = n;
        return;
    }
    else
        q = q->next;
    char flg = 1;
    while (q != NULL && q->next != NULL)
    {
        if (q->weight < n->weight ||
            (q->weight == n->weight &&
             (q->value < n->value ||
              (q->value == n->value &&
               q->slope < n->slope))))
            p = p->next, q = q->next;
        else
        {
            n->next = q, p->next = n;
            flg = 0;
            break;
        }
    }
    if (flg)
        q->next = n;
}

int refresh(P *head, P *n)
{
    P *p = head, *q = head->next, *now = NULL;
    while (q != NULL && q->next != NULL)
    {
        if ((q->weight == n->weight &&
             (q->value > n->value ||
              (q->value == n->value &&
               q->slope > n->slope))) ||
            q->weight > n->weight)
            now = p;
        p = p->next;
        if (q->next != n)
            q = q->next;
        else
            break;
    }
    if (q->next && now != NULL)
    {
        head = now->next;
        now->next = n;
        q->next = n->next;
        n->next = head;
    }
    return 1;
}

P *get(P *head)
{
    P *p = head->next;
    if (head->next != NULL)
    {
        head->next = p->next;
        p->next = NULL;
        return p;
    }
    return NULL;
}

P *new_p(int x, int y, int deep)
{
    int n;
    P *p = (P *)malloc(sizeof(P));
    p->deep = deep, p->closed = 0;
    p->x = x, p->y = y;
    n = row - 2 - x;
    p->slope = abs((col - 1 / row - 1) - abs(n == 0 ? INT_MAX : (col - 2 - y) / n));
    p->value = get_value(p->x, p->y);
    p->weight = p->deep + p->value;
    p->next = p->parent = NULL;
    return p;
}

int get_value(int x, int y)
{
    return abs(row - 2 - x) + abs(col - 2 - y);
}
