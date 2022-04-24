#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_MANTISSA_LEN (2 * MANTISSA_LEN)
#define MANTISSA_LEN 30
#define POWER_LEN 5

#define FALSE 0
#define TRUE 1

#define ERROR_WRONG_SYMBOL 2
#define EXIT_TOO_LONG_NUMBER 3
#define TOO_BIG_POWER 4

#define MAX_POWER_VALUE 99999

typedef struct
{
    char mantissa_sign;

    int mantissa_int[MAX_MANTISSA_LEN];
    int mantissa_int_len;
    int mantissa_float[MAX_MANTISSA_LEN];
    int mantissa_float_len;

    char power_sign;

    int power;
} real_number;

int read_number(real_number *number);
void arr_print(int arr[], size_t len);
void real_print(real_number a);
int analysis_rc(int rc);

int real_mult(real_number *a, real_number *b, real_number *res);
int symbol_to_int(const char symbol);
int last_elem(int arr[], int *len, int *transfer);
int insert(int arr[], int len, int element);
int left_arr(int arr[], int len);
void del_zeros(real_number *number);
int normalization(real_number *number);
void res_round(real_number *number);

int main(void)
{
    printf("This program multiplies two real numbers\n");
    printf("Input two real numbers\n");

    real_number a = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    printf("                1        10        20        30\n");
    printf("                |--------|---------|---------|\n");
    printf("Input number 1: ");
    int rc = read_number(&a);
    if (rc != TRUE)
    {
        analysis_rc(rc);
        return rc;
    }
    del_zeros(&a);
    normalization(&a);

    real_number b = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    printf("                1        10        20        30\n");
    printf("                |--------|---------|---------|\n");
    printf("Input number 2: ");
    rc = read_number(&b);
    if (rc != TRUE)
    {
        analysis_rc(rc);
        return rc;
    }
    del_zeros(&b);
    normalization(&b);

    real_number res = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    real_mult(&a, &b, &res);

    del_zeros(&res);
    normalization(&res);
    //Округление
    res_round(&res);
    del_zeros(&res);

    if (abs(res.power) > MAX_POWER_VALUE)
    {
        printf("Sorry, but the result is too big\n");
        return TOO_BIG_POWER;
    }

    printf("\n             1        10        20        30\n");
    printf("               |--------|---------|---------|\n");
    printf("The result is: ");
    real_print(res);
}

int read_number(real_number *number)
{
    int count_int = 0;
    int count_float = 0;
    int int_part = 1;
    int float_part = 0;

    char symbol = getchar();

    //Проверка символа на знак
    if (symbol == '+')
        number->mantissa_sign = '+';
    else if (symbol == '-')
        number->mantissa_sign = '-';
    else if (isdigit(symbol))
    {
        number->mantissa_sign = '+';
        number->mantissa_int[count_int] = symbol_to_int(symbol);
        count_int++;
    }
    else if (symbol == '.' || symbol == ',')
    {
        number->mantissa_sign = '+';
        int_part = 0;
        float_part = 1;
    }
    else
        return ERROR_WRONG_SYMBOL;

    //Чтение целой части
    if (int_part)
        while ((symbol = getchar()) != EOF)
        {
            if (symbol == '.' || symbol == ',')
            {
                float_part = 1;
                break;
            }
            if (symbol == '\n' || symbol == 'E' || symbol == 'e')
                break;
            if (count_int >= MANTISSA_LEN)
                return EXIT_TOO_LONG_NUMBER;
            if (!isdigit(symbol))
                return ERROR_WRONG_SYMBOL;
            number->mantissa_int[count_int] = symbol_to_int(symbol);
            count_int++;
        }
    number->mantissa_int_len = count_int;

    if (float_part)
        while ((symbol = getchar()) != EOF)
        {
            if (symbol == '\n' || symbol == 'E' || symbol == 'e')
                break;
            if (count_int + count_float >= MANTISSA_LEN)
                return EXIT_TOO_LONG_NUMBER;
            if (!isdigit(symbol))
                return ERROR_WRONG_SYMBOL;
            number->mantissa_float[count_float] = symbol_to_int(symbol);
            count_float++;
        }
    if (float_part)
        number->mantissa_float_len = count_float;
    else
        number->mantissa_float_len = 1;

    if (symbol == '\n')
    {
        number->power_sign = '+';
        number->power = 0;
        return TRUE;
    }

    symbol = getchar();
    int count_power = 0;
    if (symbol == '+')
        number->power_sign = '+';
    else if (symbol == '-')
        number->power_sign = '-';
    else if (isdigit(symbol))
    {
        number->power_sign = '+';
        number->power = symbol_to_int(symbol);
        count_power = 1;
    }
    else
        return ERROR_WRONG_SYMBOL;
    int ratio = 10;
    while ((symbol = getchar()) != EOF && symbol != '\n')
    {
        if (count_power >= POWER_LEN)
            return TOO_BIG_POWER;
        if (!isdigit(symbol))
            return ERROR_WRONG_SYMBOL;
        number->power = number->power * ratio + symbol_to_int(symbol);
        count_power++;
    }
    return TRUE;
}

