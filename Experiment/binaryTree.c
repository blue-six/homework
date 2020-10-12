#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode //树节点
{
    char sign;
    struct treeNode *left, *right;
} T;

T *createTree();              //建树（队列方法）
int out(T *head, int *leafs); //遍历输出并释放内存（左右根）LRD，返回树深度及叶子数目二倍

int main()
{
    int *leafs, deep;
    leafs = (int *)malloc(sizeof(int));
    *leafs = 0;
    T *head = createTree();
    deep = out(head, leafs);
    printf("\n深度为：%d\n叶子节点数目为：%d\n", deep, (*leafs) / 2);
    free(leafs);
    return 1;
}

T *createTree()
{
    char c;
    int index = 1, point = 0;
    T *head = (T *)malloc(sizeof(T)), *q, *queue[1024];
    queue[0] = head;
    head->left = head->right = NULL;
    printf("输入节点，不分隔，以‘#’或回车结束:\n");
    scanf("%c", &c);
    head->sign = c; //补全头节点
    scanf("%c", &c);
    while (c != '#' && c != '\n')
    {
        q = queue[point];
        if (q->left == NULL) //优先创建左节点
        {
            q->left = (T *)malloc(sizeof(T));
            q = q->left;
        }
        else if (q->right == NULL)
        {
            q->right = (T *)malloc(sizeof(T));
            q = q->right;
            ++point;
            point = point % 1024;
        }
        q->sign = c;
        q->left = q->right = NULL;
        queue[index++] = q;
        index %= 1024;
        scanf("%c", &c);
    }
    return head;
}

int out(T *head, int *leafs) //递归方法
{
    if (head == NULL)
    {
        (*leafs)++;
        return 0;
    }
    int a = out(head->left, leafs);
    int b = out(head->right, leafs);
    printf("%c", head->sign);
    free(head);
    return (a > b ? a : b) + 1;
}
