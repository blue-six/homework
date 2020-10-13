#include <stdio.h>
#include <stdlib.h>

typedef struct tree //哈夫曼树节点
{
    int percent;
    char *sign;
    struct tree *left;
    struct tree *right;
} T;

typedef struct stack //字符栈
{
    char *data;
    int last;
} stack;

void sort(int *p, char **q, int len); //排序
T *make(int *p, char **q, int len);   //构建哈夫曼树
void out(T *head, int len);           //打印输出
void _out(T *head, stack *p);
void freeNodes(T *root);
void _in(int **p, char ***c, int *len);

int main()
{
    // char sign[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    // int prob[] = {5, 9, 7, 8, 14, 6, 3, 11};
    int **prob = (int **)malloc(sizeof(int **)), *len = (int *)malloc(sizeof(int *));
    char ***sign = (char ***)malloc(sizeof(char ***));
    _in(prob, sign, len);
    sort(*prob, *sign, *len);
    T *head = make(*prob, *sign, *len);
    out(head, 8);
    freeNodes(head);
    free(*prob);
    free(prob);
    for (int i = 0; i < *len; i++)
    {
        free((*sign)[i]);
    }
    free(sign);
    return 0;
}

void sort(int *p, char **q, int len)
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
                char *ss;
                s = p[i];
                p[i] = p[j];
                p[j] = s;
                ss = q[i];
                q[i] = q[j];
                q[j] = ss;
                flg = 1;
            }
        }
    }
}

T *make(int *p, char **q, int len)
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
    printf("\n");
    stack a = {(char *)malloc(sizeof(char) * len), -1};
    _out(head, &a);
    free(a.data);
}

void _out(T *head, stack *p)
{
    if (head->sign != 0)
    {
        p->data[p->last + 1] = '\0';
        printf("%s: %s\n", head->sign, p->data);
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

void _in(int **p, char ***c, int *len)
{
    printf("请输入字符集长度：\n");
    scanf("%d", len);
    *p = (int *)malloc((*len) * sizeof(int));
    *c = (char **)malloc((*len) * sizeof(char));
    printf("请输入字符集以及出现频率\n格式：字符 频率(int)\n");
    for (int i = 0; i < *len; i++)
    {
        char *s = (char *)malloc(5 * sizeof(char));
        int n;
        scanf("%s %d", s, &n);
        (*p)[i] = n;
        (*c)[i] = s;
    }
}