void arr_print(int arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%d", arr[i]);
}

void real_print(real_number a)
{
    printf("%c", a.mantissa_sign);
    for (int i = 0; i < a.mantissa_int_len; i++)
        printf("%d", a.mantissa_int[i]);
    printf(".");
    for (int i = 0; i < a.mantissa_float_len; i++)
        printf("%d", a.mantissa_float[i]);
    printf("E");
    printf("%c", a.power_sign);
    printf("%d", a.power);
    printf("\n");
}

int analysis_rc(int rc)
{
    if (rc == 2)
    {
        printf("ERROR_WRONG_SYMBOL\n");
        return ERROR_WRONG_SYMBOL;
    }
    if (rc == 3)
    {
        printf("EXIT_TOO_LONG_NUMBER\n");
        return EXIT_TOO_LONG_NUMBER;
    }
    if (rc == 4)
    {
        printf("EXIT_TOO_LONG_NUMBER\n");
        return EXIT_TOO_LONG_NUMBER;
    }
    return TRUE;
}

int real_mult(real_number *a, real_number *b, real_number *res)
{
    //Знак мантиссы
    if (a->mantissa_sign == b->mantissa_sign)
        res->mantissa_sign = '+';
    else
        res->mantissa_sign = '-';

    //Знак порядка
    res->power = a->power;
    res->power_sign = a->power_sign;
    if (res->power_sign == '-')
        res->power *= -1;
    res->power += b->power;
    if (res->power < 0)
    {
        res->power_sign = '-';
        res->power *= -1;
    }
    else
        res->power_sign = '+';

    //Выравнивание длины мантисс
    if (a->mantissa_float_len > b->mantissa_float_len)
    {
        int len_a = a->mantissa_float_len;
        int len_b = b->mantissa_float_len;

        while (len_a > len_b)
        {
            len_b++;
            b->mantissa_float[len_b] = 0;
        }
        b->mantissa_float_len = len_b;
    }
    if (b->mantissa_float_len > a->mantissa_float_len)
    {
        int len_b = b->mantissa_float_len;
        int len_a = a->mantissa_float_len;

        while (len_b > len_a)
        {
            len_a++;
            a->mantissa_float[len_a] = 0;
        }
        a->mantissa_float_len = len_a;
    }

    int len_a = a->mantissa_float_len;
    int len_b = b->mantissa_float_len;

    int len_res = len_a + len_b;
    res->mantissa_float_len = len_res;

    int transfer = 0;
    int value = 0;
    int n = 1;
    for (int i = len_b - 1; i >= 0; i--)
    {
        int start = len_res - n;
        for (int j = len_a - 1; j >= 0; j--)
        {
            int tmp = b->mantissa_float[i] * a->mantissa_float[j];
            value = tmp % 10;
            res->mantissa_float[start] += (value + transfer) % 10;

            transfer = tmp / 10 + (value + transfer) / 10;

            if (res->mantissa_float[start] > 9)
            {
                res->mantissa_float[start] %= 10;
                transfer += 1;
            }
            start--;
        }
        n++;
        res->mantissa_float[start] += transfer;
        transfer = 0;
    }

    res->mantissa_int_len = 1;
    res->mantissa_int[0]  = 0;

    return TRUE;
}

