#include "func.h"

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
        number->mantissa_int[count_int] = ctoi(symbol);
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
            number->mantissa_int[count_int] = ctoi(symbol);
            count_int++;
        }
    number->mantissa_int_len = count_int;

    //Чтение дробной части
    if (float_part)
        while ((symbol = getchar()) != EOF)
        {
            if (symbol == '\n' || symbol == 'E' || symbol == 'e')
                break;
            if (count_int + count_float >= MANTISSA_LEN)
                return EXIT_TOO_LONG_NUMBER;
            if (!isdigit(symbol))
                return ERROR_WRONG_SYMBOL;
            number->mantissa_float[count_float] = ctoi(symbol);
            count_float++;
        }
    if (float_part)
        number->mantissa_float_len = count_float;
    else
        number->mantissa_float_len = 1;

    if (symbol == '\n')
    {
        number->degree_sign = '+';
        number->degree = 0;
        return TRUE;
    }

    //Чтение порядка
    symbol = getchar();
    int count_degree = 0;
    if (symbol == '+')
        number->degree_sign = '+';
    else if (symbol == '-')
        number->degree_sign = '-';
    else if (isdigit(symbol))
    {
        number->degree_sign = '+';
        number->degree = ctoi(symbol);
        count_degree = 1;
    }
    else
        return ERROR_WRONG_SYMBOL;
    int ratio = 10;
    while ((symbol = getchar()) != EOF && symbol != '\n')
    {
        if (count_degree >= DEGREE_LEN)
            return TOO_BIG_DEGREE;
        if (!isdigit(symbol))
            return ERROR_WRONG_SYMBOL;
        number->degree = number->degree * ratio + ctoi(symbol);
        count_degree++;
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
    printf("%c", a.degree_sign);
    printf("%d", a.degree);
    printf("\n");
}

int analysis_rc(int rc)
{
    if (rc == 2)
    {
        printf("Sorry, but you have input wrong symbol\n");
        return ERROR_WRONG_SYMBOL;
    }
    if (rc == 3)
    {
        printf("Sorry, but you have input too long mantissa\n");
        return EXIT_TOO_LONG_NUMBER;
    }
    if (rc == 4)
    {
        printf("Sorry, but you have input too long degree\n");
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
    res->degree = a->degree;
    res->degree_sign = a->degree_sign;
    define_sign(res, b->degree);

    //Выравнивание длины мантисс
    if (a->mantissa_float_len > b->mantissa_float_len)
        add_insignificant_zeros(a, b);
    if (b->mantissa_float_len > a->mantissa_float_len)
        add_insignificant_zeros(b, a);

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
        res->mantissa_float[start] += transfer; //Сброс transfer-a
        transfer = 0;
    }

    res->mantissa_int_len = 1;
    res->mantissa_int[0]  = 0;

    return TRUE;
}

int ctoi(const char symbol)
{
    return symbol - '0';
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

int shift_arr(int arr[], int len)
{
    for (int i = 0; i < len - 1; i++)
        arr[i] = arr[i + 1];
    return TRUE;
}

void del_insignificant_zeros(real_number *number)
{
    while (number->mantissa_int[0] == 0)
    {
        if (number->mantissa_int_len == 1)
            break;
        shift_arr(number->mantissa_int, number->mantissa_int_len);
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

void add_insignificant_zeros(real_number *n_1, real_number *n_2)
{
    int len_n_1 = n_1->mantissa_float_len;
    int len_n_2 = n_2->mantissa_float_len;

    while (len_n_1 > len_n_2)
    {
        len_n_2++;
        n_2->mantissa_float[len_n_2] = 0;
    }
    n_2->mantissa_float_len = len_n_2;
}

void define_sign(real_number *number, int new_number)
{
    if (number->degree_sign == '-')
        number->degree *= -1;
    number->degree += new_number;
    if (number->degree < 0)
    {
        number->degree_sign = '-';
        number->degree *= -1;
    }
    else
        number->degree_sign = '+';
}

void round_up(real_number *number, int *new_degree)
{
    int transfer;
    if (number->mantissa_float_len == 1 && number->mantissa_float[0] == 0)
        number->mantissa_float_len--;
    number->mantissa_float_len += number->mantissa_int_len;
    while (number->mantissa_int_len > 0)
    {
        last_elem(number->mantissa_int, &(number->mantissa_int_len), &transfer);
        insert(number->mantissa_float, number->mantissa_float_len, transfer);
        (*new_degree)++;
    }
    number->mantissa_int[0] = 0;
    number->mantissa_int_len++;
}

void round_down(real_number *number, int *new_degree)
{
    while (number->mantissa_float[0] == 0)
    {
        if (number->mantissa_float_len == 1)
            break;
        shift_arr(number->mantissa_float, number->mantissa_float_len);
        number->mantissa_float_len--;
        (*new_degree)--;
    }
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
                        number->degree++;
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
    int new_degree = 0;

    if (number->mantissa_int[0] != 0)
    {
        round_up(number, &new_degree);
        define_sign(number, new_degree);
        return TRUE;
    }

    //Попадаем только если int_arr[0] == 0
    if (number->mantissa_float[0] == 0)
    {
        round_down(number, &new_degree);
        define_sign(number, new_degree);
        return TRUE;
    }

    return TRUE;
}