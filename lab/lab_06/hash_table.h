#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "tree.h"

#define START_SIZE 5
#define MULTIPLIER 2.0
#define COLISION_LIM 10

typedef struct hash_table_closed
{
    int info;
    int status;
} ht_cl;

typedef struct hash_map
{
    ht_cl *data;
    size_t arr_size;
    size_t size;
    size_t limit;
} hm_cl;

unsigned int hash(int info, int key);
hm_cl *new_hm_cl(size_t size);
ht_cl *new_ht_cl(size_t size);
hm_cl *resize_ht_cl(hm_cl *table);
hm_cl *add_ht_cl(hm_cl *table, int info);
int search_ht_cl(hm_cl *table, int info);
hm_cl *delete_ht_cl(hm_cl *table, int info);
void print_ht_cl(const hm_cl *table);
hm_cl *load_ht_cl_from_file(FILE *file);
void free_ht_cl(hm_cl *table);
hm_cl *restruct_ht_cl(hm_cl *table);

#endif
