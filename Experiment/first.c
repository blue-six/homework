#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

typedef struct lln
{
    ElemType data;
    struct lln *next;
} LLN;

LLN *Creatlist(int *p);
LLN *Adjmax(LLN *h);
void freeNodes(LLN *head);
LLN *in_Creatlist();

int main()
{
    // int sz[] = {2, 6, 4, 7, 3, 0};
    // int sz1[] = {2, 6, 4, 7, 3, 9, 0};
    // int sz2[] = {2, 6, 4, 7, 3, 0};
    // int sz3[] = {2, 6, 4, 7, 3, 0};
    char flg = 'y';
    while (flg == 'y')
    {
        LLN *head = in_Creatlist();
        LLN *answer = Adjmax(head);
        printf("%d", answer == NULL ? 0 : answer->data);
        freeNodes(head);
        printf("是否继续？(y/n):\n");
        scanf("%c", &flg);
    }
}

LLN *Creatlist(int *p)
{
    LLN *head = (LLN *)malloc(sizeof(LLN)), *q = head;
    for (int i = 0; p[i] != 0; i++)
    {
        // printf("%d", 1);
        q->next = (LLN *)malloc(sizeof(LLN));
        q = q->next;
        q->data = p[i];
    }
    q->next = NULL;
    return head;
}

LLN *Adjmax(LLN *h)
{
    LLN *p = h->next, *q = p == NULL ? NULL : p->next, *point = p;
    long max = LONG_MIN, n;
    while (q != NULL && q->next != NULL)
    {
        // printf("%d", 1);
        n = p->data + q->data + q->next->data;
        if (n > max)
        {
            max = n;
            point = q;
        }
        p = q;
        q = q->next;
    }
    return point;
}

void freeNodes(LLN *head)
{
    while (head != NULL)
    {
        LLN *p = head->next;
        free(head);
        head = p;
    }
}

LLN *in_Creatlist()
{
    int n;
    LLN *head = (LLN *)malloc(sizeof(LLN)), *p = head;
    head->next = NULL;
    printf("请输入数据，以空格分隔：\n");
    scanf("%d", &n);
    while (n != 0)
    {
        p->next = (LLN *)malloc(sizeof(LLN));
        p = p->next;
        p->data = n;
        p->next = NULL;
        scanf("%d", &n);
    }
    return head;
}
