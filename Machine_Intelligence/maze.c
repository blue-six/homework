#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1

typedef struct path
{
    unsigned int x : 8, y : 8, slope : 8; //坐标值x，y，以及目标偏离程度slope
    unsigned int deep : 9, value : 9;     //深度，估值
    unsigned int weight : 9;              //权值
    unsigned int closed : 1;              //节点关闭标志
    struct path *next, *parent[3];        //下一个节点，父节点
} P;

void createMaze();                         //创建随机迷宫
void createFreeMaze();                     //创建自定义迷宫
void createWall();                         //创建迷宫外墙
int visit_aimless_dfs(int row2, int col2); //盲目搜索
int visit_aimless_bfs(int row2, int col2); //盲目搜索
int visit_A_star(int row2, int col2);      //A*搜索
int get_value(int x, int y);               //取得h值
P *get(P *head);                           //取得权值最小的节点
void add(P *head, P *n);                   //添加节点
P *new_p(int x, int y, int deep);          //创建新节点
int refresh(P *head, P *n);                //更新节点在open_list中的位置
int main_1(void);                          //原始主函数
int main_2(void);                          //魔改后用于测验的主函数
void extend(int x, int y, int deep, int row3, int cow3,
            P *map[row3][cow3], P *open_list, P *p);               //节点扩展
void find(P *head);                                                //迷宫赋值
int get_good_deep(int x, int y);                                   //弃用
void _refresh_deep(P *p, P *q);                                    //弃用
int refresh_deep(P *head, int row3, int col3, P *map[row3][col3]); //弃用

int startI = 1,
    startJ = 1; // 入口
int success = 0;
//迷宫数组
int maze[100][100];
int row = 40;
int col = 40;
//迷宫矩阵，2代表墙壁，0代表通道

int main(void)
{
    DEBUG ? main_2() : main_1();
}

int main_1(void)
{
    int i, j;

    printf("请输入迷宫行数row(0<row<100)：");
    scanf("%d", &row);
    printf("请输入迷宫列数col(0<col<100)：");
    scanf("%d", &col);

    createWall(); //创建迷宫外墙

    int choice;
    printf("请选择创建随机迷宫还是自定义迷宫（1为随机迷宫，2为自定义迷宫）:");
    scanf("%d", &choice);
    if (choice == 1)
    {
        createMaze(); //创建迷宫
    }
    else if (choice == 2)
    {
        printf("\n请输入自定义迷宫的墙壁和通道，2代表墙壁，0代表通道\n");
        createFreeMaze();
    }

    printf("\n显示迷宫：\n");
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
        printf("\n没有找到出口！\n");
    }
    else
    {
        printf("\n显示路径：\n");
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (maze[i][j] == 2)
                    printf("##");
                else if (maze[i][j] == 1)
                    printf("··");
                else
                    printf("  ");
            }
            printf("\n");
        }
    }

    system("pause");

    return 0;
}

