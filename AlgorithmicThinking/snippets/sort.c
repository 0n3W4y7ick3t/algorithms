#include<string.h>

typedef struct node {
  char *name;
  int score;
} node;

typedef int (*Comparator)(node **, int, int);

int compare(node *nodes[], int i, int j) {
  if (nodes[i]->score == nodes[j]->score)
    return strcmp(nodes[i]->name, nodes[j]->name) > 0;
  return nodes[i]->score < nodes[j]->score;
}

// sort nodes in place from index i to j using cmp()
// void sort(node *nodes[], int i, int j, int(*cmp)(node**, int, int))
void sort(node *nodes[], int i, int j, Comparator cmp) {
  int a, b;
  node *temp;
  for (a = i; a < j; a++)
    for (b = a + 1; b < j; b++)
      if (cmp(nodes, a, b)) {
        temp = nodes[a];
        nodes[a] = nodes[b];
        nodes[b] = temp;
      }
}
