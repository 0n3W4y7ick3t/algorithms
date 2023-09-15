/**
 * @author leon
 * @created Sep 14, 2023 at 13:28:53
 * @link https://dmoj.ca/problem/dwite12c1p4
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"stack_tree.h"
#define SIZE 256

char *read_line(int size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (str == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while( (ch = getchar()) != EOF && (ch != '\n') ) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if(len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (str == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

void node_ops(node* n){
  printf("%d\n", n->data);
}

void stack_traverse_tree(node* tree) {
  tree_stack* stack = new_stack(SIZE);

  while(tree) {
    node_ops(tree);
    if (!tree->right && !tree->left){
      if(is_empty(stack))
        tree = NULL;
      else
        tree = pop(stack);
    }else if (!tree->left && tree->right) {
      tree = tree->right;
    }else if (tree->left && !tree->right) {
      tree = tree->left;
    }else{
      /* both left and right child exit */
      push(stack, tree->right);
      tree = tree->left;
    }
  }
  free(stack->nodes);
  free(stack);
}

void recursion_traverse_tree(node *tree){
  if(!tree)
    return;
  node_ops(tree);
  recursion_traverse_tree(tree->left);
  recursion_traverse_tree(tree->right);
}

int main(void) {
  // mock tree
  node *n15 = new_node(15, NULL, NULL);
  node *n14 = new_node(14, NULL, NULL);
  node *n13 = new_node(13, NULL, NULL);
  node *n12 = new_node(12, NULL, NULL);
  node *n11 = new_node(11, NULL, NULL);
  node *n10 = new_node(10, NULL, NULL);
  node *n9 = new_node(9, NULL, NULL);
  node *n8 = new_node(8, NULL, NULL);
  node *n7 = new_node(7, n14, n15);
  node *n6 = new_node(6, n12, n13);
  node *n5 = new_node(5, n10, n11);
  node *n4 = new_node(4, n8, n9);
  node *n3 = new_node(3, n6, n7);
  node *n2 = new_node(2, n4, n5);
  node *n1 = new_node(1, n2, n3);

  stack_traverse_tree(n1);
  printf("#########\n");
  recursion_traverse_tree(n1);

  int i;
  node *nn = new_node(10,NULL,NULL);
  node *n = new_node(10,nn,NULL);
  node *np;
  for(i=0; i<100000; i++){
    np = new_node(10, NULL, NULL);
    nn->left = np;
    nn = np;
  }

  recursion_traverse_tree(n);

  printf("all leaves_weight sum up to: %d\n", leaves_weight(n1));
  printf("shortest path: %d\n", 2 * edges(n1) - height(n1));
  return 0;
}

