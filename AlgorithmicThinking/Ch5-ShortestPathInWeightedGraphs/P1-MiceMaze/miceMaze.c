/**
 * @author leon
 * @date Oct 20, 2023 at 16:26:21
 * @tag Dijkstra, Graph
 * @problem UVa problem 1112 - Mice and Maze
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=246&page=show_problem&problem=3553
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 101
#define DONE(i) i == 0 ? 'O' : '#'

typedef struct Edge {
  int to;
  int weight;
  struct Edge* next;
} Edge;

/* print costs and done from 1 to num */
void print_node(int costs[], bool done[], int num) {
  printf("node(cost)done: 1(%2d)%c", costs[1], DONE(done[1]));
  int i;
  for (i = 2; i <= num; ++i) {
    printf(", %2d(%2d)%c", i, costs[i], DONE(done[i]));
  }
  printf("\n");
}

void print_edges(Edge* e, int from) {
  printf("connect from node %d node(weight):\n %d(%2d)", from, e->to,
         e->weight);
  Edge* p = e->next;
  while (p) {
    printf(", %d(%2d)", p->to, p->weight);
    p = p->next;
  }
  putchar('\n');
}

void print_graph(Edge* adj_cells[MAX_CELLS], int num) {
  int i;
  for (i = 1; i <= num; ++i) {
    print_edges(adj_cells[i], i);
  }
}

int solve(Edge* adj_cells[], int num_cells, int start, int limit) {
  static bool done[MAX_CELLS]; /* if min cost for each node has been found */
  static int costs[MAX_CELLS]; /* final cost for each node */
  memset(done, 0, sizeof(bool) * MAX_CELLS);
  memset(costs, -1, sizeof(int) * MAX_CELLS);

  costs[start] = 0;

  bool found;
  int cost, old_cost;
  int i, j, min;
  Edge* p;

  while (true) {
    cost = -1;
    found = false;

    print_node(costs, done, num_cells);

    /* find the current node(not set to found yet) with min cost,
      this is the node we can finish in this round of searching */
    for (j = 1; j <= num_cells; ++j) {
      if (!done[j] && costs[j] != -1) {
        if (cost == -1 || costs[j] < cost) {
          cost = costs[j];
          min = j;
          found = true;
        }
      }
    }

    /* no new found node, time to stop */
    if (!found)
      break;

    /* when we finished one node, try update others with it. */
    done[min] = true;
    p = adj_cells[min];
    /* traverse all edges from this node */
    printf("probing from node %2d\n", min);
    while (p) {
      old_cost = costs[p->to];
      if (old_cost == -1 || old_cost > cost + p->weight) {
        costs[p->to] = cost + p->weight;
        printf("update cost of node %2d to %2d\n", p->to, costs[p->to]);
      }
      p = p->next;
    }
  }

  int result = 0;
  for (i = 1; i <= num_cells; ++i)
    if (costs[i] != -1 & costs[i] <= limit)
      result++;

  return result;
}

int main(void) {
  int cases, num_cells, exit, limit, num_edges;
  int from, to, weight;
  int case_num, i;
  static Edge* adj_cells[MAX_CELLS]; /* start from zero */
  scanf("%2d", &cases);
  for (case_num = 0; case_num < cases; case_num++) {
    scanf("%2d%2d%2d%2d", &num_cells, &exit, &limit, &num_edges);
    /* IMPORTANT
      if we dont reset this,
      all edges in the last case will be included in this case */
    for (i = 1; i <= num_cells; ++i)
      adj_cells[i] = 0;
    for (i = 0; i < num_edges; ++i) {
      scanf("%2d%2d%2d", &from, &to, &weight);
      Edge* p = malloc(sizeof(Edge));
      /* construct the graph reversely,
        so we can use exit as start cell, do one sinle Dijkstra */
      p->to = from, p->weight = weight;
      p->next = adj_cells[to];
      adj_cells[to] = p;
    }

    print_graph(adj_cells, num_cells);

    if (case_num != 0) {
      printf("\n");
    }
    printf("time limit %d to get to cell %d\n", limit, exit);
    printf("%d\n", solve(adj_cells, num_cells, exit, limit));
    /* TODO: how to free edges efficiently
      Maybe use a stack to store all edges then free on by one? */
  }
  return 0;
}
