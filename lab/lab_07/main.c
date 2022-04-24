#include <stdio.h>
#include <limits.h>
#include "main.h"
#include "messages.h"
#include "table.h"
#include "funcs.h"
#include "graph.h"

#include "messages.c"
#include "table.c"
#include "funcs.c"
#include "graph.c"

int main(void)
{
    int rc;
    int size;
    int from, to;
    int *way;
    struct table_t *union_table;
    struct table_t *table;
    uint64_t start, end;

    greeting();

    printf("Input amount of vertexes: ");
    rc = check_number(&size, 1, INT_MAX);
    if (rc != OK)
    {
        printf("Invalid value\n");
        return rc;
    }
    table = create_table(size, 2);

    printf("Input connections in graph (format \"from to length duty\"). ");
    printf("To finish write \"-1\" (index from 0):\n");
    rc = fill_table(table);
    if (rc != OK)
    {
        printf("Invalid value\n");
        free_table(table);
        return rc;
    }
    printf("Input vertex 'from' and vertex 'to': ");
    if (check_number(&from, 0, table->size) || check_number(&to, 0, table->size))
    {
        printf("Invalid vertex\n");
        free_table(table);
        return READ_ERROR;
    }
    int m = 0;
    int amount = 0;
    union_table = copy_table(table);
    start = tick();
    way = deicstra(union_table, from, to, &m, &amount);
    end = tick();
    if (way)
    {
        printf("Minimum way = %d\n", m);
        for (int i = amount - 1; i > 0; i--)
            printf("%d -> ", way[i]);
        printf("%d\n", way[0]);
        // free(way);
    }
    else
        printf("I can\'t find minimum way from %d to %d\n", from, to);

    printf("Time of working algorithm: %ju\n", end - start);
    printf("Memory for table: %ld\n\n\n",
           table->size * table->size * 3 * sizeof(int) + sizeof(struct table_t) * 2);
    export_to_dot(*table, way);
    free_table(table);
    free_table(union_table);
    free(way);
    return OK;
}
/*
0 1 1 1
1 2 2 1
2 3 11 10
3 5 8 2
3 4 4 1
4 0 3 1
0 2 4 1
4 2 10 1
-1
*/
