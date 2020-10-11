#include <stdio.h>
#include <stdlib.h>

typedef struct tree //哈夫曼树节点
{
    int percent;
    char sign;
    struct tree *left;
    struct tree *right;
} T;

typedef struct stack //字符栈
{
    char *data;
    int last;
} stack;

void sort(int *p, char *q, int len); //排序
T *make(int *p, char *q, int len);   //构建哈夫曼树
void out(T *head, int len);          //打印输出
void _out(T *head, stack *p);
void freeNodes(T *root);

int main()
{
    char sign[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int prob[] = {5, 9, 7, 8, 14, 6, 3, 11};
    sort(prob, sign, 8);
    T *head = make(prob, sign, 8);
    out(head, 8);
    freeNodes(head);
    return 0;
}

void sort(int *p, char *q, int len)
{
    char flg = 1;
    for (int i = 0; i < len && flg; i++)
    {
        flg = 0;
        for (int j = i; j < len; j++)
        {
            if (p[i] < p[j])
            {
                int s;
                s = p[i];
                p[i] = p[j];
                p[j] = s;
                s = q[i];
                q[i] = q[j];
                q[j] = s;
                flg = 1;
            }
        }
    }
}

T *make(int *p, char *q, int len)
{
    T *head, *left, *right;
    T *list[len];
    for (int i = 0; i < len; i++)
    {
        list[i] = (T *)malloc(sizeof(T));
        list[i]->percent = p[i];
        list[i]->sign = q[i];
        list[i]->left = list[i]->right = NULL;
    }
    for (int i = len - 1; i > 0; i--)
    {
        head = (T *)malloc(sizeof(T));
        head->right = right = list[i];
        head->left = left = list[i - 1];
        head->sign = 0;
        head->percent = right->percent + left->percent;
        for (int j = i - 2; j >= 0; j--)
        {
            if (head->percent < list[j]->percent)
            {
                list[j + 1] = head;
                break;
            }
            else if (j == 0)
            {
                list[1] = list[0];
                list[0] = head;
                break;
            }
            else
            {
                list[j + 1] = list[j];
            }
        }
    }
    return head;
}

void out(T *head, int len)
{
    stack a = {(char *)malloc(sizeof(char) * len), -1};
    _out(head, &a);
    free(a.data);
}

void _out(T *head, stack *p)
{
    if (head->sign != 0)
    {
        p->data[p->last + 1] = '\0';
        printf("%c: %s\n", head->sign, p->data);
        return;
    }
    p->data[++(p->last)] = '0';
    _out(head->left, p);
    p->data[p->last] = '1';
    _out(head->right, p);
    (p->last)--;
}

void freeNodes(T *root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        freeNodes(root->left);
    if (root->right != NULL)
        freeNodes(root->right);
    free(root);
}
