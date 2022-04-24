#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define OK 0
#define IO_ERROR 1
#define EMPTY_MATRIX_ERROR 2
#define MENU_NUMBER_ERROR 3
#define MATRIX_GENERATE_ERROR 4

int input_matrix_default_format(int **matrix, int *const n, int *const m, int *const number_nonzero_el)
{
    printf("Введите количество строк матрицы (> 0): ");
    if (fscanf(stdin, "%d", n) != 1)
        return IO_ERROR;
    if (*n < 1)
        return IO_ERROR;

    printf("Введите количество столбцов матрицы (> 0): ");
    if (fscanf(stdin, "%d", m) != 1)
        return IO_ERROR;
    if (*m < 1)
        return IO_ERROR;

    printf("Введите количество ненулевых элементов матрицы (>= 0): ");
    if (fscanf(stdin, "%d", number_nonzero_el) != 1)
        return IO_ERROR;
    if (*number_nonzero_el < 0 || *number_nonzero_el > *n * *m)
        return IO_ERROR;

    *matrix = (int*)calloc(*n * *m, sizeof(int));

    for (int _ = 0, i, j, val; _ < *number_nonzero_el; _++)
    {
        printf("Введите номер строки элемента (от 0 и так далее): ");
        if (fscanf(stdin, "%d", &i) != 1)
            return IO_ERROR;
        if (i < 0 || i >= *n)
            return IO_ERROR;
            
        printf("Введите номер столбца элемента (от 0 и так далее): ");
        if (fscanf(stdin, "%d", &j) != 1)
            return IO_ERROR;
        if (j < 0 || j >= *m)
            return IO_ERROR;

        printf("Введите значение элемента (!= 0): ");
        if (fscanf(stdin, "%d", &val) != 1)
            return IO_ERROR;
        if (val == 0)
            return IO_ERROR;
        *(*matrix + i * *m + j) = val;
    }

    return OK;
}

void print_matrix_default_format(int *const matrix, const int n, const int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%4d", *(matrix + i * m + j));
        }
        printf("\n");
    }
}

void print_matrix_sparse_format(int *a, int *ai, int *ja, int *pa, int *pai, int *pja)
{
    printf(" A = [ ");
    for (; a < pa; a++)
    {
        printf("%d ", *a);
    }
    printf("]\n");

    printf("IA = [ ");
    for (; ai < pai; ai++)
    {
        printf("%d ", *ai);
    }
    printf("]\n");

    printf("JA = [ ");
    for (; ja < pja; ja++)
    {
        printf("%d ", *ja);
    }
    printf("]\n");
}

void default_to_sparse(int *const matrix, const int n, const int m, \
const int number_nonzero_el, int **a, int **ai, int **ja, int **pa, int **pai, int **pja)
{
    *a = (int*)malloc(number_nonzero_el * sizeof(int));
    *ai = (int*)malloc(number_nonzero_el * sizeof(int));
    *ja = (int*)malloc((m + 1) * sizeof(int));
    *pa = *a;
    *pai = *ai;
    *pja = *ja;

    int a_ind = 0, ja_ind = 0;
    *(*ja + ja_ind) = 0;
    ja_ind++;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (*(matrix + j * m + i) != 0)
            {
                *(*a + a_ind) = *(matrix + j * m + i);
                *(*ai + a_ind) = j;
                a_ind++;
            }
        }
        *(*ja + ja_ind) = a_ind;
        ja_ind++;
    }

    *pa = *a + a_ind;
    *pai = *ai + a_ind;
    *pja = *ja + ja_ind;

}

int matrix_generate(int **matrix, int *const n, int *const m, int *const number_nonzero_el)
{
    printf("Введите количество строк матрицы (> 0): ");
    if (fscanf(stdin, "%d", n) != 1)
        return MATRIX_GENERATE_ERROR;
    if (*n < 1)
        return MATRIX_GENERATE_ERROR;

    printf("Введите количество столбцов матрицы (> 0): ");
    if (fscanf(stdin, "%d", m) != 1)
        return MATRIX_GENERATE_ERROR;
    if (*m < 1)
        return MATRIX_GENERATE_ERROR;

    float procent_matrix_sparse;
    printf("Введите процент разреженности матрицы (от 0 до 99): ");
    if (fscanf(stdin, "%f", &procent_matrix_sparse) != 1)
        return MATRIX_GENERATE_ERROR;
    if (procent_matrix_sparse < 0 || procent_matrix_sparse > 99)
        return MATRIX_GENERATE_ERROR;

    *matrix = (int*)calloc(*n * *m, sizeof(int));

    *number_nonzero_el = (procent_matrix_sparse / 100 * (*n * *m));

    srand(time(NULL));
    for (int i = 0, row, column; i < *number_nonzero_el; i++)
    {
        row = rand() % *n;
        column = rand() % *m;
        while (*(*matrix + row * *m + column) != 0)
        {
            row = rand() % *n;
            column = rand() % *m;
        }
        *(*matrix + row * *m + column) = rand() % 100 + 1;  
    }

    return OK;
}

