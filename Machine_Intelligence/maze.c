#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define __ <
#define _ >
#define MAX_SIZE 200
typedef struct path
{
    unsigned int x : 8, y : 8, slope : 8; //����ֵx��y���Լ�Ŀ��ƫ��̶�slope
    unsigned int deep : 9, value : 9;     //��ȣ���ֵ
    unsigned int weight : 9;              //Ȩֵ
    unsigned int closed : 1;              //�ڵ�رձ�־
    struct path *next, *parent;           //��һ���ڵ㣬���ڵ�
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
int refresh(P *head, P *n);                //���½ڵ���open_list�е�λ��
int main_1(void);                          //ԭʼ������
int main_2(void);                          //ħ�ĺ����ڲ����������
void show_other();                         //��ʾ������Ϣ

int startI = 1,
    startJ = 1; // ���
int count;
int success = 0;

int maze[MAX_SIZE][MAX_SIZE];                         //�Թ�����
unsigned short maze_weight[MAX_SIZE][MAX_SIZE] = {0}; //�Թ�Ȩֵ
unsigned short maze_value[MAX_SIZE][MAX_SIZE] = {0};  //�Թ���ֵ
unsigned short maze_deep[MAX_SIZE][MAX_SIZE] = {0};   //�Թ����
unsigned short maze_slope[MAX_SIZE][MAX_SIZE] = {0};  //�Թ�Ŀ��ƫ���
int row = 0;
int col = 0;
//�Թ�����2����ǽ�ڣ�0����ͨ��

int main(void)
{
    main_1();
    printf("%d", count);
}

int main_1(void)
{
    int i, j;
    char flg = 0;
    while (1)
    {
        memset(maze_weight, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_deep, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_value, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_slope, 0x7fff, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        printf("�������Թ�����row(0<row<%d)��", MAX_SIZE);
        scanf("%d", &row);
        printf("�������Թ�����col(0<col<%d)��", MAX_SIZE);
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
        printf("�Ƿ���ʾ������Ϣ:(y/n)\n");
        while (1)
        {
            scanf("\n%c", &flg);
            if (flg == 'y' || flg == 'n')
                break;
            else
                printf("�������,������!\n");
        }
        if (flg == 'y')
            show_other();
        printf("�Ƿ��˳�:(y/n)\n");
        while (1)
        {
            scanf("\n%c", &flg);
            if (flg == 'y' || flg == 'n')
                break;
            else
                printf("�������,������!\n");
        }
        if (flg == 'y')
            break;
    }

    system("pause");

    return 0;
}

int main_2(void) //�������ɹ̶���С������Թ�����⣬ͬʱչʾ��չ�Ľڵ��Լ��������ͳ�ơ�
{
    int i, j;
    row = 30;
    col = 30;
    char flg = 0;

    while (1)
    {
        memset(maze_weight, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_deep, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_value, 0, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        memset(maze_slope, 0x7fff, MAX_SIZE * MAX_SIZE * sizeof(unsigned short));
        createWall(); //�����Թ���ǽ
        createMaze(); //�����Թ�
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
        printf("�Ƿ���ʾ������Ϣ:(y/n)\n");
        while (1)
        {
            scanf("\n%c", &flg);
            if (flg == 'y' || flg == 'n')
                break;
            else
                printf("�������,������!\n");
        }
        if (flg == 'y')
            show_other();
        printf("�Ƿ��˳�:(y/n)\n");
        while (1)
        {
            scanf("\n%c", &flg);
            if (flg == 'y' || flg == 'n')
                break;
            else
                printf("�������,������!\n");
        }
        if (flg == 'y')
            break;
    }
    system("pause");

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

int visit_aimless_dfs(int row2, int col2) //äĿ�������������
{
    if (maze[row2][col2] != 0)
        return 0;
    maze[row2][col2] = 1;
    if (row2 == row - 2 && col2 == row - 2)
        return 1;
    if (visit_aimless_dfs(row2, col2 + 1) || visit_aimless_dfs(row2 + 1, col2) ||
        visit_aimless_dfs(row2 - 1, col2) || visit_aimless_dfs(row2, col2 - 1)) //��һ���ɽ⼴Ϊ�ɽ⡣
        return 1;
    maze[row2][col2] = 0;
    return 0;
}

int visit_A_star(int row2, int col2) //ͨ��A*�㷨Ѱ���Թ���·
{
    P *map[row][col];                        //�洢�ڵ���Ϣ������
    memset(map, 0, row * col * sizeof(P *)); //���������ݳ�ʼ��Ϊ0
    int x, y, flg = 0;
    int deep; //��ǰ�������Ľڵ�������Ϣ
    P *open_list = (P *)malloc(sizeof(P)), *p, *q;
    map[row2][col2] = p = open_list->next = (P *)malloc(sizeof(P));
    p->x = row2, p->y = col2, p->deep = 0, p->next = p->parent = NULL;
    p->slope = abs(p->y - startJ - ((((col - startJ - 1) * 1000) / ((row - startI - 1))) * (p->x - startI) / 1000));
    p->value = get_value(p->x, p->y), p->weight = p->deep + p->value; //��ʼ��open_list
    while (open_list->next != NULL)                                   //ͨ���ж�open_list�Ƿ�Ϊ��������ѭ��
    {
        count++;
        p = get(open_list);                                //��open_list���Ƴ�Ȩֵ��С�Ľڵ�
        p->closed = 1;                                     //���˽ڵ���Ϊ�ر�
        if (p->x == row - 2 && p->y == col - 2 && (++flg)) //����ҵ����ڼ���flg��1������ѭ��
            break;
        x = p->x, y = p->y, deep = p->deep; //�õ��ڵ������Ϣ
        /* 
        ������չ�׶�
        �������¡��ҡ��ϡ�������ڵ㲢����open_list���У�
        ͬʱ����Ӧ�Ľڵ��¼���洢�ڵ���Ϣ����Ķ�Ӧλ�õ��С�
         */
        if (maze[x + 1][y] != 2) //����˽ڵ㲻��ǽ�����ɽ�����Ӳ���
        {
            if (map[x + 1][y] == 0) //�����ǰ�ڵ����Ϣδ��¼�������У��ʹ����½ڵ㲢��¼
            {
                q = map[x + 1][y] = new_p(x + 1, y, deep + 1);
                add(open_list, q);     //���ڵ���뵽open_list����
                q->parent = map[x][y]; //���˽ڵ�ĸ��ڵ�����Ϊ��ǰ�ڵ�
            }
            else //����ڵ��Ѵ���
            {
                q = map[x + 1][y];      //ȡ�ýڵ���Ϣ
                if (q->deep > deep + 1) //�����ǰ·��������·���̣����нڵ����
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;                      //���½ڵ����
                    q->weight = q->deep + q->value;          //���½ڵ�Ȩֵ
                    q->closed == 0 && refresh(open_list, q); //����ڵ�δ�رգ��ͽ��и��´˽ڵ���open_list�е�λ��
                    q->parent = map[x][y];                   //���˽ڵ�ĸ��ڵ�����Ϊ��ǰ�ڵ�
                }
            }
        }
        if (maze[x][y + 1] != 2) //�߼���������ͬ
        {
            if (map[x][y + 1] == 0)
            {
                q = map[x][y + 1] = new_p(x, y + 1, deep + 1);
                add(open_list, q);
                q->parent = map[x][y];
            }
            else
            {
                q = map[x][y + 1];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[x][y];
                }
            }
        }
        if (maze[x - 1][y] != 2) //�߼���������ͬ
        {
            if (map[x - 1][y] == 0)
            {
                q = map[x - 1][y] = new_p(x - 1, y, deep + 1);
                add(open_list, q);
                q->parent = map[x][y];
            }
            else
            {
                q = map[x - 1][y];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[x][y];
                }
            }
        }
        if (maze[x][y - 1] != 2) //�߼���������ͬ
        {
            if (map[x][y - 1] == 0)
            {
                q = map[x][y - 1] = new_p(x, y - 1, deep + 1);
                add(open_list, q);
                q->parent = map[x][y];
            }
            else
            {
                q = map[x][y - 1];
                if (q->deep > deep + 1)
                {
                    //printf("start  %d   ", q->deep);
                    //printf("end  %d\n", deep + 1);
                    q->deep = deep + 1;
                    q->weight = q->deep + q->value;
                    q->closed == 0 && refresh(open_list, q);
                    q->parent = map[x][y];
                }
            }
        }
    }
    if (flg) //���flgΪ1��˵���ҵ�����
    {
        for (q = p; q != NULL; q = q->parent) //���Թ�ͼ�м�¼�ҵ���·��
            maze[q->x][q->y] = 1;
    }
    int n = 0, m = 0;             //ͳ�������ñ���
    for (int i = 0; i < row; i++) //�������ڵ���Ϣ�洢������б���
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] != 0) //����洢��Ϊ0��˵�����нڵ���Ϣ
            {
                maze_deep[i][j] = map[i][j]->deep;
                maze_value[i][j] = map[i][j]->value;
                maze_weight[i][j] = map[i][j]->weight;             //�洢��Ϣ
                maze_slope[i][j] = map[i][j]->slope;               //�洢��Ϣ
                ++m;                                               //���ɽڵ�����1
                if (map[i][j]->closed && ++n)                      //����ڵ��ѹرգ���չ�ڵ�����1
                    maze[i][j] = maze[i][j] == 0 ? 6 : maze[i][j]; //����Թ��˴�û��������¼�����Ϊ����չ
                else
                    maze[i][j] = maze[i][j] == 0 ? 5 : maze[i][j]; //���û����չ�����Ϊδ��չ
                free(map[i][j]);                                   //�ͷŴ˽ڵ��ڴ�
            }
        }
    }
    printf("δ��չ���:%d  �ҵ��Ľڵ�/�ܽ��:%d/%d", m - n, m, (row - 2) * (col - 2)); //��������Ϣ
    if (flg)
        return 1; //�ɹ��˳�
    return 0;     //ʧ���˳�
}

