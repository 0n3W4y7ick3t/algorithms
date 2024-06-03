/**
 * @author leon
 * @date Jun 03, 2024 at 20:25:53
 * @tag Graph, Dijkstra
 * @problem South African Computer Olympiad 2008 Day 1 Problem 3 - Visiting
 * Grandma
 * @link https://dmoj.ca/problem/saco08p3
 * @result WA
 */

/* Remodeling:
  Think of having visited cookie shop or not to be a state, which can be 0 or 1,
  imagine there is another graph, with the exactly same edges as the original
  one, it connects with the original one by edges weight of zero, at each shop,
  which are one way edges, from the state0 graph to the state1 graph, that is, a
  multi-D graph in total! It's (two) layers overlap with each other, and connect
  at the shops.

  Then the question becomes:
  starting from node[0][0], find the shortest path and number of ways to get to
  node[m-1][1].
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* INDEX_OF(n, x, y) gets the index from an array mapping
  a upper triangle nxn matrix at the position of (x, y)
  (n starts from 1, x, y start from 0)

  length of the array is:
      ((n-1) + 1)*(n-1)/2 = 0.5n^2 - 0.5n,  n >= 2

  index of (x, y) is:
      0.5(n-1 + n-1-x)*(x+1) - (n-y)
        = (n-1)(x+1) - 0.5x(x+1) - n + y
        = nx + y - 1 - 0.5x(x+3),           0 <= x < y < n;

eg(n=6):
0 1 1 1 1 1
x 0 2 2 ? 2
x x 0 3 3 3
x x x 0 4 4
x x x x 0 5
x x x x x 0

will be stored as
1 1 1 1 1 2 2 ? 2 3 3 3 4 4 5

?(1, 4) will be at index 7
*/
#define MATRIX_LEN(n) ((n) > 1 ? ((n) * (n) - (n)) >> 1 : 0)
#define INDEX_OF_HELPER(n, x, y)                                               \
  (((x) > (n) || (y) > (n)) ? -1                                               \
   : ((n) <= 1)             ? -1                                               \
                            : (n) * (x) + (y)-1 - ((((x) + 3) * (x)) >> 1))

#define INDEX_OF(n, x, y)                                                      \
  ((x) > (y)) ? INDEX_OF_HELPER(n, y, x) : INDEX_OF_HELPER(n, x, y)

#define MOD 1000000

/* read the upper matrix and store it into an array */
void read_matrix(size_t n, size_t matrix[MATRIX_LEN(n)]) {
  size_t elem;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j) {
      scanf("%zu", &elem);
      if (i >= j) /* skip the bottom part */
        continue;
      else
        matrix[INDEX_OF(n, i, j)] = elem;
    }
}

/* cost to arrive node at pos */
typedef struct Cost {
  size_t cost;
  size_t num; /* number of routes with min cost */
} Cost;

void dijkstra(size_t n_node, size_t graph[MATRIX_LEN(n_node)], size_t n_shop,
              size_t shop_pos[n_shop], size_t start, size_t end) {
  bool done[2][n_node];
  memset(done, 0, sizeof(done));

  bool shops[n_node]; // shops[i]: node i is shop?
  memset(shops, 0, sizeof(shops));
  // shop pos start at 1
  for (size_t i = 0; i < n_shop; ++i)
    shops[shop_pos[i]-1] = true;

  Cost* costs = malloc(sizeof(Cost[2 * n_node]));
  for (size_t i = 0; i < 2 * n_node; ++i)
    costs[i] = (Cost){.cost = -1, .num = 0};

  costs[start].cost = 0;
  costs[start].num = 1;

  bool found;
  /* the new found node's positoin, layer(state) */
  size_t min_pos, min_layer, min_cost, new_cost;
  while (true) {
    found = false;
    min_cost = -1;

    for (size_t l = 0; l < 2; ++l)
      /* search for all not done node in two layers */
      for (size_t i = 0; i < n_node; ++i) {
        if (!done[l][i] && costs[l * n_node + i].cost < min_cost) {
          min_cost = costs[l * n_node + i].cost;
          min_pos = i;
          min_layer = l; /* also note which layer this node at */
          found = true;
        }
      }

    if (!found)
      break;

    // printf("\nset node %zu at layer %zu to done, cost %zu, num %zu\n",
    //        min_pos, min_layer, min_cost, costs[min_layer * n_node + min_pos].num);
    done[min_layer][min_pos] = true;
    // extra edges(at shop, state0):
    if (shops[min_pos] && min_layer == 0) {
      // try to update the layer 1 with layer 0
      // printf("[shop] probing the layer 1 from shop %zu\n", min_pos);
      if (costs[n_node + min_pos].cost > min_cost) {
        // printf("update node %zu at layer 1, ", min_pos);
        // printf("set cost to %zu, ", min_cost);
        // printf("set num to %zu\n", costs[min_pos].num);
        costs[n_node + min_pos].cost = min_cost;
        costs[n_node + min_pos].num = costs[min_pos].num;
      } else if (costs[n_node + min_pos].cost == min_cost) {
        // printf("update node %zu at layer 1, ", min_pos);
        // printf("increase num by %zu\n", costs[min_pos].num);
        costs[n_node + min_pos].num =
            (costs[n_node + min_pos].num + costs[min_pos].num) % MOD;
      }
    }
    /* update costs for positions from [min_layer][min_pos] */
    // printf("probing the same layer %zu from node %zu\n", min_layer, min_pos);
    for (size_t i = 0; i < n_node; ++i) {
      if (i != min_pos) {
        // same layer, the graph is same
        new_cost = min_cost + graph[INDEX_OF(n_node, min_pos, i)];
        if (costs[min_layer * n_node + i].cost > new_cost) {
          // printf("update node %zu at layer %zu, ", i, min_layer);
          // printf("set cost to %zu, ", new_cost);
          // printf("set num to %zu\n", costs[min_layer * n_node + min_pos].num);
          costs[min_layer * n_node + i].cost = new_cost;
          costs[min_layer * n_node + i].num =
              costs[min_layer * n_node + min_pos].num;
        } else if (costs[min_layer * n_node + i].cost == new_cost) {
          // printf("update node %zu at layer %zu, ", i, min_layer);
          // printf("increase num by %zu\n", costs[min_layer * n_node + min_pos].num);
          costs[min_layer * n_node + i].num =
              (costs[min_layer * n_node + i].num + costs[min_layer * n_node + min_pos].num) % MOD;
        }
      }
    }
    // for (size_t i = 0; i < 2 * n_node; ++i) {
    //   printf("** result ** node #%zu at layer %zu cost %zu num %zu\n",
    //          i % n_node, i / n_node, costs[i].cost, costs[i].num);
    // }
  }
  printf("%zu %zu\n", costs[n_node + end].cost,
         costs[n_node + end].num % MOD); /* the results */
  free(costs);
}

int main(void) {
  size_t n_node, n_shop;
  scanf("%zu", &n_node);
  size_t* graph = malloc(sizeof(size_t[MATRIX_LEN(n_node)]));
  read_matrix(n_node, graph);
  scanf("%zu", &n_shop);
  size_t* shops_pos = malloc(sizeof(size_t[n_shop]));
  for (size_t i = 0; i < n_shop; ++i)
    /* this starts with 1 */
    scanf("%zu", &shops_pos[i]);

  dijkstra(n_node, graph, n_shop, shops_pos, 0, n_node - 1);
  free(shops_pos);
  free(graph);
  return EXIT_SUCCESS;
}