void default_matrix_sum(int **matrix3, int *n3, int *m3, int *const matrix1, \
int *const matrix2, const int n, const int m)
{
    *n3 = n;
    *m3 = m;
    *matrix3 = (int *)malloc(n * m * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            *(*matrix3 + i * m + j) = *(matrix1 + i * m + j) + *(matrix2 + i * m + j);
        }
    }
}

void sparse_matrix_sum(int **a3, int **ai3, int **ja3, int **pa3, int **pai3, int **pja3, \
int *a1, int *ai1, int *ja1, int *pa1, int *pai1, int *pja1, int *a2, int *ai2, int *ja2, \
int *pa2, int *pai2, int *pja2, const int number_nonzero_el1, const int number_nonzero_el2, const int m)
{
    *a3 = (int *)malloc((number_nonzero_el1 + number_nonzero_el2) * sizeof(int));
    *ai3 = (int *)malloc((number_nonzero_el1 + number_nonzero_el2) * sizeof(int));
    *ja3 = (int *)malloc((m + 1) * sizeof(int));

    *pa3 = *a3;
    *pai3 = *ai3;
    *pja3 = *ja3;

    int size = 0, js_size = 0, check = 0;
    *(*ja3 + js_size) = 0;
    js_size++;
    for(int i = 1; i < pja1 - ja1; i++)
    {
        for(int j = *(ja1 + i - 1); j < *(ja1 + i); j++)
        {
            check = 0;
            for(int k = *(ja2 + i - 1); k < *(ja2 + i); k++)
            {
                if (*(ai1 + j) == *(ai2 + k))
                {
                    if ((*(a1 + j) + *(a2 + k)) != 0)
                    {
                        *(*a3 + size) = *(a1 + j) + *(a2 + k);
                        *(*ai3 + size) = *(ai1 + j);
                        size++;
                    }
                    check = 1;
                }
            }
            if (check == 0)
            {
                *(*a3 + size) = *(a1 + j);
                *(*ai3 + size) = *(ai1 + j);
                size++;
            }
        }
        for(int k = *(ja2 + i - 1); k < *(ja2 + i); k++)
        {
            check = 0;
            for(int j = *(ja1 + i - 1); j < *(ja1 + i); j++)
            {
                if (*(ai1 + j) == *(ai2 + k))
                    check = 1;
            }
            if (check == 0)
            {
                *(*a3 + size) = *(a2 + k);
                *(*ai3 + size) = *(ai2 + k);
                size++;
            }
        }
        *(*ja3 + js_size) = size;
        js_size++;
    }

    *pa3 = *a3 + size;
    *pai3 = *ai3 + size;
    *pja3 = *ja3 + js_size;

    *a3 = (int*)realloc(*a3, (*pa3 - *a3) * sizeof(int));
    *ai3 = (int*)realloc(*ai3, (*pai3 - *ai3) * sizeof(int));
}

void free_all(int **matrix1, int **matrix2, int **matrix3, int **a1, \
int **a2, int **a3, int **ai1, int **ai2, int **ai3, int **ja1, int **ja2, int **ja3)
{
    if (*matrix1)
        free(*matrix1);
    if (*matrix2)
        free(*matrix2);
    if (*matrix3)
        free(*matrix3);
    if (*a1)
        free(*a1);
    if (*ai1)
        free(*ai1);
    if (*ja1)
        free(*ja1);
    if (*a2)
        free(*a2);
    if (*ai2)
        free(*ai2);
    if (*ja2)
        free(*ja2);
    if (*a3)
        free(*a3);
    if (*ai3)
        free(*ai3);
    if (*ja3)
        free(*ja3);        
}

