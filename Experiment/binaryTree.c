#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode //���ڵ�
{
    char sign;
    struct treeNode *left, *right;
} T;

T *createTree();              //���������з�����
int out(T *head, int *leafs); //����������ͷ��ڴ棨���Ҹ���LRD����������ȼ�Ҷ����Ŀ����

int main()
{
    int *leafs, deep;
    leafs = (int *)malloc(sizeof(int));
    *leafs = 0;
    T *head = createTree();
    deep = out(head, leafs);
    printf("\n���Ϊ��%d\nҶ�ӽڵ���ĿΪ��%d\n", deep, (*leafs) / 2);
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
    printf("����ڵ㣬���ָ����ԡ�#����س�����:\n");
    scanf("%c", &c);
    head->sign = c; //��ȫͷ�ڵ�
    scanf("%c", &c);
    while (c != '#' && c != '\n')
    {
        q = queue[point];
        if (q->left == NULL) //���ȴ�����ڵ�
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

int out(T *head, int *leafs) //�ݹ鷽��
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
