/**
 * @author leon
 * @date Oct 16, 2023 at 10:40:37
 * @tag Graph, BFS
 * @problem DMOJ wc18c1s3
 * @link https://dmoj.ca/problem/wc18c1s3
 */
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_GOAL 1000000

/* bitmap starts */
typedef uint32_t word_t;
enum { BIT_PER_WORD = sizeof(word_t) * CHAR_BIT };
#define WORD_OFFSET(i) ((i) / BIT_PER_WORD)
#define BIT_OFFSET(i) ((i) % BIT_PER_WORD)

void set_bit(word_t *w, int n) { w[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n)); }

void clear_bit(word_t *w, int n) { w[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n)); }

int get_bit(word_t *w, int n) {
  word_t bit = w[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
  return bit != 0;
}
/* bitmap ends */

void visit(int *steps, word_t *itching, int from, int to, int *new_visited,
           int *num_new, int *cnt) {
  (*cnt)++;
  if (steps[to] == -1 && get_bit(itching, to) == 0) {
    /* not discoverd safe zone */
    steps[to] = steps[from] + 1;
    new_visited[(*num_new)++] = to;
  }
}

int solve(word_t *itching, int goal, int jump) {
  size_t bytes;
  bytes = sizeof(int) * (goal + jump);
  int *steps = malloc(bytes);
  // a diry trick because -1(32bit) is all 1, -1(8bit) is also all 1
  memset(steps, -1, bytes);

  int *cur_pos = malloc(bytes);
  int *new_pos = malloc(bytes);
  int *tmp;
  int num_cur_pos, num_new_pos;

  /* init BFS search */
  steps[0] = 0;
  num_cur_pos = 1;
  cur_pos[0] = 0;

  int i, j, num_new_pos_flag, cnt;
  cnt = 0;
  while (num_cur_pos > 0) {
    num_new_pos = 0;
    /* search all possible postions one step away */
    for (i = 0; i < num_cur_pos; i++) {
      if (cur_pos[i] >= goal) {
        // printf("times of calling visit: %d\n", cnt);
        return steps[cur_pos[i]];
      }

      /* go up j steps */
      num_new_pos_flag = -1;
      while (num_new_pos_flag != num_new_pos) {
        /* save the old value */
        /* only breaks while loop when can't not find new pos,
         ie. cant jump up anymore */
        num_new_pos_flag = num_new_pos;
        if (cur_pos[i] + jump < goal + jump)
          visit(steps, itching, cur_pos[i], cur_pos[i] + jump, new_pos,
                &num_new_pos, &cnt);
      }

      /* go down 1~jump steps */
      for (j = 1; j < jump; j++) {
        if (cur_pos[i] - j >= 0)
          visit(steps, itching, cur_pos[i], cur_pos[i] - j, new_pos,
                &num_new_pos, &cnt);
      }
    }
    num_cur_pos = num_new_pos;

    tmp = cur_pos;
    cur_pos = new_pos;
    new_pos = tmp;
  }
  return -1;
}

int main() {
  /* goal, step of jumping, number of itching zones */
  int h, j, n, i;
  scanf("%d%d%d", &h, &j, &n);
  /* use a bitmap to store the itching zones, 1 for itching */
  int itches = (h + j) / BIT_PER_WORD + 1;
  word_t *itching = malloc(sizeof(word_t) * itches);
  memset(itching, 0, sizeof(word_t) * itches);

  int a, b, c;
  for (i = 0; i < n; i++) {
    scanf("%d%d", &a, &b);
    for (c = a; c <= b; c++)
      set_bit(itching, c);
  }
  printf("%d\n", solve(itching, h, j));
  return 0;
}
