#include <stdio.h>
#include <stdlib.h>
#define LIST_SIZE 1024

typedef struct list
{
    int data[LIST_SIZE];
    int last;
} list;

list a = {{0}, 0}, b = {{0}, 0}, result = {{0}, 0}, *_list[] = {&a, &b};

void make(int *arr, int size, char name); //填入系数
void add();                               //相加
void out();                               //输出
void in_make(char name);                  //获取输入

int main()
{
    // int a_[] = {0}; //多项式a的（幂次，系数，幂次，系数···）
    // int b_[] = {0}; //同上
    // make(a_, sizeof(a_) / sizeof(a_[0]), 'a');
    // make(b_, sizeof(b_) / sizeof(b_[0]), 'b');
    in_make('a');
    in_make('b');
    add();
    out();
    // system("pause");
    return 0;
}

void make(int *arr, int size, char name)
{
    int n = name - 'a';
    for (int i = 0; i < size; i += 2)
    {
        _list[n]->data[arr[i]] = arr[i + 1];
        if (arr[i] > _list[n]->last)
        {
            _list[n]->last = arr[i];
        }
    }
}

void add()
{
    int n = a.last > b.last ? a.last : b.last;
    for (int i = 0; i <= n; i++)
    {
        result.data[i] = a.data[i] + b.data[i];
    }
    result.last = n;
}

void out()
{
    for (int i = 0; i < result.last; i++)
    {
        if (result.data[i])
        {
            printf("%d*X^%d + ", result.data[i], i);
        }
    }
    if (result.last > 0)
    {
        printf("%d*X^%d\n", result.data[result.last], result.last);
    }
    else
    {
        printf("none\n");
    }
}

void in_make(char name)
{
    list *p = _list[name - 'a'];
    int n, x;
    printf("请输入多项式，幂次输入-1结束\n格式：系数 幂次\n");
    scanf("%d %d", &x, &n);
    while (n != -1)
    {
        p->data[n] = x;
        p->last = p->last > n ? p->last : n;
        scanf("%d %d", &x, &n);
    }
}
