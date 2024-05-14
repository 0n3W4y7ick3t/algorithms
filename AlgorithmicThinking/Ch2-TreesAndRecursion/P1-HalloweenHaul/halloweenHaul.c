/**
 * @author leon
 * @date Sep 14, 2023 at 13:28:53
 * @link https://dmoj.ca/problem/dwite12c1p4
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEST_CASES 5
#define LINE_SIZE 256

typedef struct node {
  int data;
  struct node *left, *right;
} node;

node* new_node(int data, node* left, node* right) {
  node* n = malloc(sizeof(node));
  if (!n) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  n->data = data;
  n->left = left;
  n->right = right;
  return n;
}

int leaves_weight(node* tree) {
  // sum up the data of all leaf nodes
  if (!tree->left && !tree->right)
    return tree->data;
  return leaves_weight(tree->left) + leaves_weight(tree->right);
}

int edges(node* tree) {
  if (!tree->left && !tree->right)
    return 0;
  if (!tree->left && tree->right)
    return 1 + edges(tree->right);
  if (tree->left && !tree->right)
    return 1 + edges(tree->left);
  return 2 + edges(tree->left) + edges(tree->right);
}

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int height(node* tree) {
  if (!tree || (!tree->left && !tree->right))
    return 0;
  if (tree->left && !tree->right)
    return 1 + height(tree->left);
  if (tree->right && !tree->left)
    return 1 + height(tree->right);
  return max(height(tree->left), height(tree->right)) + 1;
}

node* read_tree_helper(char* line, int* pos) {
  node* tree = malloc(sizeof(node));
  if (!tree) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }

  if (line[*pos] == '(') {
    /* start of a tree */
    (*pos)++; /* skip begin ( */
    tree->left = read_tree_helper(line, pos);
    (*pos)++; /* skip space */
    tree->right = read_tree_helper(line, pos);
    (*pos)++; /* skip end ) */
    return tree;
  } else {
    /* reads a number, ie this is a house */
    tree->left = NULL;
    tree->right = NULL;
    tree->data = line[*pos] - '0';
    (*pos)++;
    while (line[*pos] != ' ' && line[*pos] != ')' && line[*pos] != '\0') {
      /* the next char is also a number, should continue reading */
      tree->data = tree->data * 10 + line[*pos] - '0';
      (*pos)++;
    }
    return tree;
  }
}

node* read_tree(char* line) {
  int pos = 0;
  return read_tree_helper(line, &pos);
}

char* read_line(int size) {
  char* str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (!str) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != '\n')) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (!str) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

void solve(node* tree) {
  int longest = height(tree);
  int candy = leaves_weight(tree);
  int streets = 2 * edges(tree);
  printf("%d %d\n", streets - longest, candy);
}

int main(void) {
  int i;
  char* line;
  node* tree;
  for (i = 0; i < TEST_CASES; i++) {
    line = read_line(LINE_SIZE);
    tree = read_tree(line);
    solve(tree);
  }
  return 0;
}
