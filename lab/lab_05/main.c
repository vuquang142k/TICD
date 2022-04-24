#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "io.h"
#include "structures.h"
#include "list.h"
#include "q_array.h"

#include "io.c"
#include "structures.c"
#include "list.c"
#include "q_array.c"



int main(){
    setbuf(stdout, NULL);
    srand(time(NULL));
    int n = 1000;
    int interval = 100;
    int log = 0;
    time_range T1 = {1, 5};
    time_range T2 = {0, 3};
    time_range T3 = {0, 4};
    time_range T4 = {0, 1};

    int flag = -1;
    while (flag){
        printf("\nChoose option:\n"
               "\t1 - Change values\n"
               "\t2 - Print values\n"
               "\t3 - Array queue\n"
               "\t4 - List queue\n"
               "\t5 - Time\n"
               "\t0 - Exit\n");

        fflush(stdin);
        if (scanf("%d", &flag) != 1){
            printf("Wrong input!\n");
            continue;
        }

        switch (flag){
            case 0:
                break;
            case 1:
                printf("Change the processing time.\n");
                double l, r;
                int c;
                printf("1: min = %lf; max = %lf\n", T1.min, T1.max);
                printf("2: min = %lf; max = %lf\n", T2.min, T2.max);
                printf("3: min = %lf; max = %lf\n", T3.min, T3.max);
                printf("4: min = %lf; max = %lf\n", T4.min, T4.max);
                printf("What interval to change?\n");
                if (check_number(&c, 1, 4) != OK)
                {
                    fflush(stdin);
                    printf("Invalid number input!\nPlease choose some command\n");
                    break;
                }
                else
                {
                    printf("Input left and right borders: ");
                    int rc = scanf("%lf", &l);
                    if (rc != 1)
                    {
                        printf("Invalid number input!\nPlease choose some command\n");
                        break;
                    }
                    rc = scanf("%lf", &r);
                    if (rc != 1)
                    {
                        printf("Invalid number input!\nPlease choose some command\n");
                        break;
                    }
                    if (c == 1)
                    {
                        T1.min = l;
                        T1.max = r;
                    }
                    if (c == 2)
                    {
                        T2.min = l;
                        T2.max = r;
                    }
                    if (c == 3)
                    {
                        T3.min = l;
                        T3.max = r;
                    }
                    if (c == 4)
                    {
                        T4.min = l;
                        T4.max = r;
                    }
                    printf("AFTER\n");
                    printf("1: min = %lf; max = %lf\n", T1.min, T1.max);
                    printf("2: min = %lf; max = %lf\n", T2.min, T2.max);
                    printf("3: min = %lf; max = %lf\n", T3.min, T3.max);
                    printf("4: min = %lf; max = %lf\n", T4.min, T4.max);
                }
                break;
            case 2:
                print_values(n, interval, log, &T1, &T2, &T3, &T4);
                break;
            case 3:
                go_array(n, interval, T1, T2, T3, T4, log);
                break;
            case 4:
                go_list(n, interval, T1, T2, T3, T4, log);
                break;
            case 5:;
                char *q1;
                discriptor d1;

                q1 = (char*)calloc(MAX_LEN, sizeof(char));

                init_discriptor_arr(&d1, q1);

                /*node* freed_memory[MAX_LEN];
                int cnt_freed = 0;
                node* used_memory[MAX_LEN];
                int count_used = 0;
                int cnt_reused = 0;*/

                node *q2 = NULL;
                discriptor d2;
                
                init_discriptor(&d2);

                int64_t t1b = tick();
                arr_push(&d1, q1, '1');
                int64_t t1e = tick();

                int64_t x = 0, t2b, t2e;
                for (int i = 1; i <= 10; i++){
                    node *item = NULL;
                    item = create_node('2');
                    t2b = tick();
                    q2 = add_node(q2, item);
                    t2e = tick();
                    x += (t2e - t2b);
                }


                printf("ДОБАВЛЕНИЕ\n");
                printf("Очередь-массив \t %d\n", (int)(t1e - t1b)/400);
                printf("Очередь-список \t %d\n\n", (int)x / 10);

                t1b = tick();
                arr_pop(&d1, q1);
                t1e = tick();
                t2b = tick();
                node* ret = list_pop(&q2);
                t2e = tick();

                printf("УДАЛЕНИЕ\n");
                printf("Очередь-массив \t %d\n", (int)(t1e - t1b));
                printf("Очередь-список \t %d\n\n", (int)(t2e - t2b));

                int n = 10;
                printf("ПАМЯТЬ\n");
                for (int i = 0; i < 4; i++)
                {
                    printf("Колчество элементов = %d\n", n);
                    printf("Очередь-массив \t %d\n", (int)(sizeof(char) * n));
                    printf("Очередь-список \t %d\n\n", (int)((sizeof(node*) + sizeof(char)) * n));
                    n *= 10;
                }
                free(ret);
                free(q1);
                free(q2);
                break;
            default:
                printf("ERROR input.");
                break;
        }
    }
    return 0;
}