void menu(void)
{
    printf("\n\n                                == МЕНЮ ==\n\n");
    printf("1  - Ввести две матрицы с клавиатуры\n");
    printf("2  - Сгенерировать две матрицы с заданным процентом разреженности\n");
    printf("3  - Сложить две матрицы в стандартном формате\n");
    printf("4  - Сложить две матрицы в столбовой формате\n");
    printf("5  - Вывести две матрицы в обычном формате\n");
    printf("6  - Вывести две матрицы в столбовой формате\n");
    printf("7  - Сравнить время выполнения операций и объем памяти при сложении матриц в разных форматах\n");
    printf("0  - Выйти\n");
    printf("\nВведите номер действия, которое ходите произвести: ");
}

void print_error(const int error_code)
{
    if (error_code == IO_ERROR)
        system("echo \x1B[31mОшибка:\x1B[0m вы ввели матрицу некорректно");
    if (error_code == EMPTY_MATRIX_ERROR)
        system("echo \x1B[31mОшибка:\x1B[0m вы не ввели матрицы");
    if (error_code == MENU_NUMBER_ERROR)
        system("echo \x1B[31mОшибка:\x1B[0m вы ввели неверный пункт меню");
    if (error_code == MATRIX_GENERATE_ERROR)
        system("echo \x1B[31mОшибка:\x1B[0m вы ввели неверные данные для генерации матрицы");
}

