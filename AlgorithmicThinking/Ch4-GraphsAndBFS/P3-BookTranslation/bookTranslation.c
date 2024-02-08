/**
 * @author leon
 * @date Oct 18, 2023 at 10:02:51
 * @tag graphs, dynamic programming
 * @problem DMOJ ecna16d
 * @link https://dmoj.ca/problem/ecna16d
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LANS 101
#define WORD_LEN 16

/* there are not much info in node,
   so we can use an adjacency list */
typedef struct Edge {
  int cost, lang2; /* lang1 is the index of this link list */
  struct Edge *next;
} Edge;

typedef int moves[MAX_LANS];
typedef int costs[MAX_LANS];

static inline void not_null(void *ptr) {
  if (!ptr) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
}

char *read_word(size_t size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  not_null(str);
  while ((ch = getchar()) != EOF && (ch != '\n') && (ch != ' ') &&
         (ch != '\t')) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      not_null(str);
    }
  }
  str[len] = '\0';
  return str;
}

/* treat all langs cant be found as an imaginary node[num_targets + 1],
  it will not be processed anyway */
int find_lang(char **langs, char *lang) {
  int i = 0;
  while (strcmp(langs[i], lang) != 0)
    i++;
  return i;
}

void add_pos(int from, int to, int *new_pos, int *num_new_pos,
             moves min_moves) {
  if (min_moves[to] == -1) {
    min_moves[to] = min_moves[from] + 1;
    new_pos[(*num_new_pos)++] = to;
  }
}

void BFS(Edge *adj_list[], int num_target, costs min_costs) {
  static moves min_moves;
  int *cur_pos = malloc(sizeof(int) * MAX_LANS);
  int *new_pos = malloc(sizeof(int) * MAX_LANS);
  int *tmp; // for switching
  int num_cur_pos, num_new_pos;
  int i, from_lang, new_lang, best;
  Edge *e;
  for (i = 0; i < num_target; i++) {
    min_costs[i] = -1;
    min_moves[i] = -1;
  }

  // start from English, index 0
  min_moves[0] = 0;
  cur_pos[0] = 0;
  num_cur_pos = 1;
  while (num_cur_pos > 0) {
    num_new_pos = 0;
    for (i = 0; i < num_cur_pos; i++) {
      from_lang = cur_pos[i];
      /* all possible poss from from_lang is attached as a linked list
       at adj_list[from_lang] */
      e = adj_list[from_lang];
      while (e) {
        add_pos(from_lang, e->lang2, new_pos, &num_new_pos, min_moves);
        e = e->next;
      }
    }
    /* finished this round of BFS */

    /* calculate the min cost it takes from each posible node in current search
       to the new positions */
    for (i = 0; i < num_new_pos; i++) {
      best = -1;
      new_lang = new_pos[i];
      e = adj_list[new_lang];
      /* find all node can be access from to_lang
        (we store two edges for one bi-directed connection) */
      while (e) {
        /* e->lang2 is one step before to_lang */
        if (min_moves[e->lang2] + 1 == min_moves[new_lang] &&
            (best == -1 || best > e->cost))
          best = e->cost;
        e = e->next;
      }
      min_costs[new_lang] = best;
    }

    num_cur_pos = num_new_pos;
    tmp = cur_pos;
    cur_pos = new_pos;
    new_pos = tmp;
  }
}

void solve(int num_targets, costs min_costs) {
  int i, total = 0;
  for (i = 1; i < num_targets; i++)
    if (min_costs[i] == -1) {
      printf("Impossible\n");
      return;
    } else
      total += min_costs[i];
  printf("%d\n", total);
}

int main(void) {
  int num_target, num_trans, i, j, cost, lang1_index, lang2_index;
  /* all languages */
  static char *langs[MAX_LANS] = {NULL};
  /* adjacency list for storing edges list */
  static Edge *adj_list[MAX_LANS] = {NULL};
  costs min_costs;
  char *lang1, *lang2;
  Edge *edge;

  scanf("%d%d\n", &num_target, &num_trans);
  langs[0] = "English";
  for (i = 1; i <= num_target; i++)
    langs[i] = read_word(WORD_LEN);

  /* read edges */
  for (j = 0; j < num_trans; j++) {
    lang1 = read_word(WORD_LEN);
    lang2 = read_word(WORD_LEN);
    scanf("%d\n", &cost);
    lang1_index = find_lang(langs, lang1);
    lang2_index = find_lang(langs, lang2);

    edge = malloc(sizeof(Edge));
    not_null(edge);
    edge->lang2 = lang2_index;
    edge->cost = cost;
    edge->next = adj_list[lang1_index];
    adj_list[lang1_index] = edge;

    edge = malloc(sizeof(Edge));
    not_null(edge);
    edge->lang2 = lang1_index;
    edge->cost = cost;
    edge->next = adj_list[lang2_index];
    adj_list[lang2_index] = edge;
  }

  /* when the edge for each tartget languaes is min,
   only we can have a min edges for all of them */
  BFS(adj_list, num_target + 1, min_costs);
  solve(num_target + 1, min_costs);
  return 0;
}
