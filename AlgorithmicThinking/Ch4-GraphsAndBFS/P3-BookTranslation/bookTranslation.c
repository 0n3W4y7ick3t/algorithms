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

void check_alloc(void *ptr) {
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
  check_alloc(str);
  while ((ch = getchar()) != EOF && (ch != '\n') && (ch != ' ') &&
         (ch != '\t')) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      check_alloc(str);
    }
  }
  str[len] = '\0';
  return str;
}

/* if not found, return the next index where we can insert it */
int find_lang(char **langs, char *lang) {
  int i = 0;
  while (strcmp(langs[i], lang) != 0)
    i++;
  langs[i] = lang;
  return i;
}

void add_pos(int from, int to, int *new_pos, int *num_new_pos,
             moves min_moves) {
  if (min_moves[to] == -1) {
    min_moves[to] = min_moves[from] + 1;
    new_pos[(*num_new_pos)++] = to;
  }
}

void find_distances(Edge *adj_list[], int num_target, costs min_costs) {
  static moves min_moves;
  int *cur_pos = malloc(sizeof(int) * MAX_LANS);
  int *new_pos = malloc(sizeof(int) * MAX_LANS);
  int *tmp; // for switching
  int num_cur_pos, num_new_pos;
  int i, from_lang, to_lang, best;
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

    /* calculate the min cost it takes to the new positions */
    for (i = 0; i < num_new_pos; i++) {
      best = -1;
      to_lang = new_pos[i];
    }

    tmp = cur_pos;
    cur_pos = new_pos;
    new_pos = tmp;
  }
}

int main(void) {
  int num_target, num_trans, i, j, cost, lang1_index, lang2_index, result;
  /* all languages */
  static char *langs[MAX_LANS] = {NULL};
  /* adjacency list for storing edges list */
  static Edge *adj_list[MAX_LANS] = {NULL};
  costs min_costs;
  char *lang1, *lang2;
  Edge *edge;

  scanf("%d%d", &num_target, &num_trans);
  langs[0] = "English";
  for (i = 1; i < num_target; i++)
    langs[i] = read_word(WORD_LEN);

  /* read edges */
  for (j = 0; j < num_trans; j++) {
    lang1 = read_word(WORD_LEN);
    lang2 = read_word(WORD_LEN);
    scanf("%d", &cost);
    lang1_index = find_lang(langs, lang1);
    lang2_index = find_lang(langs, lang2);

    edge = malloc(sizeof(Edge));
    check_alloc(edge);
    edge->lang2 = lang2_index;
    edge->cost = cost;
    edge->next = adj_list[lang1_index];
    adj_list[lang1_index] = edge;

    edge = malloc(sizeof(Edge));
    check_alloc(edge);
    edge->lang2 = lang1_index;
    edge->cost = cost;
    edge->next = adj_list[lang2_index];
    adj_list[lang2_index] = edge;
  }

  /* when the edge for each tartget languaes is min,
   only we can have a min edges for all of them */
  find_distances(adj_list, num_target + 1, min_costs);
  return 0;
}
