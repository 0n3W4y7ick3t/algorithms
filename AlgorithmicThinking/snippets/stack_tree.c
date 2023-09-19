#include<stdlib.h>
#include<stdio.h>
#include"stack_tree.h"


int is_empty(tree_stack *stack){
  return stack->top == -1;
}
int max(int n1, int n2){
  return n1 > n2 ? n1:n2;
}

tree_stack* new_stack(int capacity){
  tree_stack *stack = malloc(sizeof(tree_stack));
  if (stack == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  stack->top = -1;
  node **nodes = malloc(sizeof(node*) * capacity);
  if (nodes == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  stack->nodes = nodes;
  stack->capacity = capacity;
  return stack;
}


int push(tree_stack *stack, node* n){
  if (stack->top == stack->capacity - 1){
    /* double the capacity */
    stack->nodes = realloc(stack->nodes, stack->capacity * 2);
    stack->capacity *= 2;
  }
  stack->nodes[++stack->top] = n;
  return stack->top;
}

node* pop(tree_stack *stack) {
  if(is_empty(stack)){
    fprintf(stderr, "trying to pop an empty stack!\n");
    return NULL;
  }
  return stack->nodes[stack->top--];
}

node* new_node(int data, node *left, node *right) {
  node *n = malloc(sizeof(node));
  if (n == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  n->data = data;
  n->left = left;
  n->right = right;
  return n;
}

int leaves_weight(node *tree){
  // sum up the data of all leaf nodes
  if(tree->left == NULL && tree->right == NULL)
    return tree->data;
  return leaves_weight(tree->left) + leaves_weight(tree->right);
}

int edges(node *tree) {
  if(tree->left == NULL && tree->right == NULL)
    return 0;
  if(tree->left == NULL && tree->right != NULL)
    return 1 + edges(tree->right);
  if(tree->left != NULL && tree->right == NULL)
    return 1 + edges(tree->left);
  return 2 + edges(tree->left) + edges(tree->right);
}

int height(node *tree) {
  if (tree == NULL)
    return 0;
  return max(height(tree->left), height(tree->right)) + 1;
}

