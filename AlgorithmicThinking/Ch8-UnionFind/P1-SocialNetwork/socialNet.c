/**
 * @author leon
 * @date Jun 07, 2024 at 21:24:20
 * @tag union find
 * @problem SOCNETC - Social Network Community
 * @link https://www.spoj.com/problems/SOCNETC/
 * @result AC
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

size_t find(size_t* parents, size_t x) {
  size_t root = x;
  while (parents[root] != root)
    root = parents[root];

  // path compression
  // update parents in this path to root
  size_t temp = x;
  while (parents[x] != root) {
    temp = parents[x];
    parents[x] = root;
    x = temp;
  }
  return root;
}

void add(size_t node_n, size_t* parents, size_t* sizes, size_t x, size_t y,
         size_t limit) {
  size_t root_x = find(parents, x);
  size_t root_y = find(parents, y);
  if (root_x == root_y || sizes[root_x] + sizes[root_y] > limit)
    return;
  if (sizes[root_x] > sizes[root_y]) {
    parents[root_y] = root_x;
    sizes[root_x] += sizes[root_y];
  } else {
    parents[root_x] = root_y;
    sizes[root_y] += sizes[root_x];
  }
}

int main(void) {
  size_t node_n, size_limit, ops_n;
  scanf("%zu%zu%zu", &node_n, &size_limit, &ops_n);
  // starts with 1
  size_t* parents = malloc(sizeof(size_t[node_n + 1]));
  size_t* sizes = malloc(sizeof(size_t[node_n + 1]));
  for (size_t i = 1; i <= node_n; ++i) {
    sizes[i] = 1;
    parents[i] = i;
  }
  size_t x, y;
  char op;
  for (size_t i = 0; i < ops_n; ++i) {
    scanf(" %c", &op);
    switch (op) {
    case 'S':
      scanf("%zd", &x);
      printf("%zu\n", sizes[find(parents, x)]);
      break;
    case 'A':
      scanf("%zu%zu", &x, &y);
      add(node_n, parents, sizes, x, y, size_limit);
      break;
    case 'E':
      scanf("%zu%zu", &x, &y);
      if (find(parents, x) == find(parents, y))
        printf("Yes\n");
      else
        printf("No\n");
      break;
    }
  }
  return EXIT_SUCCESS;
}