int symbol_to_int(const char a)
{
    return a - '0';
}

int last_elem(int arr[], int *len, int *transfer)
{
    *transfer = arr[(*len) - 1];
    (*len)--;
    return TRUE;
}

int insert(int arr[], int len, int element)
{
    for (int i = len - 1; i >= 0; i--)
        arr[i + 1] = arr[i];
    arr[0] = element;
    return TRUE;
}

int left_arr(int arr[], int len)
{
    for (int i = 0; i < len - 1; i++)
        arr[i] = arr[i + 1];
    return TRUE;
}

void del_zeros(real_number *number)
{
    while (number->mantissa_int[0] == 0)
    {
        if (number->mantissa_int_len == 1)
            break;
        left_arr(number->mantissa_int, number->mantissa_int_len);
        number->mantissa_int_len--;
    }

    int len = number->mantissa_float_len;
    while (number->mantissa_float[len - 1] == 0)
    {
        if (len == 1)
            break;
        len--;
    }
    number->mantissa_float_len = len;
}

void res_round(real_number *number)
{
    int transfer = 0;
    int i = 1;
    if (number->mantissa_float_len > MANTISSA_LEN)
    {
        if (number->mantissa_float[MANTISSA_LEN] >= 5)
        {
            number->mantissa_float[MANTISSA_LEN - i]++;
            while (number->mantissa_float[MANTISSA_LEN - i] > 9)
            {
                transfer = number->mantissa_float[MANTISSA_LEN - i] / 10;
                number->mantissa_float[MANTISSA_LEN - i] %= 10;
                i++;
                number->mantissa_float[MANTISSA_LEN - i] += transfer;
                if (i == MANTISSA_LEN)
                {
                    if (number->mantissa_float[0] > 9)
                    {
                        number->mantissa_float[0] = 1;
                        number->power++;
                    }
                    break;
                }
            }
        }
        number->mantissa_float_len = MANTISSA_LEN;
    }
}

int normalization(real_number *number)
{
    int new_power = 0;

    if (number->mantissa_int[0] != 0)
    {
        int transfer;
        if (number->mantissa_float_len == 1 && number->mantissa_float[0] == 0)
            number->mantissa_float_len--;
        number->mantissa_float_len += number->mantissa_int_len;
        while (number->mantissa_int_len > 0)
        {
            last_elem(number->mantissa_int, &(number->mantissa_int_len), &transfer);
            insert(number->mantissa_float, number->mantissa_float_len, transfer);
            new_power++;
        }
        number->mantissa_int[0] = 0;
        number->mantissa_int_len++;
        if (number->power_sign == '-')
        number->power *= -1;
        number->power += new_power;
        if (number->power < 0)
        {
            number->power_sign = '-';
            number->power *= -1;
        }
        else
            number->power_sign = '+';
        return TRUE;
    }

    //Попадаем только если int_arr[0] == 0
    if (number->mantissa_float[0] == 0)
    {
        while (number->mantissa_float[0] == 0)
        {
            if (number->mantissa_float_len == 1)
                break;
            left_arr(number->mantissa_float, number->mantissa_float_len);
            number->mantissa_float_len--;
            new_power--;
        }
        if (number->power_sign == '-')
        number->power *= -1;
        number->power += new_power;
        if (number->power < 0)
        {
            number->power_sign = '-';
            number->power *= -1;
        }
        else
            number->power_sign = '+';
        return TRUE;
    }
    return TRUE;
}