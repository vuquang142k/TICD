#ifndef FUNC_H
#define FUNC_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_MANTISSA_LEN (2 * MANTISSA_LEN)
#define MANTISSA_LEN 30
#define DEGREE_LEN 5

#define FALSE 0
#define TRUE 1

#define ERROR_WRONG_SYMBOL 2
#define EXIT_TOO_LONG_NUMBER 3
#define TOO_BIG_DEGREE 4

#define MAX_DEGREE_VALUE 99999

typedef struct
{
    char mantissa_sign;

    int mantissa_int[MAX_MANTISSA_LEN];
    int mantissa_int_len;
    int mantissa_float[MAX_MANTISSA_LEN];
    int mantissa_float_len;

    char degree_sign;

    int degree;
} real_number;

int read_number(real_number *number);
void arr_print(int arr[], size_t len);
void real_print(real_number a);
int analysis_rc(int rc);

int real_mult(real_number *a, real_number *b, real_number *res);
int ctoi(const char symbol);
int last_elem(int arr[], int *len, int *transfer);
int insert(int arr[], int len, int element);
int shift_arr(int arr[], int len);
void del_insignificant_zeros(real_number *number);
void add_insignificant_zeros(real_number *n_1, real_number *n_2);
void define_sign(real_number *number, int new_number);
void round_up(real_number *number, int *new_degree);
void round_down(real_number *number, int *new_degree);
int normalization(real_number *number);
void res_round(real_number *number);

#endif