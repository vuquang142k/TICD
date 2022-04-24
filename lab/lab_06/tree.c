#include "define.h"
#include "tree.h"

static int node_count = 0;
static size_t counts_made = 0;

void print_tree(tree_node *head, int down, int lr, int flag)
{
    if (head)
    {
        print_tree(head->right, down + 1, 2, flag);
        for (int i = 0; i < down; i++)
        {
            printf("         ");
        }

        switch(lr)
        {
            case 0:
                printf("➙");
                break;
            case 1:
                printf("➘");
                break;
            case 2:
                printf("➚");
                break;
        }
        if (flag)
        {
            printf(" %d(%d)\n",head->info, head->height-1);
        }
        else
        {
            printf(" %d\n", head->info);
        }

        print_tree(head->left, down + 1, 1, flag);
    }
}

unsigned long long int tick()
{
    unsigned long long int time = 0;
    __asm__ __volatile__ ("rdtsc" : "=A" (time));

    return time;
}

tree_node *create_node(int info)
{
    tree_node *node = malloc(sizeof(tree_node));

    if (node)
    {
        node->info = info;
        node->count = 1;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

tree_node *find_node(tree_node *tree, int info)
{
    counts_made += 1;

    if (tree == NULL)
    {
        return NULL;
    }

    if (info == tree->info)
    {
        return tree;
    }
    else if (info < tree->info)
    {
        return find_node(tree->left, info);
    }
    else
    {
        return find_node(tree->right, info);
    }
}

void zero_count_made(void)
{
    counts_made = 0;
}

void show_count(void)
{
    printf("%11d|", (int)counts_made);
}

int get_count_made(void)
{
    return (int)counts_made;
}

void apply_in(tree_node *tree, void (*f)(tree_node*, void*), void *arg)
{
    if (tree == NULL)
    {
        return;
    }

    apply_in(tree->left, f, arg);
    f(tree, arg);
    apply_in(tree->right, f, arg);
}

void apply_post(tree_node *tree, void (*f)(tree_node*, void*), void *arg)
{
    if (tree == NULL)
    {
        return;
    }

    apply_post(tree->left, f, arg);
    apply_post(tree->right, f, arg);
    f(tree, arg);
}

void print_node(tree_node *tree, void *arg)
{
    printf("On element: %d, adr:, count: %d\n", tree->info, tree->count);
}

void apply_pre(tree_node *tree, void (*f)(tree_node*, void*), void *arg)
{
    if (tree == NULL)
    {
        return;
    }

    f(tree, arg);
    apply_pre(tree->left, f, arg);
    apply_pre(tree->right, f, arg);
}

void free_node(tree_node *tree, void *param)
{
    if (!tree)
    {
        free(tree);
    }

    tree = NULL;
}

void free_tree(tree_node *tree)
{
    if (!tree)
    {
        apply_post(tree, free_node, NULL);
    }
}

tree_node *insert(tree_node *tree, tree_node *node)
{
    node_count += 1;

    if (tree == NULL)
    {
        return node;
    }

    if (node->info == tree->info)
    {
        tree->count++;
        free(node);
    }
    else if (node->info < tree->info)
    {
        tree->left = insert(tree->left, node);
    }
    else
    {
        tree->right = insert(tree->right, node);
    }

    return balance(tree);
}

tree_node *binary_tree_insert(tree_node *tree, tree_node *node, int *count)
{
    node_count++;

    if (tree == NULL)
    {
        return node;
    }

    (*count)++;

    if (node->info == tree->info)
    {
        tree->count++;
        free(node);
    }
    else if (node->info < tree->info)
    {
        tree->left = binary_tree_insert(tree->left, node, count);
    }
    else
    {
        tree->right = binary_tree_insert(tree->right, node, count);
    }

    return tree;
}

tree_node *read_tree_from_file(FILE *file)
{
    rewind(file);
    int tmp;
    tree_node *tree = NULL;

    while (fscanf(file, "%d", &tmp) == 1)
    {
        tree_node *cur = create_node(tmp);
        tree = insert(tree, cur);
    }

    return tree;
}

tree_node *read_binary_tree_from_file(FILE *file, int *count)
{
    rewind(file);
    int tmp;
    tree_node *tree = NULL;

    while (fscanf(file, "%d", &tmp) == 1)
    {
        tree_node *cur = create_node(tmp);
        tree = binary_tree_insert(tree, cur, count);
    }

    return tree;
}

tree_node *find_min(tree_node *p)
{
    return p->left ? find_min(p->left) : p;
}

tree_node *remove_min(tree_node *p)
{
    if (p->left == 0)
    {
        return p->right;
    }

    p->left = remove_min(p->left);
    return balance(p);
}

tree_node *remove_tree(tree_node *p, int k)
{
    if (!p)
    {
        return 0;
    }

    if (k < p->info)
    {
        p->left = remove_tree(p->left, k);
    }
    else if (k > p->info)
    {
        p->right = remove_tree(p->right, k);
    }
    else
    {
        tree_node *q = p->left;
        tree_node *r = p->right;
        free(p);

        if (!r)
        {
            return q;
        }

        tree_node *min = find_min(r);
        min->right = remove_min(r);
        min->left = q;

        return balance(min);
    }

    return balance(p);
}

tree_node *binary_tree_remove(tree_node *p, int k)
{
    if (!p)
    {
        return 0;
    }

	if (k < p->info)
    {
		p->left = remove_tree(p->left, k);
    }
	else if (k > p->info)
    {
		p->right = remove_tree(p->right, k);
    }
	else
	{
		tree_node *q = p->left;
		tree_node *r = p->right;
		free(p);

		if (!r)
        {
            return q;
        }
		tree_node *min = find_min(r);
		min->right = remove_min(r);
		min->left = q;

		return min;
	}

	return p;
}

void level_apply(int *levels, tree_node *tree, int index)
{
    if (tree != NULL)
    {
        levels[index]++;
        level_apply(levels, tree->left, index + 1);
        level_apply(levels, tree->right, index + 1);
    }
}

int *count_levels(tree_node *tree, int *n)
{
    int *levels = malloc(sizeof(int) * node_count);
    *n = node_count;

    for (int i = 0; i < *n; i++)
    {
        levels[i] = 0;
    }

    level_apply(levels, tree, 0);

    return levels;
}

void print_array(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] != 0)
        {
            printf("level %d: %d\n", i, arr[i]);
        }
    }
}

unsigned char height(tree_node *p)
{
    return p ? p->height : 0;
}

int bfactor(tree_node *p)
{
	printf("\n%d\n", height(p->right) - height(p->left));
    return (height(p->right) - height(p->left));
}

void fix_height(tree_node *p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

tree_node *rotate_right(tree_node *p)
{
    tree_node *q = p->left;
    p->left = q->right;
    q->right = p;
    fix_height(p);
    fix_height(q);

    return q;
}

tree_node *rotate_left(tree_node *q)
{
	tree_node *p = q->right;
	q->right = p->left;
	p->left = q;
	fix_height(q);
	fix_height(p);

	return p;
}

tree_node *balance(tree_node *p)
{
    fix_height(p);

    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
        {
            p->right = rotate_right(p->right);
        }

        return rotate_left(p);
    }

    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
        {
            p->left = rotate_left(p->left);
        }

        return rotate_right(p);
    }

    return p;
}
