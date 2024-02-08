/*
 * @author leon
 * @created Sep 13, 2023 at 18:20:45
 */
#ifndef PRINT_BINARY_TREE
#define PRINT_BINARY_TREE

typedef struct node {
  int data;
  struct node *left, *right;
} node;

int printTree(node *tree);

#endif /* PRINT_BINARY_TREE */

