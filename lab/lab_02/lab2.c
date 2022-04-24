#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<inttypes.h>

#define OK 0
#define ERR_OPTION_MUST_BE_NUM -1
#define ERR_OPTION_MUST_BE_FROM_0_TO_13 -2
#define ERR_FILE_MISTAKE -3
#define ERR_NO_SUCH_FILE -42

#define MAX_FLATS 1000

#define MAX_SIZE 1000

#define OK_FILE 0
#define OK_ADDING 0
#define OK_DEL 0

//for file
#define ERR_UNRIGHT_ELEMENTS -3

//for adding
#define ERR_UNRIGHT_CITY_INPUT -4
#define ERR_UNRIGHT_SQUARE_INPUT -5
#define ERR_UNRIGHT_ROOMS_INPUT -6
#define ERR_UNRIGHT_COST_INPUT -7
#define ERR_UNRIGHT_PRIMARY_INPUT -8
#define ERR_UNRIGHT_DECOR_INPUT -9
#define ERR_UNRIGHT_YEAR_INPUT -10
#define ERR_UNRIGHT_OWNER_INPUT -11
#define ERR_UNRIGHT_RESIDENT_INPUT -12
#define ERR_UNRIGHT_ANIMAL_INPUT -13
#define ERR_TOO_MUCH_FLATS -14
#define ERR_FILE_CLEAR -15

#define MAX_SIZE 1000
#define OK_SEARCH 0
#define ERR_UNRIGHT_START_PRICE -15
#define ERR_UNRIGHT_END_PRICE -16
#define ERR_START_CANNOT_BE_BIGGER_THEN_END_PRICE -17
#define GHZ 3500000000

#define MAX_ADDRES_NAME 100
#define FLATS_MAX 1000

//typedef enum BOOLEAN{false = 0, true} bool;

typedef struct
{
    int decorated;
} primary_t;

typedef struct
{
    int build_year;
    int previous_owners;
    int previous_rezidents;
    int animals;
} not_primary_t;

typedef union
{
    primary_t primary;
    not_primary_t secondary;
} primary_check_t;

typedef struct
{
    char address[MAX_ADDRES_NAME + 1];
    int square;
    int rooms;
    int one_m_cost;
    int is_primary;
    primary_check_t flat;
} appartment_t;

typedef struct keys
{
    int square;
    int id;
} keys_t;

typedef struct table
{
    appartment_t *flats_arr;
    keys_t *keys;
    int size;
    int maxsize; 
} table_t;

int check_clear(FILE *f)
{
    if (fseek(f, 0, SEEK_END) != 0)
    {
        return ERR_FILE_CLEAR;
    }

    size_t current_size = ftell(f);

    if (current_size <= 0)
    {
        return ERR_FILE_CLEAR;
    }

    return fseek(f, 0, SEEK_SET);
}

short int read_file(FILE *flats_file, appartment_t *flats_arr, int *size, keys_t *keys)
{
    int num_flat = *size;
    int rc = 0;

    if (check_clear(flats_file) != 0)
    {
        puts("Ошибка: файл пустой");
        return ERR_FILE_CLEAR;
    }

    while (!feof(flats_file))
    {
        // Adress square rooms cost primary -> 1-decor 2 time owners citizens animals
        rc = fscanf(flats_file, "%100s %d %d %d %d", flats_arr[num_flat].address, &flats_arr[num_flat].square, &flats_arr[num_flat].rooms, &flats_arr[num_flat].one_m_cost, &flats_arr[num_flat].is_primary);

        keys[num_flat].square = flats_arr[num_flat].square;
        keys[num_flat].id = num_flat;

        if (rc != 5)
        {
            puts("Ошибка: неверные элементы в файле");
            return ERR_UNRIGHT_ELEMENTS;
        }

        if (flats_arr[num_flat].is_primary == 1)
        {
            primary_t type;
            rc = fscanf(flats_file, "%d", &type.decorated);

            if (rc != 1)
            {
                puts("Ошибка: неверные элементы в файле");
                return ERR_UNRIGHT_ELEMENTS;
            }

            flats_arr[num_flat].flat.primary = type;
        }
        else if (flats_arr[num_flat].is_primary == 2)
        {
            not_primary_t type;

            rc = fscanf(flats_file, "%d %d %d %d", &type.build_year, &type.previous_owners, &type.previous_rezidents, &type.animals);

            if (rc != 4)
            {
                printf("Ошибка: неверные элементы в файле");
                return ERR_UNRIGHT_ELEMENTS;
            }

            flats_arr[num_flat].flat.secondary = type;
        }
        num_flat++;

        if (num_flat > MAX_SIZE)
        {
            puts("Ошибка: количество записей переполнено");
            return ERR_TOO_MUCH_FLATS;
        }
    }

    fseek(flats_file, 0, SEEK_SET);

    *size = num_flat;

    return OK_FILE;
}