void add(P *head, P *n) //��open_list�м���ڵ�
{
    P *p = head, *q = p;
    char flg = 1;           //����ʧ�ܱ�־
    if (head->next == NULL) //ͷ�ڵ�֮��Ϊ�յ�����ж�
    {
        head->next = n; //֮��Ϊ����ֱ�����ӷ���
        return;
    }
    else
        q = q->next;  //����ǰָ������ƶ�
    while (q != NULL) //������ĩβ���˳�
    {
        if (q->weight < n->weight ||   //������ж�������ȨֵС�ķ���ǰ��
            (q->weight == n->weight && //���Ȩֵ���
             (q->value __ n->value ||  //��ֵС�ķ���ǰ��
              (q->value == n->value && //�����ֵ���
               q->slope < n->slope)))) //��Ŀ��ƫ���С�ķ���ǰ��
            p = p->next, q = q->next;
        else //������������в���
        {
            n->next = q, p->next = n;
            flg = 0; //����ɹ�flg��0
            break;   //�����������ѭ��
        }
    }
    if (flg) //�������ʧ�ܣ�ֱ�����ӵ�ĩβ
        p->next = n;
}

int refresh(P *head, P *n) //�Ե�һ�ڵ���������
{
    P *p = head, *q = head->next, *now = NULL;
    while (q != NULL && q->next != NULL) //������ĩβ���˳�
    {
        if ((q->weight == n->weight && //��������Ĺ�����add()��ͬ
             (q->value _ n->value ||
              (q->value == n->value &&
               q->slope > n->slope))) ||
            q->weight > n->weight)
            now = p; //�ҵ�Ŀ��λ�ò���¼
        p = p->next;
        if (q->next != n) //�ҵ��ڵ�����λ��
            q = q->next;
        else
            break;
    }
    if (q->next && now != NULL) //������������
    {
        head = now->next;
        now->next = n;
        q->next = n->next;
        n->next = head;
    }
    return 1;
}

