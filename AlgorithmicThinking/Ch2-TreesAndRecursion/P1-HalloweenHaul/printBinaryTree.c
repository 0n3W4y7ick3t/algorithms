/*
 * @author leon
 * @created Sep 13, 2023 at 18:20:45
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct node {
  int data;
  struct node *left, *right;
} node;

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

int max(int n1, int n2){
  return n1 > n2 ? n1:n2;
}

int height(node *tree) {
  if (tree == NULL)
    return 0;
  return max(height(tree->left), height(tree->right)) + 1;
}

int width(int height) {
  return height * (height + 1) / 2;
}


int initPrint(int *M, node *n, int row, int col, int height) {
  if(n == NULL)
    pof2DArr(M,)
  return 0;
}

int* pof2DArr(int *M, int i, int j){
  return &M[(i-1)*j + j];
}

/* this is the func to print the binary tree */
int printTree(node *tree) {
  int h = height(tree);
  int w = width(h);

  int *M = malloc((h*w) * sizeof(int)); /* the matrix to be print out */
  if (M == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  initPrint(M, tree, 0, w/2, h);

  int i, j;
  for (i=0; i < h; i++) {
    for (j=0; j<w; j++) {
      if(*pof2DArr(M, i, j) == 0){
        printf(" ");
      }
      else
        printf("%d", *pof2DArr(M, i, j));
    }
    printf("\n");
  }
  return 0;
}

int main(void) {
  // mock tree
  node *n7 = new_node(7, NULL, NULL);
  node *n6 = new_node(6, n6, NULL);
  node *n5 = new_node(5, NULL, NULL);
  node *n4 = new_node(4, NULL, NULL);
  node *n3 = new_node(3, n6, NULL);
  node *n2 = new_node(1, n4, n5);
  node *n1 = new_node(1, n2, n3);



  return 0;
}