short int add_flat(appartment_t *flats_arr, int *size, keys_t *keys)
{
    appartment_t new_flat;
    system("clear"); 

    // Adress square rooms cost primary -> 1-decor 2 time owners citizens animals

    puts("\nВведите город (одно слово без пробелов):");
    if (scanf("%s", new_flat.address) != 1)
    {
        puts("Ошибка: неверно введён город");
        return ERR_UNRIGHT_CITY_INPUT;
    }

    puts("\nВведите площадь квартиры (от 1 до 100000):");
    if ((scanf("%d", &new_flat.square) != 1) || (new_flat.square < 1) || (new_flat.square > 100000))
    {
        puts("Ошибка: неверно введена площадь квартиры");
        return ERR_UNRIGHT_SQUARE_INPUT;
    }

    puts("\nВведите количество комнат (от 1 до 100):");
    if ((scanf("%d", &new_flat.rooms) != 1) || (new_flat.rooms < 1) || (new_flat.rooms > 100))
    {
        puts("Ошибка: неверно введено количество комнат");
        return ERR_UNRIGHT_ROOMS_INPUT;
    }

    puts("\nВведите цену за один квадратный метр квартиры (от 1 до 1000000):");
    if ((scanf("%d", &new_flat.one_m_cost) != 1) || (new_flat.one_m_cost < 1) || (new_flat.one_m_cost > 1000000))
    {
        puts("Ошибка: неверно введена цена за 1 квадртатный метр");
        return ERR_UNRIGHT_COST_INPUT;
    }

    puts("\nВведите класс квартиры: 1 - первичная, 2 - вторичная:");
    if ((scanf("%d", &new_flat.is_primary) != 1) || (new_flat.is_primary < 1) || (new_flat.is_primary > 2))
    {
        puts("Ошибка: неверно введен класс квартиры");
        return ERR_UNRIGHT_PRIMARY_INPUT;
    }

    if (new_flat.is_primary == 1)
    {
        puts("\nЕсть ли декор? (1 - да, 0 - нет):");
        if ((scanf("%d", &new_flat.flat.primary.decorated) != 1) || (new_flat.flat.primary.decorated < 0) || (new_flat.flat.primary.decorated > 1))
        {
            puts("Ошибка: неверно введено наличие декора");
            return ERR_UNRIGHT_DECOR_INPUT;
        }
    }
    else if (new_flat.is_primary == 2)
    {
        puts("\nВведите год постройки (от 1900 до 2020):");
        if ((scanf("%d", &new_flat.flat.secondary.build_year) != 1) || (new_flat.flat.secondary.build_year < 1900) || (new_flat.flat.secondary.build_year > 2020))
        {
            puts("Ошибка: неверно введён год постройки");
            return ERR_UNRIGHT_YEAR_INPUT;
        }

        puts("\nВведите количество прошлых владельцев (от 1 до 1000):");
        if ((scanf("%d", &new_flat.flat.secondary.previous_owners) != 1) || (new_flat.flat.secondary.previous_owners < 1) || (new_flat.flat.secondary.previous_owners > 1000))
        {
            puts("Ошибка: неверно введено количество прошлых владельцев");
            return ERR_UNRIGHT_OWNER_INPUT;
        }

        puts("\nВведите количество прошлых жильцов (от 1 до 1000):");
        if ((scanf("%d", &new_flat.flat.secondary.previous_rezidents) != 1) || (new_flat.flat.secondary.previous_rezidents < 1) || (new_flat.flat.secondary.previous_rezidents > 1000))
        {
            puts("Ошибка: неверно введено количество прошлых жильцов");
            return ERR_UNRIGHT_RESIDENT_INPUT;
        }

        puts("\nБыли ли животные в квартире? (1 - да, 0 - нет):");
        if ((scanf("%d", &new_flat.flat.secondary.animals) != 1) || (new_flat.flat.secondary.animals < 0) || (new_flat.flat.secondary.animals > 1))
        {
            puts("Ошибка: неверно введено наличие животного");
            return ERR_UNRIGHT_ANIMAL_INPUT;
        }
    }

    *size += 1;

    if (*size > MAX_SIZE)
    {
        puts("Ошибка: количество записей переполнено");
        return ERR_TOO_MUCH_FLATS;
    }

    keys[*size - 1].square = new_flat.square;
    keys[*size - 1].id = *size - 1;

    flats_arr[*size - 1] = new_flat;

    puts("\nКвартира была успешно добавлена\n");
    return OK_ADDING;
}


