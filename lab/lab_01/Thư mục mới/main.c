#include <stdio.h>
#include <stdlib.h>
#include "func.c"

int main(void)
{
    printf("This program multiplies two real numbers\n");
    printf("Input two real numbers\n");
    printf("Afer each number click enter\n");
    printf("'.' is equally ','\n");
    printf("'E' is equally 'e'\n\n");

    real_number a = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    printf("                      1        10        20        30\n");
    printf("                      |--------|---------|---------|\n");
    printf("Input number 1: ");
    int rc = read_number(&a);
    if (rc != TRUE)
    {
        analysis_rc(rc);
        return rc;
    }
    del_insignificant_zeros(&a);
    normalization(&a);

    real_number b = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    printf("                       1        10        20        30\n");
    printf("                       |--------|---------|---------|\n");
    printf("Input number 2: ");
    rc = read_number(&b);
    if (rc != TRUE)
    {
        analysis_rc(rc);
        return rc;
    }
    del_insignificant_zeros(&b);
    normalization(&b);

    real_number res = { '\0', {0}, 0, {0}, 0, '\0', 0 };
    real_mult(&a, &b, &res);

    del_insignificant_zeros(&res);
    normalization(&res);
    //Округление
    res_round(&res);
    del_insignificant_zeros(&res);

    if (abs(res.degree) > MAX_DEGREE_VALUE)
    {
        printf("Sorry, but the result is too big\n");
        return TOO_BIG_DEGREE;
    }

    printf("\n                  1        10        20        30\n");
    printf("                  |--------|---------|---------|\n");
    printf("The result is: ");
    real_print(res);
}
