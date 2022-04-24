#include <stdio.h>
#include "io.h"

int64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

void input_values(int *n, int *interval, int *log, time_range *T1, time_range *T2, time_range *T3, time_range *T4){
    do{
        printf("\nInput stop queue1_out value: ");
        fflush(stdin);
    }
    while (scanf("%d", n) != 1);

    do{
        printf("\nShow log? 0(n)/1(y): ");
        fflush(stdin);
    }
    while (scanf("%d", log) != 1);

    if (*log){
        do{
            printf("\nPrint log after: ");
            fflush(stdin);
        }
        while (scanf("%d", interval) != 1);
    }

    int flag = 0;
    do{
        printf("\nChange ranges? 0(n)/1(y): ");
        fflush(stdin);
    }
    while (scanf("%d", &flag) != 1);

    if (flag){
        input_range(T1, 1);
        input_range(T2, 2);
        input_range(T3, 3);
        input_range(T4, 4);
    }
}

void input_range(time_range *T, int i){
    do{
        printf("\nInput T%d_min T%d_max  value: ", i, i);
        fflush(stdin);
    }
    while (scanf("%lf %lf", &(T->min), &(T->max)) != 2);
}

void print_values(int n, int interval, int log, time_range *T1, time_range *T2, time_range *T3, time_range *T4){
    printf("Stop queue1_out value is %d\n", n);

    if (log)
        printf("Show log every %d queue1_out\n", interval);
    else
        printf("Don't show log.\n");

    print_range(T1, 1);
    print_range(T2, 2);
    print_range(T3, 3);
    print_range(T4, 4);
}

void print_range(time_range *T, int i){
    printf("T%d_min: %lf   T%d_max: %lf \n", i, T->min, i, T->max);
}

int check_number(int *const number, const int l, const int r)
{
    int n = 0;
    fflush(stdin);
    n = scanf("%d", number);
    if (n != 1)
        return DONT_INT_NUM_CHOICE;

    if (*number < l || *number > r)
    {
        return INV_NUM_CHOICE;
    }

    return OK;
}