void swap_flats(appartment_t *flats_arr, int i, int j)
{
    appartment_t tmp;

    tmp = flats_arr[i];
    flats_arr[i] = flats_arr[j];
    flats_arr[j] = tmp;
}


void swap_keys(keys_t *keys, int i, int j)
{
    keys_t tmp;

    tmp = keys[i];
    keys[i] = keys[j];
    keys[j] = tmp;
}


void del_by_pos(appartment_t *flats_arr, keys_t *keys, int size, int flat)
{
    for (int i = flat; i < size; i++)
    {
        swap_flats(flats_arr, i, i + 1);
    }

    for (int i = flat; i < size; i++)
    {
        swap_keys(keys, i, i + 1);
        keys[i].id -= 1;
    }
}


short int delete_by_square(appartment_t *flats_arr, int *size, keys_t *keys)
{
    system("clear");

    if (*size == 0)
    {
        puts("\nТаблица пустая\n");
        return OK_DEL;
    }
    puts("Введите значение площади, по которому будут удалены квартиры с такой площадью:");

    int square_del;
    int rc = scanf("%d", &square_del);
    int size_arr = *size;

    if ((rc != 1) || (square_del < 1))
    {
        puts("Ошибка: неверно введена площадь");
        return ERR_UNRIGHT_SQUARE_INPUT;
    }

    for (int flat = 0; flat < size_arr; flat++)
    {
        if (flats_arr[flat].square == square_del)
        {
            del_by_pos(flats_arr, keys, *size, flat);
            *size -= 1;
        }
    }

    if (*size == size_arr)
    {
        puts("\nНе найдены квартиры с такой площадью\n");
    }
    else
    {
        puts("\nУспешно удалены квартиры из таблицы\n");
    }

    return OK_DEL;
}

void bubble_sort_keys(keys_t *keys, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (keys[j].square > keys[j + 1].square)
            {
                swap_keys(keys, j, j + 1);
            }
        }
    }
}


void bubble_sort_flats(appartment_t *flats_arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size- i - 1; j++)
        {
            if (flats_arr[j].square > flats_arr[j + 1].square)
            {
                swap_flats(flats_arr, j, j + 1);
            }
        }
    }
}



void bubble_table(table_t table)
{
    appartment_t *flats_arr = table.flats_arr;

    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }

    bubble_sort_flats(flats_arr, table.size);

    puts("Таблица сортировкой пузырьком успешно отсортирована");

    // table_t table_tmp = {.flats_arr = flats_arr, .size = table.size};

    // print_table(table_tmp);
}


void bubble_table_keys(table_t table)
{
    keys_t *keys = table.keys;

    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }    

    bubble_sort_keys(keys, table.size);

    puts("Таблица ключей сортировкой пузырьком успешно отсортирована");
}


int comparator_table(const void *val1, const void *val2)
{
    return ((appartment_t*)(val1))->square - ((appartment_t*)(val2))->square;
}