int main_2(void) //连续生成固定大小的随机迷宫并求解，同时展示扩展的节点以及相关数据统计。
{
    int i, j;

    while (1)
    {
        createWall(); //创建迷宫外墙
        createMaze(); //创建迷宫
        if (visit_A_star(startI, startJ) == 0)
        {
            printf("\n没有找到出口！\n");
            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (maze[i][j] == 2)
                        printf("##");
                    else if (maze[i][j] == 1)
                        printf("··");
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
            printf("\n显示路径：\n");
            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (maze[i][j] == 2)
                        printf("##");
                    else if (maze[i][j] == 1)
                        printf("··");
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

//创建迷宫外墙
void createWall()
{
    //创建迷宫外墙,第一行、第一列、最后一行、最后一列均为墙壁
    for (int i = 0; i < col; i++) //第一行
        maze[0][i] = 2;
    for (int i = 1; i < row; i++) //第一列
        maze[i][0] = 2;
    for (int i = 1; i < col; i++) //最后一行
        maze[row - 1][i] = 2;
    for (int i = 1; i < row - 1; i++) //最后一列
        maze[i][col - 1] = 2;
}

//创建随机迷宫
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

//创建自定义迷宫
void createFreeMaze()
{
    for (int i = 1; i < row - 1; i++)
    {
        //第一行，第一格为入口
        if (i == 1)
        {
            printf("迷宫第%d行，共%d格：  ", i, col - 3);
            for (int j = 2; j < col - 1; j++)
                scanf("%d", &maze[i][j]);
        }
        //最后一行,最后一格为出口
        else if (i == row - 2)
        {
            printf("迷宫第%d行，共%d格：", i, col - 3);
            for (int j = 1; j < col - 2; j++)
                scanf("%d", &maze[i][j]);
        }
        else
        {
            printf("迷宫第%d行，共%d格：", i, col - 2);
            for (int j = 1; j < col - 1; j++)
                scanf("%d", &maze[i][j]);
        }
    }
    maze[1][1] = 0;             //入口为通道
    maze[row - 2][col - 2] = 0; //出口为通道
}

int visit_aimless_dfs(int row2, int col2) //盲目深度优先搜索。
{
    if (maze[row2][col2] != 0)
        return 0;
    maze[row2][col2] = 1;
    if (row2 == row - 2 && col2 == row - 2)
        return 1;
    if (visit_aimless_dfs(row2, col2 + 1) || visit_aimless_dfs(row2 + 1, col2) ||
        visit_aimless_dfs(row2 - 1, col2) || visit_aimless_dfs(row2, col2 - 1)) //有一个可解即为可解。
        return 1;
    maze[row2][col2] = 0;
    return 0;
}

int visit_A_star(int row2, int col2) //通过A*算法寻找迷宫出路
{
    P *map[row][col]; //存储节点信息的数组
    char x, y, flg = 0;
    int deep, weight = INT_MAX;              //当前遍历到的节点的相关信息
    memset(map, 0, row * col * sizeof(P *)); //将数组内容初始化为0
    P *open_list = (P *)malloc(sizeof(P)), *p, *q;
    map[row2][col2] = p = open_list->next = (P *)malloc(sizeof(P));
    p->x = row2, p->y = col2, p->deep = 0, p->next = NULL, memset(p->parent, 0, 3 * sizeof(P *));
    p->slope = (p->y / p->x) - abs(col - col2) / abs(row - row2);
    p->value = get_value(p->x, p->y), p->weight = p->deep + p->value; //初始化open_list
    while (open_list->next != NULL)                                   //通过判断open_list是否为空来跳出循环
    {
        p = get(open_list);                 //从open_list中移出权值最小的节点
        x = p->x, y = p->y, deep = p->deep; //得到节点相关信息
        // refresh_deep(p, row, col, map);
        if (p->weight > weight) //(p->value + get_good_deep(x, y))
            break;
        p->closed = 1;                                      //将此节点标记为关闭
        if ((x == row - 2) && (y == col - 2) && flg++ == 0) //如果找到出口即将flg置1并跳出循环
            weight = p->weight, q = p,
            DEBUG && printf("weight=%d\n", weight);
        /* 
        进入扩展阶段
        依次向下、右、上、左遍历节点并加入open_list当中，
        同时将对应的节点记录到存储节点信息数组的对应位置当中。
         */
        extend(x + 1, y, deep, row, col, map, open_list, p);
        extend(x, y + 1, deep, row, col, map, open_list, p);
        extend(x - 1, y, deep, row, col, map, open_list, p);
        extend(x, y - 1, deep, row, col, map, open_list, p);
    }
    if (flg) //如果flg为1，说明找到出口
    {
        for (p = q; q != NULL; q = q->parent[0]) //在迷宫图中记录找到的路径
            maze[q->x][q->y] = 1;
        find(p);
    }
    int n = 0, m = 0;             //统计数据用变量
    for (int i = 0; i < row; i++) //对整个节点信息存储数组进行遍历
    {
        for (int j = 0; j < col; j++)
        {
            if (map[i][j] != 0) //如果存储不为0，说明存有节点信息
            {
                ++m;                                               //生成节点数加1
                if (map[i][j]->closed && ++n)                      //如果节点已关闭，扩展节点数加1
                    maze[i][j] = maze[i][j] == 0 ? 6 : maze[i][j]; //如果迷宫此处没有其他记录，标记为已扩展
                else
                    maze[i][j] = maze[i][j] == 0 ? 5 : maze[i][j]; //如果没有扩展，标记为未扩展
                free(map[i][j]);                                   //释放此节点内存
            }
        }
    }
    printf("未扩展结点:%d  找到的节点/总结点:%d/%d", m - n, m, (row - 2) * (col - 2)); //输出相关信息
    if (flg)
        return 1; //成功退出
    return 0;     //失败退出
}

void add(P *head, P *n) //向open_list中加如节点
{
    P *p = head, *q = p;
    if (head->next == NULL) //头节点之后为空的相关判断
    {
        head->next = n; //之后为空则直接链接返回
        return;
    }
    else
        q = q->next;                     //否则当前指针向后移动
    char flg = 1;                        //加入失败标志
    while (q != NULL && q->next != NULL) //变历到末尾就退出
    {
        if (q->weight < n->weight ||   //排序的判断条件，权值小的放在前方
            (q->weight == n->weight && //如果权值相等
             (q->value < n->value ||   //估值小的放在前面
              (q->value == n->value && //如果估值相等
               q->slope < n->slope)))) //对目标偏离较小的放在前面
            p = p->next, q = q->next;
        else //满足条件后进行插入
        {
            n->next = q, p->next = n;
            flg = 0; //加入成功flg置0
            break;   //插入完成跳出循环
        }
    }
    if (flg) //如果加入失败，直接链接到末尾
        q->next = n;
}

int refresh(P *head, P *n) //对单一节点重新排序
{
    P *p = head, *q = head->next, *now = NULL;
    while (q != NULL && q->next != NULL) //遍历到末尾就退出
    {
        if ((q->weight == n->weight && //重新排序的规则，与add()相同
             (q->value > n->value ||
              (q->value == n->value &&
               q->slope > n->slope))) ||
            q->weight > n->weight)
            now = p; //找到目标位置并记录
        p = p->next;
        if (q->next != n) //找到节点所在位置
            q = q->next;
        else
            break;
    }
    if (q->next && now != NULL) //进行重排链接
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

P *get(P *head) //得到open_list中的最小节点并删除
{
    P *p = head->next;
    if (head->next != NULL) //由于已排好顺序，故取头节点之后的一个节点即可
    {
        head->next = p->next;
        p->next = NULL;
        return p;
    }
    return NULL;
}

P *new_p(int x, int y, int deep) //创建新节点
{
    int n;
    P *p = (P *)malloc(sizeof(P)); //申请内存空间
    p->deep = deep, p->closed = 0;
    p->x = x, p->y = y;
    n = row - 2 - x;
    p->slope = abs((col - 2 / row - 2) - abs(n == 0 ? INT_MAX : (col - 2 - y) / n));
    p->value = get_value(p->x, p->y);
    p->weight = p->deep + p->value, p->next = NULL;
    memset(p->parent, 0, 3 * sizeof(P *)); //进行相关初始化
    return p;
}

int get_value(int x, int y) //获得传入位置的路径估值
{
    return abs(row - 2 - x) + abs(col - 2 - y);
}

int get_good_deep(int x, int y) //获得可能的最优步数
{
    return abs(x - 1) + abs(y - 1);
}

void extend(int x, int y, int deep, int row3, int col3, P *map[row3][col3], P *open_list, P *p)
{
    P *q;
    if (maze[x][y] != 2) //如果此节点不是墙，即可进行添加操作
    {
        if (map[x][y] == 0) //如果当前节点的信息未记录到数组中，就创建新节点并记录
        {
            q = map[x][y] = new_p(x, y, deep + 1);
            q->parent[0] = p;  //将此节点的父节点设置为当前节点
            add(open_list, q); //将节点加入到open_list当中
        }
        else //如果节点已存在
        {
            q = map[x][y];          //取得节点信息
            if (q->deep > deep + 1) //如果当前路径比已有路径短，进行节点更新
            {
                DEBUG &&printf("start  %3d   ", q->deep);
                DEBUG &&printf("end  %3d   close  %3d\n", deep + 1, q->closed);
                q->deep = deep + 1;             //更新节点深度
                q->weight = q->deep + q->value; //更新节点权值
                q->parent[0] = p;               //将此节点的父节点设置为当前节点
                q->parent[1] = q->parent[2] = 0;
                q->closed == 0 && refresh(open_list, q); //如果节点未关闭，就进行更新此节点在open_list中的位置
            }
            else if (q->deep == deep + 1 && q->parent[0] != p) //如果当前节点可以是此节点的父节点，则记录
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

void find(P *head) //递归记录其他路径
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
