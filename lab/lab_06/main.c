#include "define.h"
#include "tree.h"
#include "hash_table.h"

/*#include "tree.c"
#include "hash_table.c"*/

int search_in_file(FILE *file_in, int num)
{
    int search;
    int counts_made = 0, flag = 0;

    while (fscanf(file_in, "%d", &search) > 0)
    {
        counts_made += 1;

        if (search == num)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 1)
    {
        return counts_made;
    }
    else
    {
        return 0;
    }
}

void make_graph(struct tree_node *binary_tree, struct tree_node *parent, FILE *graph)
{
	if (binary_tree)
	{
		if (parent != NULL)
			fprintf(graph, "%d->%d\n", parent->info, binary_tree->info);
		make_graph(binary_tree->left, binary_tree, graph);
		make_graph(binary_tree->right, binary_tree, graph);
	}
}

int main(void)
{
    FILE *file_in = fopen("input.txt", "r");
    //FILE *graph;
    tree_node *tree = NULL;
    tree_node *binary_tree = NULL;
    hm_cl *table = NULL;
    unsigned long long t1, t2;
    int count = 0, choice = -1;

    do
    {
        printf("\n%sУсловие:\n", GREEN);
        printf("В текстовом файле содержатся целые числа. Построить ДДП из чисел\n");
        printf("файла. Вывести его на экран в виде дерева. Сбалансировать полученное\n");
        printf("дерево и вывести его на экран. Построить хеш-таблицу из чисел файла.\n");
        printf("Использовать закрытое хеширование для устранения коллизий. Осуществить поиск\n");
        printf("введенного целого числа в ДДП, в сбалансированном дереве, в хеш-таблице\n");
        printf("и в файле. Сравнить время поиска, объем памяти и количество сравнений\n");
        printf("при использовании различных (4-х) структур данных. Если количество\n");
        printf("сравнений в хеш-таблице больше указанного, то произвести\n");
        printf("реструктуризацию таблицы, выбрав другую функцию.%s\n", RESET);
        printf("\n%sМеню:\n", CYAN);
        printf("[1] - загрузить информацию из файла\n");
        printf("[2] - вывести на экран деревья и хеш-таблицы\n");
        //printf("[3] - добавить элемент\n");
        //printf("[4] - удалить элемент\n");
        printf("[3] - найти элемент и провести сравнение в различных структурах\n");
        printf("[4] - подсчитать среднее количество сравнений для поиска\n");
        //printf("[6] - печать текущего дерева в формате .png (бета-версия)\n");
        printf("[0] - выйти из программы%s\n", RESET);
        printf("\nВыберите пункт меню: ");
        scanf("%d", &choice);
        fflush(stdin);
        getchar();

        switch (choice)
        {
            case 1:
                free_tree(tree);
                tree = read_tree_from_file(file_in);

                if (tree)
                {
                    printf("%sСбалансированное дерево создано!%s\n", GREEN, RESET);
                }

                binary_tree =  read_binary_tree_from_file(file_in, &count);

                if (binary_tree)
                {
                    printf("%sБинарное дерево поиска создано!%s\n", GREEN, RESET);
                }

                free_ht_cl(table);
                table = load_ht_cl_from_file(file_in);

                if (table)
                {
                    printf("%sХеш-таблица с закрытой адресацией создана!%s\n", GREEN, RESET);
                }

                break;
            case 2:
                if (binary_tree)
                {
                    printf("%sБинарное дерево поиска\n%s", YELLOW, RESET);
                    print_tree(binary_tree, 0, 0, 0);
                }

                if (tree)
                {
                    printf("%sСбалансированное бинарное дерево\n%s", YELLOW, RESET);
                    print_tree(tree, 0, 0, 1);
                }

                if (table)
                {
                    printf("%sХеш-таблица с закрытой адресацией%s\n", YELLOW, RESET);
                    print_ht_cl(table);
                }

                break;
            /*case 3:
                printf("Введите число: ");
                int num = 0;
                scanf("%d", &num);

                tree_node *tmp = create_node(num);
                tree = insert(tree, tmp);

                tmp = create_node(num);
                binary_tree = binary_tree_insert(binary_tree, tmp, &count);

                if (!table)
                {
                    table = new_hm_cl(START_SIZE);
                }
                table = add_ht_cl(table, num);

                printf("%sГотово!%s\n", GREEN, RESET);
                break;
            case 4:
                printf("Введите число: ");
                num = 0;
                scanf("%d", &num);

                if (tree)
                {
                    tree = remove_tree(tree, num);
                }

                if (binary_tree)
                {
                    binary_tree = binary_tree_remove(binary_tree, num);
                }

                if (table)
                {
                    table = delete_ht_cl(table, num);
                }
                else
                {
                    puts("Не удалось удалить!");
                }

                printf("%sГотово!%s\n", GREEN, RESET);
                break;*/
            case 3:
                printf("Введите число: ");
                int num = 0;
                scanf("%d", &num);
                

                zero_count_made();
                printf("%s       Структура        | Время(тик) | Сравнений | Память %s\n", GREEN, RESET);
                t1 = tick();
                tree_node *find_1 = find_node(binary_tree, num);
                t2 = tick();

                if (find_1 == NULL)
                {
                    printf("%sНе найдено!%s\n", RED, RESET);
                }
                else
                {
                    printf(" Дерево поиска          | %10lld |", t2 - t1);
                    show_count();
                    printf("%6ld\n", sizeof(tree_node) * table->size);
                }

                zero_count_made();
                t1 = tick();
                tree_node *find_2 = find_node(tree, num);
                t2 = tick();

                if (find_2 == NULL)
                    printf("%sНе найдено!%s\n", RED, RESET);
                else
                {
                    printf(" Cбалансированное дерево| %10lld |", t2 - t1);
                    show_count();
                    printf("%6ld\n", sizeof(tree_node) * table->size);
                }

                t1 = tick();
                int find_3 = search_ht_cl(table, num);
                t2 = tick();

                if (find_3 == 0)
                    printf("%sНе найдено!%s\n", RED, RESET);
                else
                    printf(" Хеш-таблица (закрытая) | %10lld |%11d|%6ld\n", t2 - t1, find_3, sizeof(hm_cl) * table->size + sizeof(ht_cl));

                rewind(file_in);
                fseek(file_in, 0, SEEK_END);
                int s = ftell(file_in);
                fseek(file_in, 0L, SEEK_SET);
                t1 = tick();
                int find_4 = search_in_file(file_in, num);
                t2 = tick();

                if (find_4 == 0)
                    printf("%sНе найдено!%s\n", RED, RESET);
                else
                    printf(" Файл                   | %10lld |%11d|%6ld\n", t2 - t1, find_4, sizeof(char) * s);

                break;
             case 4:
                 printf("%s        Структура        | Среднее время поиска | Среднее количество сравнений %s\n", GREEN, RESET);
                 int array[N];
                 int t = 0;
                 rewind(file_in);
            
                 while (fscanf(file_in, "%d", &array[t]) == 1)
                 {
                     t++;
                 }
                 rewind(file_in);
            
                 for (int i = t; i < N; i++)
                 {
                     array[i] = array[i-t];
                 }
                 // for (int i = 0; i < N; i++)
                 //     printf("%d ", array[i]);
            
                 float c = 0;
                 t1 = tick();
            
                 for (int i = 0; i < N; i++)
                 {
                     zero_count_made();
                     find_node(binary_tree, array[i]);
                     c += get_count_made();
                    //printf("\n%d ",c);
                 }
                 t2 = tick() - t1;
                 printf(" Дерево поиска           |            %lld       |        %.2f\n", 2*t2/N, c/N);
                 //printf("time %lld, count = %f", t2, c);
            
                 c = 0;
                 t1 = tick();
            
                 for (int i = 0; i < N; i++)
                 {
                     zero_count_made();
                     find_node(tree, array[i]);
                     c += get_count_made();
                    //printf("\n%d ",c);
                 }
                 t2 = tick() - t1;
                 printf(" Сбалансир.дерево        |            %lld       |        %.2f\n", 2*t2/N, c/N);
            
                 int tmp1;
                 c = 0;
                 t1 = tick();
            
                 for (int i = 0; i < N; i++)
                 {
                     tmp1 = search_ht_cl(table, array[i]);
                     c += tmp1;
                 }
                 t2 = tick() - t1;
                 printf(" Хеш-таблица (закрытая)  |            %lld       |        %.2f\n", 2*t2/N, c/N);
            
                 c = 0;
                 rewind(file_in);
                 t1 = tick();
            
                 for (int i = 0; i < N; i++)
                 {
                     search_in_file(file_in, array[i]);
                     //printf("%d %d\n", i, tmp1);
                     c += tmp1;
                 }
                 t2 = tick() - t1;
                 printf(" Файл                    |            %lld       |        %.2f\n", 2*t2/N, c/N);
                 break;
            /*case 6:
                graph = fopen("graph.gv", "w");
				fprintf(graph, "digraph G{\n");
				// fprintf(graph, "%d\n", tree->info);
                fprintf(graph, "%d\n", binary_tree->info);
				// make_graph(tree, NULL, graph);
                make_graph(binary_tree, NULL, graph);
				fprintf(graph, "}");
				fclose(graph);
				system("file.sh");
				break;*/
            case 0:
                break;
            default:
                printf("%sПункт меню выбран неверно!%s\n", RED, RESET);
				break;
        }
    } while (choice != 0);
}
