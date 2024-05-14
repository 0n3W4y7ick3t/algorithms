/**
 * @author leon
 * @date Oct 20, 2023 at 16:26:21
 * @tag
 * @problem UVa problem 1112 - Mice and Maze
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=246&page=show_problem&problem=3553
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 101

typedef struct Pass {
  int to;
  int weight;
  struct Pass* next;
} Pass;

static inline void not_null(void* ptr) {
  if (!ptr) {
    fprintf(stderr, "err, pointer is null!\n");
    exit(1);
  }
}

void solve(Pass* adj_cells[], int num_cells, int from, int timer) {
  int total = 0;
  static bool done[MAX_CELLS];
  static int min_times[MAX_CELLS];

  memset(done, 0, sizeof(bool) * MAX_CELLS);
  memset(min_times, -1, sizeof(int) * MAX_CELLS);

  bool found;
  int i, j;
  int min_time, min_time_index, old_time;
  Pass* p;
  min_times[from] = 0;

  for (i = 0; i < num_cells; i++) {
    min_time = -1;
    found = false;
    // check if there is a passage between curent and all others
    for (j = 0; j <= num_cells && j != i; j++) {
      if (!done[j] && min_times[j] >= 0) {
        // maybe we can update min_time with [j]
        if (min_time == -1 || min_times[j] < min_time) {
          min_time = min_times[j];
          min_time_index = j;
          found = true;
        }
      }
    }

    if (!found)
      break; // time to stop
    done[min_time_index] = true;

    // when we found a shortest passage, try update others with it.
    p = adj_cells[min_time_index];
    while (p) {
      old_time = min_times[p->to];
      if (old_time == -1 || old_time > min_time + p->weight)
        min_times[p->to] = min_time + p->weight;
      p = p->next;
    }
  }

  for (i = 1; i <= num_cells; i++)
    if (min_times[i] <= timer)
      total++;

  printf("%d\n", total);
}

int main(void) {
  int* p;
  not_null(NULL);
  int cases, num_cells, num_pass, exit, timer, i, j, from, to, weight;
  static Pass* adj_cells[MAX_CELLS];
  scanf("%d", &cases);
  for (i = 0; i < cases; i++) {
    scanf("%d%d%d%d", &num_cells, &exit, &timer, &num_pass);
    for (j = 0; j < num_pass; j++) {
      scanf("%d%d%d", &from, &to, &weight);
      Pass* p = malloc(sizeof(Pass));
      not_null(p);
      /* construct the graph reversely,
        so we can use exit as start cell, do one sinle Dijkstra */
      p->to = from, p->weight = weight;
      p->next = adj_cells[to];
      adj_cells[to] = p;
    }
    solve(adj_cells, num_cells, exit, timer);
  }
  return 0;
}