int main(void)
{
    int *matrix1 = NULL;
    int n1, m1;
    int number_nonzero_el1 = 0;
    int *a1 = NULL, *ai1 = NULL, *ja1 = NULL;
    int *pa1 = NULL, *pai1 = NULL, *pja1 = NULL; 

    int *matrix2 = NULL;
    int n2, m2;
    int number_nonzero_el2 = 0;
    int *a2 = NULL, *ai2 = NULL, *ja2 = NULL;
    int *pa2 = NULL, *pai2 = NULL, *pja2 = NULL;

    int *matrix3 = NULL;
    int n3, m3;
    int *a3 = NULL, *ai3 = NULL, *ja3 = NULL;
    int *pa3 = NULL, *pai3 = NULL, *pja3 = NULL; 

    int menu_number;

    system("figlet -w 200 Hello, user!");
    do
    {
        menu();
        //fpurge(stdin);
        int rc = fscanf(stdin, "%d", &menu_number);

        if (menu_number < 0 || menu_number > 7 || rc != 1)
        {
            print_error(MENU_NUMBER_ERROR);
            continue;
        }
        if (menu_number == 1)
        {
            printf("\n\x1B[32mВведите первую матрицу:\x1B[0m\n");
            if (input_matrix_default_format(&matrix1, &n1, &m1, &number_nonzero_el1) != OK)
            {
                print_error(IO_ERROR);
                if (matrix1)
                    free(matrix1);
                matrix1 = NULL;
                continue;
            }
            printf("\n\x1B[32mВведите вторую матрицу:\x1B[0m\n");
            if (input_matrix_default_format(&matrix2, &n2, &m2, &number_nonzero_el2) != OK || \
            m1 != m2 || n1 != n2 || (number_nonzero_el1 == 0 && number_nonzero_el2 == 0))
            {
                print_error(IO_ERROR);
                if (matrix1)
                    free(matrix1);
                if (matrix2)
                    free(matrix2);
                matrix1 = NULL;
                matrix2 = NULL;
                continue;
            }
            default_to_sparse(matrix1, n1, m1, number_nonzero_el1, &a1, &ai1, &ja1, &pa1, &pai1, &pja1);
            default_to_sparse(matrix2, n2, m2, number_nonzero_el2, &a2, &ai2, &ja2, &pa2, &pai2, &pja2);
            continue;
        }
        if (menu_number == 2)
        {
            printf("\n\x1B[32mГенерирование первой матрицы:\x1B[0m\n");
            if (matrix_generate(&matrix1, &n1, &m1, &number_nonzero_el1) != OK)
            {
                print_error(MATRIX_GENERATE_ERROR);
                if (matrix1)
                    free(matrix1);
                matrix1 = NULL;
                continue;
            }
            printf("\n\x1B[32mГенерирование второй матрицы:\x1B[0m\n");
            if (matrix_generate(&matrix2, &n2, &m2, &number_nonzero_el2) != OK || \
            m1 != m2 || n1 != n2)
            {
                print_error(MATRIX_GENERATE_ERROR);
                if (matrix1)
                    free(matrix1);
                if (matrix2)
                    free(matrix2);
                matrix1 = NULL;
                matrix2 = NULL;
                continue;
            }
            default_to_sparse(matrix1, n1, m1, number_nonzero_el1, &a1, &ai1, &ja1, &pa1, &pai1, &pja1);
            default_to_sparse(matrix2, n2, m2, number_nonzero_el2, &a2, &ai2, &ja2, &pa2, &pai2, &pja2);
            continue;
        }
        if (menu_number == 3)
        {
            if (matrix1 == NULL || matrix2 == NULL)
            {
                print_error(EMPTY_MATRIX_ERROR);
                continue;
            }
            default_matrix_sum(&matrix3, &n3, &m3, matrix1, matrix2, n2, m2);
            printf("\n\x1B[32mРезультат сложения матриц:\x1B[0m\n");
            print_matrix_default_format(matrix3, n3, m3);
            continue;

        }
        if (menu_number == 4)
        {
            if (matrix1 == NULL || matrix2 == NULL)
            {
                print_error(EMPTY_MATRIX_ERROR);
                continue;
            }
            sparse_matrix_sum(&a3, &ai3, &ja3, &pa3, &pai3, &pja3, a1, ai1, ja1, pa1, pai1, pja1, a2, ai2, ja2, \
            pa2, pai2, pja2, number_nonzero_el1, number_nonzero_el2, m1);
            printf("\n\x1B[32mРезультат сложения матриц:\x1B[0m\n");
            print_matrix_sparse_format(a3, ai3, ja3, pa3, pai3, pja3);
            continue;
        }
        if (menu_number == 5)
        {
            if (matrix1 == NULL || matrix2 == NULL)
            {
                print_error(EMPTY_MATRIX_ERROR);
                continue;
            }
            printf("\n\x1B[32mВывод первой матрицы:\x1B[0m\n");
            print_matrix_default_format(matrix1, n1, m1);
            printf("\n\x1B[32mВывод второй матрицы:\x1B[0m\n");
            print_matrix_default_format(matrix2, n2, m2);
            continue;
        }
        if (menu_number == 6)
        {
            if (matrix1 == NULL || matrix2 == NULL)
            {
                print_error(EMPTY_MATRIX_ERROR);
                continue;
            }
            printf("\n\x1B[32mВывод первой матрицы:\x1B[0m\n");
            print_matrix_sparse_format(a1, ai1, ja1, pa1, pai1, pja1);
            printf("\n\x1B[32mВывод второй матрицы:\x1B[0m\n");
            print_matrix_sparse_format(a2, ai2, ja2, pa2, pai2, pja2);
            continue;
        }
        if (menu_number == 7)
        {
            if (matrix1 == NULL || matrix2 == NULL)
            {
                print_error(EMPTY_MATRIX_ERROR);
                continue;
            }

            struct timeval ta, te;

            printf("Размер %d x %d:\n", n2, m2);
            printf("Разреженности процент: %.1f", (float)number_nonzero_el1/(n2 * m2)*100);

            printf("\n\x1B[32mСтандартный формат\x1B[0m\n");
            printf("Скорость работы: ");
            gettimeofday(&ta, NULL);
            default_matrix_sum(&matrix3, &n3, &m3, matrix1, matrix2, n2, m2);
            gettimeofday(&te, NULL);
            printf("%lf sec\n",\
            te.tv_sec - ta.tv_sec + (te.tv_usec - ta.tv_usec)/1000000.0);
            printf("Объем памяти: ");
            printf("%lu\n", n3 * m3 * sizeof(int));

            printf("\n\x1B[32mСтолбовой формат\x1B[0m\n");
            printf("Скорость работы: ");
            gettimeofday(&ta, NULL);
            sparse_matrix_sum(&a3, &ai3, &ja3, &pa3, &pai3, &pja3, a1, ai1, ja1, pa1, pai1, pja1, a2, ai2, ja2, \
            pa2, pai2, pja2, number_nonzero_el1, number_nonzero_el2, m1);
            gettimeofday(&te, NULL);
            printf("%lf sec\n",\
            te.tv_sec - ta.tv_sec + (te.tv_usec - ta.tv_usec)/1000000.0);
            printf("Объем памяти: ");
            printf("%lu\n", (pa3 - a3) * sizeof(int) + (pai3 - ai3) * sizeof(int) + (pja3 - ja3) * sizeof(int));
        }
        if (menu_number == 0)
        {
            system("figlet -w 200 Exit");
            free_all(&matrix1, &matrix2, &matrix3, &a1, &a2, &a3, &ai1, &ai2, &ai3, &ja1, &ja2, &ja3);
            return OK;
        }
    } while (1);
}