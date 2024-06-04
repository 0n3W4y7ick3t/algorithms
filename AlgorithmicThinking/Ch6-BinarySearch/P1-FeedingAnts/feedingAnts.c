/**
 * @author leon
 * @date Jun 04, 2024 at 20:03:34
 * @tag binary search, optimize, graph
 * @problem COCI '14 Contest 4 #4 Mravi
 * @link https://dmoj.ca/problem/coci14c4p4
 * @result AC 120/120
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define UPPER_BOUND 2000000000

typedef struct Edge {
  size_t to;
  uint8_t percent;
  bool super_pipe;
  struct Edge* next;
} Edge;

Edge** read_edges(size_t n_nodes) {
  Edge** edges = malloc(sizeof(Edge* [n_nodes]));
  size_t from, to;
  int percent, super_pipe;
  // for n_nodes nodes, there is n_nodes - 1 edges!
  for (size_t i = 0; i < n_nodes - 1; ++i) {
    scanf("%zu%zu%d%d", &from, &to, &percent, &super_pipe);
    Edge* edge = malloc(sizeof(Edge));
    edge->to = to - 1;
    edge->percent = percent;
    edge->super_pipe = super_pipe;
    edge->next = edges[from - 1];
    edges[from - 1] = edge;
  }
  return edges;
}

// wether give node amount is enough
bool enough(size_t node, double amount, size_t n, Edge* edges[n], int needed[n]) {
  // base case: node is leaf
  if (needed[node] != -1)
    return amount >= needed[node];

  // check all the children
  Edge* e = edges[node];
  double to_child;
  bool ok;
  while (e) {
    to_child = amount * e->percent / 100;
    if (e->super_pipe)
      to_child *= to_child;
    if (!enough(e->to, to_child, n, edges, needed))
      return false;
    e = e->next;
  }
  return true;
}

// solve using binary search
void BS(size_t n, Edge* edges[n], int needed[n]) {
  double lo = 0, hi = UPPER_BOUND, mid;
  // desired precision is 3 digits
  while (hi - lo > 0.0001) {
    mid = (lo + hi) / 2;
    if (enough(0, mid, n, edges, needed))
      hi = mid;
    else
      lo = mid;
  }
  printf("%.3lf\n", hi);
}

int main(void) {
  size_t n_nodes; // 1<= N <= 1000
  scanf("%zu", &n_nodes);
  Edge** edges = read_edges(n_nodes);
  int needed[n_nodes];
  for (size_t i = 0; i < n_nodes; ++i)
    scanf("%d", &(needed[i]));
  BS(n_nodes, edges, needed);
  free(edges);
  return EXIT_SUCCESS;
}
