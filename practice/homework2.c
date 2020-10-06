#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_SIZE 1024
#define T int

typedef struct lln
{
    T data;
    struct lln *next;
} LinkListNode;

typedef struct Student
{
    int sno;
    char name[20], sex;
    char class[20];
} ElemType;

typedef struct StudentList
{
    ElemType data[LIST_SIZE];
    int last;
} stu_l;

void insert_s(stu_l *sl, ElemType s);
void delete_s(stu_l *sl, int y);
void move_s(ElemType *s1, ElemType *s2);

void move_s(ElemType *s1, ElemType *s2)
{
    s1->sno = s2->sno;
    s1->sex = s2->sex;
    strcpy(s1->name, s2->name);
    strcpy(s1->class, s2->class);
}

void insert_s(stu_l *sl, ElemType s)
{
    if (sl->last < 0)
    {
        move_s(&sl->data[0], &s);
        sl->last = 0;
    }
    else
    {
        int index = sl->last;
        ElemType *p = sl->data;
        do
        {
            if (index > -1 && p[index].sno > s.sno)
            {
                move_s(&p[index + 1], &p[index]);
            }
            else
            {
                move_s(&p[index + 1], &s);
                break;
            }
            index--;
        } while (index >= -1);
        (sl->last)++;
    }
}

void delete_s(stu_l *sl, int y)
{
    int end = sl->last, start = 0;
    int mid = end / 2, index = -1;
    while (index == -1)
    {
        switch (sl->data[mid].sno > y ? 1 : sl->data[mid].sno == y ? 0 : -1)
        {
        case 1:
            if (end == start)
                index = -2;
            else
            {
                end = mid;
                mid = (start + end) / 2;
            }
            break;
        case 0:
            index = mid;
            break;
        case -1:
            if (end == start)
                index = -2;
            else
            {
                start = mid;
                mid = (start + end + 1) / 2;
            }
        }
    }
    if (index >= 0)
    {
        (sl->last)--;
        while (index <= sl->last)
        {
            move_s(&sl->data[index], &sl->data[index + 1]);
            index++;
        }
    }
}

void sub(LinkListNode *a, LinkListNode *b)
{
    LinkListNode *p;
    for (LinkListNode *i = b->next; i != NULL; i = i->next)
    {
        p = a;
        for (LinkListNode *j = a->next; j != NULL; j = j->next)
        {
            if (j->data == i->data)
            {
                p->next = j->next;
                free(j);
                j = p;
            }
            else
                p = j;
        }
    }
}

LinkListNode * and (LinkListNode * a, LinkListNode *b)
{
    LinkListNode *head, *p;
    head = (LinkListNode *)malloc(sizeof(LinkListNode));
    p = head;
    head->next = NULL;
    for (LinkListNode *i = a->next; i != NULL; i = i->next)
    {
        for (LinkListNode *j = b->next; j != NULL; j = j->next)
        {
            if (i->data == j->data)
            {
                p->next = (LinkListNode *)malloc(sizeof(LinkListNode));
                p = p->next;
                p->data = i->data;
                p->next = NULL;
                break;
            }
        }
    }
    return head;
}
