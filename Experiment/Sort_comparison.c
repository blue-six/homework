/* 
思路：输入数组长度，产生随机数，排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1
#define RANGE 1000
#define BOTTOM -500

typedef struct count
{
    unsigned int num_compare, num_move;
    char name[18];
} C;

void out(int *array, int len);                //输出数组
int input();                                  //获取输入
int *copy(int *array, const int len);         //数组拷贝
int count(int *array, const int len);         //排序相关数据统计
int *generate(const int len);                 //生成指定长度的随机数数组
C *bubble_sort(int *array, const int len);    //冒泡排序
C *insert_sort(int *array, const int len);    //插入排序
C *selection_sort(int *array, const int len); //选择排序
C *quick_sort(int *array, const int len);     //快速排序
C *shell_sort(int *array, const int len);     //希尔排序
C *heap_sort(int *array, const int len);      //堆排序

int main(void)
{
    const int len = input();
    int *array = generate(len);
    count(array, len);
    return 1;
}

int input()
{
    int len;
    printf("请输入数组长度:\n");
    while (1)
    {
        scanf("%d", &len);
        if (len > 0)
            return len;
        else
            printf("请输入大于零的数!\n");
    }
}

int *generate(const int len)
{
    int *array = (int *)malloc(len * sizeof(int));
    srand((unsigned)time(NULL));
    printf("生成的数组为:");
    for (int i = 0; i < len; i++)
    {
        array[i] = (rand() % (RANGE + 1)) + BOTTOM;
        printf(" %3d", array[i]);
    }
    printf("\n");
    return array;
}

int *copy(int *array, const int len)
{
    int *p = (int *)malloc(len * sizeof(int));
    memcpy(p, array, len * sizeof(int));
    return p;
}

void out(int *array, int len)
{
    printf("排序好的数组为:");
    for (int i = 0; i < len; i++)
    {
        printf(" %3d", array[i]);
    }
    printf("\n");
}

int count(int *array, const int len)
{
    C *(*sort[])(int *, const int) = {&bubble_sort, &insert_sort, &selection_sort,
                                      &shell_sort, &quick_sort, &heap_sort};
    C *p;
    int *q;
    for (int i = 0; i < 4; i++)
    {
        q = copy(array, len);
        p = sort[i](q, len);
        out(q, len);
        free(q);
        printf("%s() --> 比较次数:%5d   ,  移动次数:%5d\n", p->name, p->num_compare, p->num_move);
        free(p);
    }
    return 1;
}

C *bubble_sort(int *array, const int len)
{
    int i, j, temp, flg = 1;
    C *p = (C *)malloc(sizeof(C));
    p->num_compare = p->num_move = 0, memset(p->name, '\0', 18 * sizeof(char));
    strcpy(p->name, __func__);
    for (i = 0; i < len - 1 && flg; i++)
    {
        flg = 0;
        for (j = 0; j < len - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                flg == 0 && flg++;
                p->num_compare++;
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                p->num_move += 3;
            }
        }
    }
    return p;
}

C *insert_sort(int *array, const int len)
{
    int i, j, temp;
    C *p = (C *)malloc(sizeof(C));
    p->num_compare = p->num_move = 0, memset(p->name, '\0', 18 * sizeof(char));
    strcpy(p->name, __func__);
    for (i = 1; i < len; i++)
    {
        temp = array[i], p->num_move++;
        for (j = i; ++(p->num_compare) && j > 0 &&
                    ++(p->num_compare) && array[j - 1] > temp;
             j--)
            array[j] = array[j - 1], p->num_move++;
        array[j] = temp, p->num_move++;
    }
    return p;
}

C *selection_sort(int *array, const int len)
{
    int i, j, min;
    C *p = (C *)malloc(sizeof(C));
    p->num_compare = p->num_move = 0, memset(p->name, '\0', 18 * sizeof(char));
    strcpy(p->name, __func__);
    for (i = 0; i < len; i++)
    {
        min = i, p->num_move++;
        for (j = i + 1; j < len; j++)
        {
            p->num_compare++;
            if (array[min] > array[j])
                min = j, p->num_move++;
        }
        j = array[i], array[i] = array[min], array[min] = j, p->num_move += 3;
    }
    return p;
}

C *quick_sort(int *array, const int len)
{
    return NULL;
}

C *shell_sort(int *array, const int len)
{
    int gap, i, j;
    int temp;
    C *p = (C *)malloc(sizeof(C));
    p->num_compare = p->num_move = 0, memset(p->name, '\0', 18 * sizeof(char));
    strcpy(p->name, __func__);
    for (gap = len >> 1; gap > 0; gap = gap >> 1)
        for (i = gap; i < len; i++)
        {
            temp = array[i], p->num_move++;
            for (j = i - gap; ++(p->num_compare) && j >= 0 && ++(p->num_compare) && array[j] > temp; j -= gap)
                array[j + gap] = array[j], p->num_move++;
            array[j + gap] = temp, p->num_move++;
        }
    return p;
}

C *heap_sort(int *array, const int len)
{
    return NULL;
}
