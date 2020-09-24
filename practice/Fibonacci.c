#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int point = 1;

long fib0(int n);
unsigned long long fib1(int n);
unsigned long long fib2(int n);
unsigned long long fib3(int n);
double count_time(unsigned long long (*f)(int));

int main()
{
    // for (int i = 0; i < 100; i++)
    //     printf("%llu\n", fib1(i + 1));
    printf("%e ms\n", count_time(fib1));
    printf("%e ms\n", count_time(fib2));
    printf("%e ms\n", count_time(fib3));
    return 1;
}

double count_time(unsigned long long (*f)(int))
{
    clock_t start = clock(), end;
    for (int i = 0; i < 1000000; i++)
    {
        (*f)(100);
        point = 1;
    }
    end = clock();
    return (double)(end - start) / 1000000;
}

long fib0(int n)
{
    if (n <= 2)
        return 1;
    else
        return fib0(n - 1) + fib0(n - 2);
}

unsigned long long fib1(int n)
{
    static unsigned long long list[100] = {1, 1};
    if (n - 1 <= point)
        return list[n - 1];
    else
    {
        list[++point] = fib1(n - 1) + fib1(n - 2);
        return list[point];
    }
}

unsigned long long fib2(int n)
{
    unsigned long long list[n];
    list[0] = 1, list[1] = 1;
    for (short i = 2; i < n; i++)
    {
        list[i] = list[i - 1] + list[i - 2];
    }
    return list[n - 1];
}

unsigned long long fib3(int n)
{
    unsigned long long list[2] = {1, 1};
    for (int i = 2; i < n; i++)
    {
        list[i % 2] = list[0] + list[1];
    }
    return list[(n - 1) % 2];
}
