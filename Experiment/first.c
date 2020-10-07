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

int main()
{
    // int sz[] = {2, 6, 4, 7, 3, 0};
    int sz1[] = {2, 6, 4, 7, 3, 9, 0};
    // int sz2[] = {2, 6, 4, 7, 3, 0};
    // int sz3[] = {2, 6, 4, 7, 3, 0};
    LLN *head = Creatlist(sz1);
    LLN *answer = Adjmax(head);
    printf("%d", answer == NULL ? 0 : answer->data);
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
