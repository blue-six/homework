#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;
typedef struct lln //������
{
    ElemType data;
    struct lln *next;
    struct lln *prev;
} LLN;

LLN *a_head, *a_end; //����a
LLN *b_head, *b_end; //����b
LLN *c_head, *c_end; //����c
unsigned int numbers;

void start(int n);
void hanoi(int n, char A, char B, char C); //hanoi���㷨
void move(char A, int n, char B);
LLN *push(LLN *end, int n);
LLN *pop(LLN *end);
void out(LLN *head, char name);
void freeNodes(LLN *head);

int main()
{
    int n;
    printf("�����뺺ŵ��������\n");
    scanf("%d", &n);
    start(n);
    return 0;
}

void start(int n)
{
    if (n <= 0)
        return;
    a_head = (LLN *)malloc(sizeof(LLN));
    b_head = (LLN *)malloc(sizeof(LLN));
    c_head = (LLN *)malloc(sizeof(LLN));
    int m = n;
    a_end = a_head;
    b_end = b_head;
    c_end = c_head;
    a_end->next = NULL;
    b_end->next = NULL;
    c_end->next = NULL;
    a_end->prev = NULL;
    b_end->prev = NULL;
    c_end->prev = NULL;
    while (n)
    {
        a_end = push(a_end, n--);
    }
    out(a_head, 'a');
    out(b_head, 'b');
    out(c_head, 'c');
    hanoi(m, 'a', 'b', 'c');
    freeNodes(a_head);
    freeNodes(b_head);
    freeNodes(c_head);
    printf("���ƶ�%u��", numbers);
}

void move(char A, int n, char B)
{
    printf("\n%d form %c to %c\n", n, A, B);
    switch (A - 'a')
    {
    case 0:
        a_end = pop(a_end);
        break;
    case 1:
        b_end = pop(b_end);
        break;
    default:
        c_end = pop(c_end);
        break;
    }
    switch (B - 'a')
    {
    case 0:
        a_end = push(a_end, n);
        break;
    case 1:
        b_end = push(b_end, n);
        break;
    default:
        c_end = push(c_end, n);
        break;
    }
    out(a_head, 'a');
    out(b_head, 'b');
    out(c_head, 'c');
}

void hanoi(int n, char A, char B, char C) //hanoi���㷨
{
    if (n == 1)
        move(A, 1, C); //��A�ϵ�һ���̵�C
    else
    {
        hanoi(n - 1, A, C, B); //����������
        move(A, n, C);         //��A�ϵ�n ���̵�C
        hanoi(n - 1, B, A, C); //����������
    }
    numbers++;
}

LLN *push(LLN *end, int n)
{
    end->next = (LLN *)malloc(sizeof(LLN));
    end->next->prev = end;
    end = end->next;
    end->data = n;
    end->next = NULL;
    return end;
}

LLN *pop(LLN *end)
{
    end = end->prev;
    free(end->next);
    end->next = NULL;
    return end;
}

void out(LLN *head, char name)
{
    printf("%c:", name);
    head = head->next;
    while (head != NULL)
    {
        printf("%d  ", head->data);
        head = head->next;
    }
    printf("\n");
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