void qsort_table(table_t table)
{
    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }
    
    qsort(table.flats_arr, table.size, sizeof(appartment_t), comparator_table);

    puts("Таблица сортировкой qsort успешно отсортирована");
}


int comparator_keys(const void *val1, const void *val2)
{
    return ((keys_t*)(val1))->square - ((keys_t*)(val2))->square;
}


void qsort_table_keys(table_t table)
{   
    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }
    qsort(table.keys, table.size, sizeof(keys_t), comparator_keys);

    puts("Таблица ключей сортировкой qsort успешно отсортирована");
}


uint64_t tick_count(void)
{
    uint32_t high, low;
    __asm__ __volatile__ (
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r" (high), "=r" (low)
        :: "%rax", "%rbx", "%rcx", "%rdx"
        );

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}


void print_result(int64_t start, int64_t end)
{
    printf("\n %ld тактов, %.10lf секунд\n\n", end - start, (double)(end - start)/GHZ);
}


short int compare_sorts(FILE *flats_file)
{
    system("clear");
    int64_t start1, end1, start2, end2, start3, end3, start4, end4;

    appartment_t flats_arr[MAX_SIZE];
    int size = 0;
    keys_t keys[MAX_SIZE];

    int rc = 0;

    rc = read_file(flats_file, flats_arr, &size, keys);

    printf("\n\nСортировка файла из %d записей\n\n", size);
    puts("-------------------------------------------------------------------------");

    start1 = tick_count();
    bubble_sort_flats(flats_arr, size);
    end1 = tick_count();

    puts("Сортировка таблицы пузырьком: ");

    print_result(start1, end1);

    puts("-------------------------------------------------------------------------");

    size = 0;

    rc = read_file(flats_file, flats_arr, &size, keys);

    start2 = tick_count();
    bubble_sort_keys(keys, size);
    end2 = tick_count();

    puts("Сортировка таблицы ключей пузырьком: ");

    print_result(start2, end2);

    size = 0;

    puts("-------------------------------------------------------------------------");

    rc = read_file(flats_file, flats_arr, &size, keys);

    

    start3 = tick_count();
    qsort(flats_arr, size, sizeof(appartment_t), comparator_table);
    end3 = tick_count();

    puts("Сортировка таблицы qsort: ");

    print_result(start3, end3);

    size = 0;

    puts("-------------------------------------------------------------------------");

    rc = read_file(flats_file, flats_arr, &size, keys);

    start4 = tick_count();
    qsort(keys, size, sizeof(keys_t), comparator_table);
    end4 = tick_count();

    puts("Сортировка таблицы ключей qsort: ");

    print_result(start4, end4);


    printf("\n%lu размер массива ключей (в байтах)", sizeof(*keys) * size);
    printf("\n%lu размер таблицы (в байтах)\n\n", sizeof(*flats_arr) * size);

    return rc;
}
void print_string(appartment_t flat)
{
    printf("%-20s | %7d | %6d | %6d |  %5d | ", flat.address, flat.square, flat.rooms, flat.one_m_cost, flat.is_primary);

    if (flat.is_primary == 1)
    {
        if (flat.flat.primary.decorated == 1)
        {
            printf("   Да   |   --   |     --    |   --   |     --   \n");
        }
        else if (flat.flat.primary.decorated == 0)
        {
            printf("   Нет  |   --   |     --    |   --   |     --   \n");
        }
    }
    else if (flat.is_primary == 2)
    {
        printf("   --   | %6d | %9d | %6d |", flat.flat.secondary.build_year, flat.flat.secondary.previous_owners, flat.flat.secondary.previous_rezidents);

        if (flat.flat.secondary.animals == 1)
        {
            puts("    Да   ");
        }
        else if (flat.flat.secondary.animals == 0)
        {
            puts("    Нет   ");
        }
    }
}


void print_table(table_t table)
{
    system("clear"); 

    int size = table.size;

    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }

    appartment_t *flats_arr = table.flats_arr;

    puts("    Адресс (город)   | Площадь | Комнат |  Цена  |  Класс | Отделка |  Год   | Владельцы | Жильцы | Животные");

    puts("--------------------------------------------------------------------------------------------------------------");

    for (int flat_num = 0; flat_num < size; flat_num++)
    {
        print_string(flats_arr[flat_num]);
    }
}


