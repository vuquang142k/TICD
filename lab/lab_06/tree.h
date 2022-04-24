#ifndef TREE_H
#define TREE_H

typedef struct tree_node
{
    int info, count;
    unsigned char height;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

void print_tree(tree_node *head, int down, int lr, int flag);
unsigned long long int tick();
tree_node *create_node(int info);
tree_node *find_node(tree_node *tree, int info);
void zero_count_made(void);
void show_count(void);
int get_count_made(void);
void apply_in(tree_node *tree, void (*f)(tree_node*, void*), void *arg);
void apply_post(tree_node *tree, void (*f)(tree_node*, void*), void *arg);
void print_node(tree_node *tree, void *arg);
void apply_pre(tree_node *tree, void (*f)(tree_node*, void*), void *arg);
void free_node(tree_node *tree, void *param);
void free_tree(tree_node *tree);
tree_node *insert(tree_node *tree, tree_node *node);
tree_node *binary_tree_insert(tree_node *tree, tree_node *node, int *count);
tree_node *read_tree_from_file(FILE *file);
tree_node *read_binary_tree_from_file(FILE *file, int *count);
tree_node *find_min(tree_node *p);
tree_node *remove_min(tree_node *p);
tree_node *remove_tree(tree_node *p, int k);
tree_node *binary_tree_remove(tree_node *p, int k);
void level_apply(int *levels, tree_node *tree, int index);
int *count_levels(tree_node *tree, int *n);
void print_array(int *arr, int n);
unsigned char height(tree_node *p);
int bfactor(tree_node *p);
void fix_height(tree_node *p);
tree_node *rotate_right(tree_node *p);
tree_node *rotate_left(tree_node *q);
tree_node *balance(tree_node *p);

#endif
