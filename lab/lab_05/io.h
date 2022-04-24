#ifndef IO_H
#define IO_H

#define DONT_INT_NUM_CHOICE   -4
#define INV_NUM_CHOICE        -5
#define OK 0

#include "structures.h"

void input_values(int *n, int *interval, int *log, time_range *T1, time_range *T2, time_range *T3, time_range *T4);
void input_range(time_range *T, int i);
void print_values(int n, int interval, int log, time_range *T1, time_range *T2, time_range *T3, time_range *T4);
void print_range(time_range *T, int i);
int check_number(int *const number, const int l, const int r);
int64_t tick(void);

#endif 