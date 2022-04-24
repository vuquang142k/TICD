#include "define.h"
#include "hash_table.h"

unsigned int hash(int info, int key)
{
    return (unsigned int) info % key;
}

hm_cl *new_hm_cl(size_t size)
{
    hm_cl *table = malloc(sizeof(hm_cl));
    table->size = size;
    table->arr_size = 0;
    table->data = new_ht_cl(table->size);
    table->limit = 0;

    return table;
}

ht_cl *new_ht_cl(size_t size)
{
    ht_cl *result = malloc(sizeof(ht_cl) * size);

    if (result != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            result[i].status = 0;
            result[i].info = 0;
        }
    }

    return result;
}

hm_cl *resize_ht_cl(hm_cl *table)
{
    if (table == NULL)
    {
        return NULL;
    }

    size_t tmp = table->size;
    table->size *= MULTIPLIER;
    ht_cl *new = new_ht_cl(table->size);

    for (int i = 0; i < tmp; i++)
    {
        new[i].info = table->data[i].info;
        new[i].status = table->data[i].status;
    }

    free(table->data);
    table->data = new;

    return table;
}

hm_cl *add_ht_cl(hm_cl *table, int info)
{
    if (table == NULL)
    {
        return NULL;
    }

    if (table->arr_size == table->size)
    {
        table = resize_ht_cl(table);
    }

    // printf("\n\n\n\n\n\n%ld\n\n\n\n\n", table->limit);
    while (table->limit >= COLISION_LIM)
    {
        table = restruct_ht_cl(table);
    }

    if (search_ht_cl(table, info))
    {
        return table;
    }

    unsigned int i = hash(info, table->size);
    int cols = 0;

    while (table->data[i % table->size].status == 1)
    {
        i += 1;
        cols += 1;

        if (cols > table->size)
        {
            output("Таблица переполнена!\n");
            break;
        }
    }

    table->limit += cols == 0 ? 0 : 1;
    table->data[i].info = info;
    table->data[i].status = 1;
    table->arr_size++;

    return table;
}

int search_ht_cl(hm_cl *table, int info)
{
    if (table == NULL)
    {
        return 0;
    }

    unsigned int tmp = hash(info, table->size);
    unsigned int i = tmp;
    int cols = 0;

    while (cols < table->size)
    {
        cols += 1;

        if (table->data[i % table->size].info == info && table->data[i % table->size].status == 1)
        {
            return cols;
        }

        i += 1;
    }

    return 0;
}

hm_cl *delete_ht_cl(hm_cl *table, int info)
{
    if (table == NULL)
    {
        return NULL;
    }

    if (search_ht_cl(table, info) == 0)
    {
        printf("%sНе найдено%s\n", RED, RESET);
        return table;
    }

    unsigned int tmp = hash(info, table->size);
    unsigned int i = tmp;
    int cols = 0;

    while (cols < table->size)
    {
        if (table->data[i % table->size].status == 1 && table->data[i % table->size].info == info)
        {
            table->data[i].status = 2;
        }

        cols += 1;
        i += 1;
    }

    return table;
}

void print_ht_cl(const hm_cl *table)
{
    if (table == NULL)
    {
        return;
    }

    printf("Длина: %ld \n", table->size);
    printf("  Коллизий - %d\n", (int)table->limit);
    printf("   Хеш  |  Элемент\n");

    for (int i = 0; i < table->size; i++)
    {
        if (table->data[i].status == 1)
        {
            printf("%6d  | %5d\n", i, table->data[i].info);
        }
    }
}

hm_cl *load_ht_cl_from_file(FILE *file)
{
    hm_cl *table = new_hm_cl(START_SIZE);
    rewind(file);
    int tmp;

    while (fscanf(file, "%d", &tmp) == 1)
    {
        table = add_ht_cl(table, tmp);
    }

    return table;
}

void free_ht_cl(hm_cl *table)
{
    if (table == NULL)
    {
        return;
    }

    free(table->data);
    free(table);
}

hm_cl *restruct_ht_cl(hm_cl *table)
{
    printf("%sРеструктурирование таблицы %d -> %d\n%s", RED, (int)table->size, (int)table->size + START_SIZE, RESET);
    hm_cl *new = new_hm_cl(table->size + START_SIZE);

    for (int i = 0; i < table->size; i++)
    {
        if (table->data[i].status == 1)
        {
            add_ht_cl(new, table->data[i].info);
        }
    }

    free_ht_cl(table);
    return new;
}
