/**
 * @author leon
 * @date Jun 07, 2024 at 20:57:09
 * @tag segment trees, treaps
 * @problem POJ problem 1785 - Binary Search Heap Construction
 * @link http://poj.org/problem?id=1785
 * @result AC
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 50000

typedef struct TreapNode {
  char* label;
  int priority;
} TreapNode;

typedef struct SegTreeNode {
  size_t left, right;
  size_t max;
} SegNode;

// read string until '/', size is the inital length
char* read_label(int size) {
  char* str;
  int ch;
  size_t len = 0;
  str = malloc(size);
  // read until '/'
  while ((ch = getchar()) != EOF && ch != '/') {
    str[len++] = ch;
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
    }
  }
  str[len] = '\0';
  return str;
}

int compare(void const* v1, void const* v2) {
  TreapNode const* n1 = v1;
  TreapNode const* n2 = v2;
  return strcmp(n1->label, n2->label);
}

// give each node the range(left, right)
void init_segtree(SegNode segtree[4 * MAX_NODES], size_t n, size_t left,
                  size_t right) {
  segtree[n].left = left;
  segtree[n].right = right;
  if (left == right)
    return;

  size_t mid = (right + left) / 2;
  init_segtree(segtree, 2 * n, left, mid);
  init_segtree(segtree, 2 * n + 1, mid + 1, right);
}

// calculate problem for each range and store the results into each
// node(recursively)
size_t build_segtree(SegNode segtree[4 * MAX_NODES], size_t n,
                     TreapNode treap[MAX_NODES + 1]) {
  if (segtree[n].left == segtree[n].right) {
    segtree[n].max = segtree[n].left;
    return segtree[n].max;
  }

  size_t left = build_segtree(segtree, 2 * n, treap);
  size_t right = build_segtree(segtree, 2 * n + 1, treap);
  if (treap[left].priority > treap[right].priority)
    segtree[n].max = left;
  else
    segtree[n].max = right;
  return segtree[n].max;
}

// query the segment tree, find the max in [left, right]
size_t range_query_max(SegNode segtree[4 * MAX_NODES], size_t root, size_t left,
                       size_t right, TreapNode treaps[MAX_NODES + 1]) {
  if (right < segtree[root].left || left > segtree[root].right)
    return -1; // no overlap

  if (left <= segtree[root].left && right >= segtree[root].right)
    return segtree[root].max; // fills all

  size_t left_max =
      range_query_max(segtree, root * 2, left, right, treaps);
  size_t right_max =
      range_query_max(segtree, root * 2 + 1, left, right, treaps);

  if (left_max == -1)
    return right_max;
  if (right_max == -1)
    return left_max;
  if (treaps[left_max].priority > treaps[right_max].priority)
    return left_max;
  else
    return right_max;
}

// print [left, right]
void solve(TreapNode treap[MAX_NODES + 1], size_t left, size_t right,
           SegNode segtree[4 * MAX_NODES]) {
  if (left > right)
    return;

  // get max from a range
  size_t root = range_query_max(segtree, 1, left, right, treap);

  printf("(");
  // left sub tree
  solve(treap, left, root - 1, segtree);
  // root
  printf("%s/%d", treap[root].label, treap[root].priority);
  // right sub tree
  solve(treap, root + 1, right, segtree);
  printf(")");
}

int main(void) {
  int nodes_n;
  static TreapNode treap[MAX_NODES + 1] = {0};
  static SegNode segtree[4 * MAX_NODES] = {0};
  while (1) {
    scanf("%d ", &nodes_n);
    if (nodes_n == 0)
      break;
    for (size_t i = 1; i <= nodes_n; ++i) {
      // construct the treap
      treap[i].label = read_label(8);
      scanf("%d ", &treap[i].priority);
    }
    // sort by label
    qsort(&treap[1], nodes_n, sizeof(TreapNode), compare);
    init_segtree(segtree, 1, 1, nodes_n);
    build_segtree(segtree, 1, treap);
    solve(treap, 1, nodes_n, segtree);
    printf("\n");
  }
  return EXIT_SUCCESS;
}
