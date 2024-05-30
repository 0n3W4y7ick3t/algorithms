/**
 * @author leon
 * @date Sep 13, 2023 at 18:20:45
 */
#include "printbtree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int height(node* tree) {
  if (!tree)
    return 0;
  return max(height(tree->left), height(tree->right)) + 1;
}

int width(int height) { return pow(2.0, height + 1e-10) - 1; }

int* pof2DArr(int* M, int i, int j) { return &M[i * j + j]; }

int initPrint(int* M, node* n, int row, int col, int height) {
  if (!n)
    return 0;
  *pof2DArr(M, row, col) = n->data;
  /* the smaller the subtree(height) is, the narrower the (col) gaps are. */
  initPrint(M, n->left, row + 1, col - (int)pow(2.0, height - 2.0), height - 1);
  initPrint(M, n->right, row + 1, col + (int)pow(2.0, height - 2.0),
            height - 1);
  return 0;
}

/* this is the func to print the binary tree */
int printTree(node* tree) {
  int h = height(tree);
  int w = width(h);

  /* the matrix to be print out */
  int* M = malloc((h * w) * sizeof(int));
  if (!M) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  memset(M, 0, h * w);
  initPrint(M, tree, 0, w / 2, h);

  int i, j;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      if (*pof2DArr(M, i, j) == 0) {
        printf("     ");
      } else
        printf("%5d", *pof2DArr(M, i, j));
    }
    printf("\n");
  }
  free(M);
  return 0;
}
