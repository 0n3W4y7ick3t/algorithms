/*
 * @author leon
 * @created Sep 13, 2023 at 18:20:45
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int data;
  struct node *left, *right;
} node;

node *new_node(int data, node *left, node *right) {
  node *n = new node;
  n->data = data;
  n->left = left;
  n->right = right;
  return n;
}

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int height(node *tree) {
  if (!tree)
    return 0;
  return max(height(tree->left), height(tree->right)) + 1;
}

int width(int height) { return pow(2.0, height + 1e-10) - 1; }

int initPrint(int **M, node *n, int row, int col, int height) {
  if (!n)
    return 0;
  printf("set (%d, %d) as %d\n", row, col, n->data);
  M[row][col] = n->data;
  /* the smaller the subtree(height) is, the narrower the (col) gaps are. */
  initPrint(M, n->left, row + 1, col - (int)pow(2.0, height - 2.0), height - 1);
  initPrint(M, n->right, row + 1, col + (int)pow(2.0, height - 2.0),
            height - 1);
  return 0;
}

/* this is the func to print the binary tree */
int printTree(node *tree) {
  int h = height(tree);
  int w = width(h);

  /* the matrix to be print out */
  int **M = new int *[h];
  for (int z = 0; z < h; z++)
    M[z] = new int[w];
  initPrint(M, tree, 0, w / 2, h);

  int i, j;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      if (M[i][j] == 0) {
        printf("     ");
      } else
        printf("%5d", M[i][j]);
    }
    printf("\n");
  }
  return 0;
}

int main(void) {
  // mock tree
  node *n15 = new_node(15, 0, 0);
  node *n14 = new_node(14, 0, 0);
  node *n13 = new_node(13, 0, 0);
  node *n12 = new_node(12, 0, 0);
  node *n11 = new_node(11, 0, 0);
  node *n10 = new_node(10, 0, 0);
  node *n9 = new_node(9, 0, 0);
  node *n8 = new_node(8, 0, 0);
  node *n7 = new_node(7, n14, n15);
  node *n6 = new_node(6, n12, n13);
  node *n5 = new_node(5, n10, n11);
  node *n4 = new_node(4, n8, n9);
  node *n3 = new_node(3, n6, n7);
  node *n2 = new_node(2, n4, n5);
  node *n1 = new_node(1, n2, n3);

  printTree(n1);
  return 0;
}
