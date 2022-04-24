#include<stdio.h>
#include"func.c"

int main(void)
{
    print_rules();
    short int rc = 0;
    float_number number_1;

    puts("                                 -+|........|.........|..........|E-+|...|");
    printf("Input number 1:  ");

    rc = read_number(&number_1);
    if (rc != 0)
    {
        return rc;
    }

    normalization(&number_1);

    float_number number_2;
    puts("\n                                 -+|........|.........|..........|E-+|...|");
    printf("Input number 2:  ");

    rc = read_number(&number_2);

    if (rc != 0)
    {
        return rc;
    }

    normalization(&number_2);

    int result[60] = {0}, res_power, ind;

    rc = multiply(number_1, number_2, result, &res_power, &ind);

    if (rc != 0)
    {
        return rc;
    }
    print_result(number_1, number_2, result, res_power, ind);
    return rc;
}
