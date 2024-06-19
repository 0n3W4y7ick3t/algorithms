/**
 * @author leon
 * @date Oct 10, 2023 at 20:10:05
 * @tag Graph, DP
 * @problem CCC '99 S4 - A Knightly Pursuit
 * @link https://dmoj.ca/problem/ccc99s4
 * @result WA
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BORAD_SIZE 100

typedef struct {
  int row;
  int col;
} Pos;

#define NUM_MOVES 8
static int row_moves[NUM_MOVES] = {1, 1, 2, 2, -1, -1, -2, -2};
static int col_moves[NUM_MOVES] = {2, -2, 1, -1, 2, -2, 1, -1};

void visit_p(int* steps, int rows, int cols, int from_row, int from_col,
             int to_row, int to_col, Pos* new_visited, int* num_visited) {
  if (to_row >= 1 && to_row <= rows && to_col >= 1 && to_col <= cols &&
      steps[to_row * cols + to_col] == -1) {
    /* this is a new position */
    steps[to_row * cols + to_col] = steps[from_row * cols + from_col] + 1;
    new_visited[(*num_visited)++] = (Pos){to_row, to_col};
  }
}

void BFS(int* steps, int rows, int cols, Pos kp) {
  Pos* cur_visited = malloc(sizeof(Pos[rows*cols]));
  Pos* new_visited = malloc(sizeof(Pos[rows*cols]));
  int num_cur, num_new;

  /* start point */
  steps[kp.row * cols + kp.col] = 0;
  cur_visited[0] = kp;
  num_cur = 1;

  while (num_cur > 0) {
    num_new = 0;
    /* for each current postion, try to visit on step forward */
    for (int i = 0; i < num_cur; ++i) {
      /* try to visit more */
      for (int j = 0; j < NUM_MOVES; ++j) {
        visit_p(steps, rows, cols, cur_visited[i].row, cur_visited[i].col,
                cur_visited[i].row + row_moves[j],
                cur_visited[i].col + col_moves[j], new_visited, &num_new);
      }
    }
    /* set num_cur to what we have found in this round */
    num_cur = num_new;
    for (int i = 0; i < num_cur; ++i)
      cur_visited[i] = new_visited[i];
  }
  free(cur_visited);
  free(new_visited);
}

int main(void) {
  int rounds, rows, cols;
  Pos kp, pp;
  scanf("%d", &rounds);
  for (int k = 0; k < rounds; k++) {
    scanf("%d%d", &rows, &cols);
    scanf("%d%d", &pp.row, &pp.col);
    scanf("%d%d", &kp.row, &kp.col);
    int* steps = malloc(sizeof(int[(rows + 1) * (cols + 1)]));
    /* reset board */
    memset(steps, -1, sizeof(int[(rows + 1) * (cols + 1)]));

    /* find out the min moves for knight to get to all positions */
    BFS(steps, rows, cols, kp);
    /* thus the knight can get to (i,j) in (Bij + 2n) steps, n=0,1,2,3,4
       but it cant get to there in (Bij + 2n + 1) steps(proved),
       because of the parity between two coordinates wont match then */

    /* grab results in (i,j), pawn's steps: i - pp.row */
    int p_moves, k_moves;
    bool won = false, staled = false;
    for (int i = pp.row; i <= rows; i++) {
      p_moves = i - pp.row;
      k_moves = steps[i * cols + pp.col];
      // p_moves == k_moves + 2n
      if (k_moves >= 0 && p_moves >= k_moves && (p_moves - k_moves) % 2 == 0) {
        printf("Win in %d knight move(s).\n", k_moves);
        won = true;
        break;
      }
    }
    if (won) continue;

    for (int i = pp.row; i <= rows; i++) {
      p_moves = i - pp.row;
      k_moves = steps[i * cols + pp.col];
      // p_moves = (k_moves + 1) + 2n
      if (k_moves >= 0 && p_moves > k_moves && (p_moves - 1 - k_moves) % 2 == 0) {
        printf("Stalemate in %d knight move(s).\n", k_moves);
        staled = true;
        break;
      }
    }
    if (staled) continue;

    printf("Loss in %d knight move(s).\n", rows - pp.row - 1);
    free(steps);
  }
  return EXIT_SUCCESS;
}