P *get(P *head) //�õ�open_list�е���С�ڵ㲢ɾ��
{
    P *p = head->next;
    if (head->next != NULL) //�������ź�˳�򣬹�ȡͷ�ڵ�֮���һ���ڵ㼴��
    {
        head->next = p->next;
        p->next = NULL;
        return p;
    }
    return NULL;
}

P *new_p(int x, int y, int deep) //�����½ڵ�
{
    int n;
    P *p = (P *)malloc(sizeof(P)); //�����ڴ�ռ�
    p->deep = deep, p->closed = 0;
    p->x = x, p->y = y;
    n = ((col - startJ - 1) * 1000) / ((row - startI - 1));
    p->slope = abs(y - startJ - (n * (x - startI) / 1000));
    p->value = get_value(p->x, p->y);
    p->weight = p->deep + p->value;
    p->next = p->parent = NULL; //������س�ʼ��
    return p;
}

int get_value(int x, int y) //��ô���λ�õ�·����ֵ
{
    return abs(row - 2 - x) + abs(col - 2 - y);
}

void show_other()
{
    printf("�Թ�Ȩֵ:\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (maze[i][j] == 2)
                printf("####");
            else if (maze_weight[i][j] == 0)
                printf("    ");
            else
                printf("%3d ", maze_weight[i][j]);
        }
        printf("\n");
    }
    printf("�Թ�deep:\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (maze[i][j] == 2)
                printf("####");
            else if (maze_weight[i][j] == 0)
                printf("    ");
            else
                printf("%3d ", maze_deep[i][j]);
        }
        printf("\n");
    }
    printf("�Թ���ֵ:\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (maze[i][j] == 2)
                printf("####");
            else if (maze_weight[i][j] == 0)
                printf("    ");
            else
                printf("%3d ", maze_value[i][j]);
        }
        printf("\n");
    }
    printf("�Թ�slope:\n");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (maze[i][j] == 2)
                printf("####");
            else if (maze_slope[i][j] >= 5000)
                printf("    ");
            else
                printf("%3d ", maze_slope[i][j]);
        }
        printf("\n");
    }
}
