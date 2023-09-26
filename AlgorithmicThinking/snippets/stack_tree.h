#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int data;
  struct node *left, *right;
} node;

typedef struct tree_stack {
  int top;
  int capacity;
  node **nodes;
} tree_stack;

int is_empty(tree_stack *);

int max(int, int);

tree_stack *new_stack(int);

int push(tree_stack *, node *);

node *pop(tree_stack *);

node *new_node(int, node *, node *);

int leaves_weight(node *);

int edges(node *);

int height(node *);