void print_table_keys(table_t table)
{
    system("clear"); 

    int size = table.size;

    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }

    keys_t *keys = table.keys;

    puts("    Таблица ключей");

    for (int key = 0; key < size; key++)
    {
        printf("Номер: %3d  | Площадь: %9d  \n", keys[key].id, keys[key].square);
    }
}

 

void print_table_by_keys(table_t table)
{
    system("clear");
    
    if (table.size == 0)
    {
        puts("Таблица пустая");
        return;
    }

    puts("    Адресс (город)   | Площадь | Комнат |  Цена  |  Класс | Отделка |  Год   | Владельцы | Жильцы | Животные");

    puts("--------------------------------------------------------------------------------------------------------------");
    for (int flat = 0; flat < table.size; flat++)
    {
        print_string(table.flats_arr[table.keys[flat].id]);
    }
}


short int search(table_t table)
{
    system("clear");

    if (table.size == 0)
    {
        puts("Таблица пустая");
        return OK_SEARCH;
    }

    int cost_start, cost_end;

    puts("Введите нижнюю границу цены (не меньше 1):");
    int rc = scanf("%d", &cost_start);

    if ((rc != 1) || (cost_start < 1))
    {
        puts("Ошибка: неверно введена начальная граница");
        return ERR_UNRIGHT_START_PRICE;
    }

    puts("\n\nВведите верхнюю границу цены:");
    rc = scanf("%d", &cost_end);

    if ((rc != 1) || (cost_end < 1))
    {
        puts("Ошибка: неверно введена конечная цена");
        return ERR_UNRIGHT_END_PRICE;
    }

    if (cost_start > cost_end)
    {
        puts("Ошибка: конечная цена не может быть меньше начальной");
        return ERR_START_CANNOT_BE_BIGGER_THEN_END_PRICE;
    }

    int check = 0;

    for (int flat = 0; flat < table.size; flat++)
    {
        if ((table.flats_arr[flat].rooms == 2) && (table.flats_arr[flat].is_primary == 2) && (table.flats_arr[flat].flat.secondary.animals == 0) && (table.flats_arr[flat].one_m_cost >= cost_start) && (table.flats_arr[flat].one_m_cost <= cost_end))
        {
            if (check == 0)
            {
                puts("\n\n    Адресс (город)   | Площадь | Комнат |  Цена  |  Класс | Отделка |  Год   | Владельцы | Жильцы | Животные");

                puts("--------------------------------------------------------------------------------------------------------------");
            }

            print_string(table.flats_arr[table.keys[flat].id]);

            check = 1;
        }
    }

    if (check == 0)
    {
        puts("\n\nНичего не было найдено по заданным границам цены");
    }
    return OK_SEARCH;
}
void print_menu(void)
{
    system("setterm -foreground red");
    puts("\n\n      Пункты программы(Program points):\n");
    system("setterm -foreground green");
    puts("1. Загрузить список квартир из файла (Load list of apartments from file)");
    puts("2. Добавить квартиру в конец списка (Add an apartment to the end of the list)");
    puts("3. Удалить квартиры по значению площади (Remove apartments by area value)");
    puts("4. Найти все двухкомнатное вторичное жильё без живтотных в указанном ценовом диапазоне (Find)");
    puts("5. Отсортировать таблицу пузырьком (Sort table by bubble)");
    puts("6. Отсортировать таблицу ключей пузырьком (Bubble Sort Key's table)");
    puts("7. Отсортировать таблицу qsort (Sort table by qsort)");
    puts("8. Отсортировать таблицу ключей qsort (Sort Key's table by qsort)");
    puts("9. Вывести сравнение сортировок пузырьком и qsort (Print comparison of bubble sorts and qsort)");
    puts("10. Вывести таблицу (Output table)");
    puts("11. Вывести таблицу по таблице ключей (Output a table from a key table)");
    puts("12. Вывести таблицу ключей (Output the key table)");
    puts("13. Очистить таблицу (Clear table)");
    puts("\n 0 - выйти из программы (exit the program)\n\n\n");

    printf("Введите желаемый пункт меню (Enter the desired menu item): ");
}

