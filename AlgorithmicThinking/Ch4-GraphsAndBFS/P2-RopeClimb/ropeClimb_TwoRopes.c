/**
 * @author leon
 * @date Oct 17, 2023 at 21:11:10
 * @tag Graph, BFS
 * @problem WC '18 Contest 1 S3 - Reach for the Top
 * @link https://dmoj.ca/problem/wc18c1s3
 * @result WA on test cases #5, 6
 */

/*** remodel the question ***
 * We make every fall down move one step far, and consider consecutive fall
 * moves, no matter how many they are, take one move.
 * use another imaginary rope, swithing to this rope takes one move,
 * and fall down on it or move back to the original rope both take no move,
 * thus we get a new graph(two ropes), which has a significant less edges than
 * the original one.
 */
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_GOAL 1000000

typedef struct {
  int height;
  uint8_t on_rope;
} Pos;

/* bitmap starts */
typedef uint32_t word_t;
enum { BIT_PER_WORD = sizeof(word_t) * CHAR_BIT };
#define WORD_OFFSET(i) ((i) / BIT_PER_WORD)
#define BIT_OFFSET(i) ((i) % BIT_PER_WORD)

void set_bit(word_t* w, int n) { w[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n)); }

void clear_bit(word_t* w, int n) { w[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n)); }

int get_bit(word_t* w, int n) {
  word_t bit = w[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
  return bit != 0;
}
/* bitmap ends */

// jump up
void jump(int* steps, word_t* itching, int from, int to, Pos* pos,
          int* num_pos) {
  if (get_bit(itching, to) == 0 &&
      (steps[2 * to] == -1 || steps[2 * to] > steps[2 * from] + 1)) {
    // safe zone, not discorved or can be updated
    steps[2 * to] = steps[2 * from] + 1; // take one extra move
    pos[(*num_pos)++] = (Pos){.height = to, .on_rope = 0};
  }
}

// switch to rope 1
void move2_rope1(int* steps, int from, Pos* pos, int* num_pos) {
  if (steps[2 * from + 1] == -1 || steps[2 * from + 1] > steps[2 * from] + 1) {
    //  not discorved or can be updated
    steps[2 * from + 1] = steps[2 * from] + 1; // take one extra move
    pos[(*num_pos)++] = (Pos){.height = from, .on_rope = 1};
  }
}

// fall down
void fall(int* steps, int from, int to, Pos* pos, int* num_pos) {
  if (steps[2 * to + 1] == -1 || steps[2 * to + 1] > steps[2 * from + 1] + 1) {
    // not discorved or can be updated
    steps[2 * to + 1] = steps[2 * from + 1]; // doesnt take any move
    pos[(*num_pos)++] = (Pos){.height = to, .on_rope = 1};
  }
}

// switch to rope 0
void move2_rope0(int* steps, word_t* itching, int from, Pos* pos,
                 int* num_pos) {
  if (get_bit(itching, from) == 0 &&
      (steps[2 * from] == -1 || steps[2 * from] > steps[2 * from + 1])) {
    // safe zone, not discorved or can be updated
    steps[2 * from] = steps[2 * from + 1]; // doesnt take any move
    pos[(*num_pos)++] = (Pos){.height = from, .on_rope = 0};
  }
}

void BFS(int* steps, word_t* itching, int goal, int jumping) {
  size_t bytes;
  bytes = sizeof(Pos) * (goal * 2) * 2;
  Pos* cur_pos = malloc(bytes);
  Pos* new_pos = malloc(bytes);
  Pos* tmp;
  int num_cur_pos, num_new_pos, from_rope, from_height, i;

  /* init BFS search */
  steps[0] = 0; // 2 * 0 + 0
  num_cur_pos = 1;
  cur_pos[0] = (Pos){.height = 0, .on_rope = 0};

  while (num_cur_pos > 0) {
    num_new_pos = 0;
    /* search all possible moves */
    for (i = 0; i < num_cur_pos; i++) {
      from_height = cur_pos[i].height;
      from_rope = cur_pos[i].on_rope;

      if (from_rope == 0) {
        // two ways of moving that take one extra move
        // should be parts of the NEXT search of BFS(cant be find in steps + 1)
        if (from_height + jumping < goal * 2)
          jump(steps, itching, from_height, from_height + jumping, new_pos,
               &num_new_pos);
        move2_rope1(steps, from_height, new_pos, &num_new_pos);
      } else {
        // dont take any extra moves
        // should be add to the CURRENT search of BFS(can be find in steps)
        if (from_height - 1 >= 0)
          fall(steps, from_height, from_height - 1, cur_pos, &num_cur_pos);
        move2_rope0(steps, itching, from_height, cur_pos, &num_cur_pos);
      }
    }
    num_cur_pos = num_new_pos;

    tmp = cur_pos;
    cur_pos = new_pos;
    new_pos = tmp;
  }
  free(cur_pos);
  free(new_pos);
}

int main() {
  /* goal, step of jumping, number of itching zones */
  int h, j, n;
  scanf("%d%d%d", &h, &j, &n);
  /* use a bitmap to store the itching zones, 1 for itching */
  int itches = (h * 2) / BIT_PER_WORD + 1;
  word_t* itching = malloc(sizeof(word_t) * itches);
  memset(itching, 0, sizeof(word_t) * itches);

  // set itching
  int a, b, c, i, result;
  for (i = 0; i < n; i++) {
    scanf("%d%d", &a, &b);
    for (c = a; c <= b; c++)
      set_bit(itching, c);
  }
  int* steps = malloc(sizeof(int) * (h * 2) * 2);
  memset(steps, -1, sizeof(int) * (h * 2) * 2);
  BFS(steps, itching, h, j);
  // grab result
  result = steps[2 * h];
  for (i = h; i < h * 2; i++) {
    if (steps[2 * i] != -1)
      result = result < steps[2 * i] ? result : steps[2 * i];
    // printf("height:%d [%d] [%d]\n", i, steps[2 * i], steps[2 * i + 1]);
  }

  printf("%d\n", result);
  free(itching);
  free(steps);
  return 0;
}
