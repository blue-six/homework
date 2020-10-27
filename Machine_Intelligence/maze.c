#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1

typedef struct path
{
    unsigned int x : 8, y : 8, slope : 8; //����ֵx��y���Լ�Ŀ��ƫ��̶�slope
    unsigned int deep : 9, value : 9;     //��ȣ���ֵ
    unsigned int weight : 9;              //Ȩֵ
    unsigned int closed : 1;              //�ڵ�رձ�־
    struct path *next, *parent[3];        //��һ���ڵ㣬���ڵ�
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
void extend(int x, int y, int deep, int row3, int cow3,
            P *map[row3][cow3], P *open_list, P *p);               //�ڵ���չ
void find(P *head);                                                //�Թ���ֵ
int get_good_deep(int x, int y);                                   //����
void _refresh_deep(P *p, P *q);                                    //����
int refresh_deep(P *head, int row3, int col3, P *map[row3][col3]); //����

int startI = 1,
    startJ = 1; // ���
int success = 0;
//�Թ�����
int maze[100][100];
int row = 40;
int col = 40;
//�Թ�����2����ǽ�ڣ�0����ͨ��

int main(void)
{
    DEBUG ? main_2() : main_1();
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

int main_2(void) //�������ɹ̶���С������Թ�����⣬ͬʱչʾ��չ�Ľڵ��Լ��������ͳ�ơ�
{
    int i, j;

    while (1)
    {
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
                    else if (maze[i][j] == 10)
                        printf("--");
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
    P *map[row][col]; //�洢�ڵ���Ϣ������
    char x, y, flg = 0;
    int deep, weight = INT_MAX;              //��ǰ�������Ľڵ�������Ϣ
    memset(map, 0, row * col * sizeof(P *)); //���������ݳ�ʼ��Ϊ0
    P *open_list = (P *)malloc(sizeof(P)), *p, *q;
    map[row2][col2] = p = open_list->next = (P *)malloc(sizeof(P));
    p->x = row2, p->y = col2, p->deep = 0, p->next = NULL, memset(p->parent, 0, 3 * sizeof(P *));
    p->slope = (p->y / p->x) - abs(col - col2) / abs(row - row2);
    p->value = get_value(p->x, p->y), p->weight = p->deep + p->value; //��ʼ��open_list
    while (open_list->next != NULL)                                   //ͨ���ж�open_list�Ƿ�Ϊ��������ѭ��
    {
        p = get(open_list);                 //��open_list���Ƴ�Ȩֵ��С�Ľڵ�
        x = p->x, y = p->y, deep = p->deep; //�õ��ڵ������Ϣ
        // refresh_deep(p, row, col, map);
        if (p->weight > weight) //(p->value + get_good_deep(x, y))
            break;
        p->closed = 1;                                      //���˽ڵ���Ϊ�ر�
        if ((x == row - 2) && (y == col - 2) && flg++ == 0) //����ҵ����ڼ���flg��1������ѭ��
            weight = p->weight, q = p,
            DEBUG && printf("weight=%d\n", weight);
        /* 
        ������չ�׶�
        �������¡��ҡ��ϡ�������ڵ㲢����open_list���У�
        ͬʱ����Ӧ�Ľڵ��¼���洢�ڵ���Ϣ����Ķ�Ӧλ�õ��С�
         */
        extend(x + 1, y, deep, row, col, map, open_list, p);
        extend(x, y + 1, deep, row, col, map, open_list, p);
        extend(x - 1, y, deep, row, col, map, open_list, p);
        extend(x, y - 1, deep, row, col, map, open_list, p);
    }
    if (flg) //���flgΪ1��˵���ҵ�����
    {
        for (p = q; q != NULL; q = q->parent[0]) //���Թ�ͼ�м�¼�ҵ���·��
            maze[q->x][q->y] = 1;
        find(p);
    }
    int n = 0, m = 0;             //ͳ�������ñ���
    for (int i = 0; i < row; i++) //�������ڵ���Ϣ�洢������б���
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] != 0) //����洢��Ϊ0��˵�����нڵ���Ϣ
            {
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
    if (head->next == NULL) //ͷ�ڵ�֮��Ϊ�յ�����ж�
    {
        head->next = n; //֮��Ϊ����ֱ�����ӷ���
        return;
    }
    else
        q = q->next;                     //����ǰָ������ƶ�
    char flg = 1;                        //����ʧ�ܱ�־
    while (q != NULL && q->next != NULL) //������ĩβ���˳�
    {
        if (q->weight < n->weight ||   //������ж�������ȨֵС�ķ���ǰ��
            (q->weight == n->weight && //���Ȩֵ���
             (q->value < n->value ||   //��ֵС�ķ���ǰ��
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
        q->next = n;
}

int refresh(P *head, P *n) //�Ե�һ�ڵ���������
{
    P *p = head, *q = head->next, *now = NULL;
    while (q != NULL && q->next != NULL) //������ĩβ���˳�
    {
        if ((q->weight == n->weight && //��������Ĺ�����add()��ͬ
             (q->value > n->value ||
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

int refresh_deep(P *head, int row3, int col3, P *map[row3][col3])
{
    int x = head->x, y = head->y;
    _refresh_deep(map[x + 1][y], head);
    _refresh_deep(map[x][y + 1], head);
    _refresh_deep(map[x - 1][y], head);
    _refresh_deep(map[x][y - 1], head);
    return 1;
}

void _refresh_deep(P *p, P *q)
{
    if (p != 0 && p->deep < q->deep)
    {
        if (q->parent[0] != p && q->parent[1] != p && q->parent[2] != p)
        {
            if (q->parent[1] == 0)
                q->parent[1] = p, q->deep = p->deep + 1, q->weight = q->value + q->deep;
            else if (q->parent[2] == 0)
                q->parent[2] = p, q->deep = p->deep + 1, q->weight = q->value + q->deep;
            else
            {
                printf("no space\nnow_x=%3d  now_y=%3d\n", q->x, q->y);
                printf("aim_x=%3d  aim_y=%3d\n", p->x, p->y);
                printf("0_x=%3d  0_y=%3d\n", q->parent[0]->x, q->parent[0]->y);
                printf("1_x=%3d  1_y=%3d\n", q->parent[1]->x, q->parent[1]->y);
                printf("2_x=%3d  2_y=%3d\n", q->parent[2]->x, q->parent[2]->y);
            }
        }
    }
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
    n = row - 2 - x;
    p->slope = abs((col - 2 / row - 2) - abs(n == 0 ? INT_MAX : (col - 2 - y) / n));
    p->value = get_value(p->x, p->y);
    p->weight = p->deep + p->value, p->next = NULL;
    memset(p->parent, 0, 3 * sizeof(P *)); //������س�ʼ��
    return p;
}

int get_value(int x, int y) //��ô���λ�õ�·����ֵ
{
    return abs(row - 2 - x) + abs(col - 2 - y);
}

int get_good_deep(int x, int y) //��ÿ��ܵ����Ų���
{
    return abs(x - 1) + abs(y - 1);
}

void extend(int x, int y, int deep, int row3, int col3, P *map[row3][col3], P *open_list, P *p)
{
    P *q;
    if (maze[x][y] != 2) //����˽ڵ㲻��ǽ�����ɽ�����Ӳ���
    {
        if (map[x][y] == 0) //�����ǰ�ڵ����Ϣδ��¼�������У��ʹ����½ڵ㲢��¼
        {
            q = map[x][y] = new_p(x, y, deep + 1);
            q->parent[0] = p;  //���˽ڵ�ĸ��ڵ�����Ϊ��ǰ�ڵ�
            add(open_list, q); //���ڵ���뵽open_list����
        }
        else //����ڵ��Ѵ���
        {
            q = map[x][y];          //ȡ�ýڵ���Ϣ
            if (q->deep > deep + 1) //�����ǰ·��������·���̣����нڵ����
            {
                DEBUG &&printf("start  %3d   ", q->deep);
                DEBUG &&printf("end  %3d   close  %3d\n", deep + 1, q->closed);
                q->deep = deep + 1;             //���½ڵ����
                q->weight = q->deep + q->value; //���½ڵ�Ȩֵ
                q->parent[0] = p;               //���˽ڵ�ĸ��ڵ�����Ϊ��ǰ�ڵ�
                q->parent[1] = q->parent[2] = 0;
                q->closed == 0 && refresh(open_list, q); //����ڵ�δ�رգ��ͽ��и��´˽ڵ���open_list�е�λ��
            }
            else if (q->deep == deep + 1 && q->parent[0] != p) //�����ǰ�ڵ�����Ǵ˽ڵ�ĸ��ڵ㣬���¼
            {
                if (q->parent[1] == 0 || p == q->parent[1])
                    q->parent[1] = p;
                else if (q->parent[2] == 0 || q->parent[2] == p)
                    q->parent[2] = p;
                else
                {
                    printf("no space\nnow_x=%3d  now_y=%3d\n", q->x, q->y);
                    printf("aim_x=%3d  aim_y=%3d\n", p->x, p->y);
                    printf("0_x=%3d  0_y=%3d\n", q->parent[0]->x, q->parent[0]->y);
                    printf("1_x=%3d  1_y=%3d\n", q->parent[1]->x, q->parent[1]->y);
                    printf("2_x=%3d  2_y=%3d\n", q->parent[2]->x, q->parent[2]->y);
                }
            }
        }
    }
}

void find(P *head) //�ݹ��¼����·��
{
    int n = head == 0 ? 0 : maze[head->x][head->y];
    if (head == 0 || (n != 0 && n != 1))
        return;
    if (n == 1)
        maze[head->x][head->y] = 15;
    else
        maze[head->x][head->y] = 10;
    find(head->parent[0]);
    find(head->parent[1]);
    find(head->parent[2]);
    if (n == 1)
        maze[head->x][head->y] = 1;
}