short int option_menu(void)
{
    int option = -1; 

    int mis, rc = 0;

    appartment_t flats_arr[MAX_FLATS];

    table_t table = {.maxsize = MAX_FLATS, .size = 0};

    keys_t keys[MAX_FLATS];

    while (option != 0)
    {
        print_menu();

        mis = scanf("%d", &option);

        if (mis != 1)
        {
            puts("Ошибка: пункт меню введён неверно");
            return ERR_OPTION_MUST_BE_NUM;
        }

        if ((option < 0) || (option > 13))
        {
            puts("Ошибка: пункт меню введён неверно");
            return ERR_OPTION_MUST_BE_FROM_0_TO_13;
        }

        // File to array
        if (option == 1)
        {
            char file[MAX_SIZE];

            printf("Введите имя файла: ");
            if (scanf("%s", file) != 1)
            {
                puts("Ошибка: неверно введено имя файла");
                return ERR_NO_SUCH_FILE;
            }

            FILE *flats_file = fopen(file, "r");

            system("clear"); 

            if (flats_file == NULL)
            {
                puts("Ошибка: неверно введено имя файла");
                return ERR_FILE_MISTAKE;
            }

            int size = table.size;

            rc = read_file(flats_file, flats_arr, &size, keys);

            if (rc != 0)
            {
                return rc;
            }

            table.size = size;
            table.keys = keys;
            table.flats_arr = flats_arr;

            fclose(flats_file);

            puts("Список квартир из файла был успешно загружен");
        }
        // Add flat info
        else if (option == 2)
        {
            int size = table.size;

            rc = add_flat(flats_arr, &size, keys);

            if (rc != 0)
            {
                return rc;
            }

            table.size = size;
            table.keys = keys;
            table.flats_arr = flats_arr;
        }
        // Delete flats by square
        else if (option == 3)
        {
            int size = table.size;

            rc = delete_by_square(flats_arr, &size, keys);

            if (rc != 0)
            {
                return rc;
            }

            table.size = size;
            table.keys = keys;
            table.flats_arr = flats_arr;
        }
        //Search
        else if (option == 4)
        {
            rc = search(table);

            if (rc != 0)
            {
                return rc;
            }
        }
        //Table bubble
        else if (option == 5)
        {
            system("clear");
            bubble_table(table);
        }
        //Keys bubble
        else if (option == 6)
        {
            system("clear");
            bubble_table_keys(table);
        }
        //Table qsort
        else if (option == 7)
        {
            system("clear");
            qsort_table(table);
        }
        //Keys qsort
        else if (option == 8)
        {
            system("clear");
            qsort_table_keys(table);
        }
        //Compare sorts
        else if (option == 9)
        {
            char file[MAX_SIZE];

            printf("Введите имя файла: ");
            if (scanf("%s", file) != 1)
            {
                puts("Ошибка: неверно введено имя файла");
                return ERR_NO_SUCH_FILE;
            }

            FILE *flats_file = fopen(file, "r");

            if (flats_file == NULL)
            {
                puts("Ошибка: неверно введено имя файла");
                return ERR_FILE_MISTAKE;
            }

            rc = compare_sorts(flats_file);

            if (rc != 0)
            {
                puts("Ошибка: неверные записи в файле");
                return ERR_FILE_MISTAKE;
            }

            fclose(flats_file);
        }
        //Print table
        else if (option == 10)
        {
            print_table(table);
        }
        //Print table by keys
        else if (option == 11)
        {
            print_table_by_keys(table);
        }
        //Print keys
        else if (option == 12)
        {
            print_table_keys(table);
        }
        //Clear table
        else if (option == 13)
        {
            system("clear");
            table.size = 0;
            puts("Таблица успешно очищена\n");
        }
    }

    system("clear");

    return OK;
}
int main(void)
{
    //system("clear"); 
    system("setterm -foreground green");

    int rc = option_menu();

    return rc;
}