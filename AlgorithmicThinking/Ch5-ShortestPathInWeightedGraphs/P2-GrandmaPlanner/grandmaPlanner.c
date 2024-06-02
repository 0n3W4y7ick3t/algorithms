/**
 * @author leon
 * @date Jun 01, 2024 at 10:25:53
 * @tag Graph, Dijkstra
 * @problem DMOJ problem saco08p3
 * @link https://dmoj.ca/problem/saco08p3
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
                            : (n) * (x) + (y) - 1 - ((((x) + 3) * (x)) >> 1))

#define INDEX_OF(n, x, y)                                                      \
  ((x) > (y)) ? INDEX_OF_HELPER(n, y, x) : INDEX_OF_HELPER(n, x, y)

/* read the upper matrix and store it into an array */
void read_matrix(size_t n, int matrix[MATRIX_LEN(n)]) {
  int elem;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j) {
      scanf("%d", &elem);
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

Cost* dijstra(size_t n, int matrix[MATRIX_LEN(n)], int start) {
  bool done[n];
  memset(done, 0, sizeof(done));
  Cost* all_costs = malloc(sizeof(Cost[n]));
  for (size_t i = 0; i < n; ++i)
    all_costs[i] = (Cost){.cost = -1, .num = 0};

  all_costs[start].cost = 0;
  all_costs[start].num = 1;
  bool found;
  size_t i, min_cost, min_pos, new_cost;
  while (true) {
    found = false;
    min_cost = -1;

    for (i = 0; i < n; ++i) {
      if (!done[i] && all_costs[i].cost < min_cost) {
        min_cost = all_costs[i].cost;
        min_pos = i;
        found = true;
      }
    }
    if (!found)
      break;

    done[min_pos] = true;
    /* update costs for positions from min_pos */
    for (i = 0; i < n; ++i) {
      if (i != min_pos) {
        new_cost = min_cost + matrix[INDEX_OF(n, min_pos, i)];
        if (all_costs[i].cost == -1 || all_costs[i].cost > new_cost) {
          all_costs[i].cost = new_cost;
          all_costs[i].num = 1;
        } else if (all_costs[i].cost == new_cost) {
          ++all_costs[i].num;
        }
      }
    }
  }
  return all_costs;
}

/* writes the min cost and number of ways into ans[2]
   node in matrix index starts from 0, shop pos starts from 1 */
void solve(size_t n, int matrix[MATRIX_LEN(n)],
           size_t m, int shops_pos[m], size_t ans[2]) {
  /* run dijstra twice, one from start, another from end */
  Cost* from_start = dijstra(n, matrix, 0);
  Cost* to_end = dijstra(n, matrix, n - 1);

  size_t min_cost = -1, cost, num_of_ways;
  for (int i = 0; i < m; ++i) {
    /* printf("shop %d: from start %zu ways of weight %zu\n",
           shops_pos[i], from_start[shops_pos[i] - 1].num, from_start[shops_pos[i] - 1].cost);
    printf("shop %d: to end     %zu ways of weight %zu\n",
           shops_pos[i], to_end[shops_pos[i] - 1].num, to_end[shops_pos[i] - 1].cost); */
    cost = from_start[shops_pos[i] - 1].cost + to_end[shops_pos[i] - 1].cost;
    if (min_cost > cost) {
      min_cost = cost;
      num_of_ways =
          from_start[shops_pos[i] - 1].num * to_end[shops_pos[i] - 1].num;
    } else if (min_cost == cost) {
      num_of_ways +=
          from_start[shops_pos[i] - 1].num * to_end[shops_pos[i] - 1].num;
    }
  }
  free(from_start);
  free(to_end);
  ans[0] = min_cost;
  ans[1] = num_of_ways % 1000000;
}

int main(void) {
  size_t n, m;
  scanf("%zd", &n);
  int* matrix = malloc(sizeof(int[MATRIX_LEN(n)]));
  read_matrix(n, matrix);
  scanf("%zd", &m);
  int* shops_pos = malloc(sizeof(int[m]));
  for (size_t i = 0; i < m; ++i)
    /* this starts with 1 */
    scanf("%d", &shops_pos[i]);

  size_t answer[2];
  solve(n, matrix, m, shops_pos, answer);
  printf("%zu %zu\n", answer[0], answer[1]); /* the results */
  free(shops_pos);
  free(matrix);
  return EXIT_SUCCESS;
}
