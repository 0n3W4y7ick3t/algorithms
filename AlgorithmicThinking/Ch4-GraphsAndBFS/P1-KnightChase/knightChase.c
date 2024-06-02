/**
 * @author leon
 * @date Oct 10, 2023 at 20:10:05
 * @tag Graph, DP
 * @problem CCC '99 S4 - A Knightly Pursuit
 * @link https://dmoj.ca/problem/ccc99s4
 * @result RTE segment fault
 */
#include <stdio.h>
#include <stdlib.h>

#define BORAD_SIZE 100

typedef struct {
  int row;
  int col;
} Pos;

typedef int board[BORAD_SIZE][BORAD_SIZE];
typedef Pos visited[BORAD_SIZE * BORAD_SIZE];

void visit_p(board B, int rows, int cols, int from_row, int from_col,
             int to_row, int to_col, int* num_new_visited,
             visited new_visited) {
  if (to_row >= 1 && to_row <= rows && to_col >= 1 && to_col <= cols &&
      B[to_row][to_col] != -1) {
    /* this is a new position */
    Pos new_p = {.row = to_row, .col = to_col};
    B[to_row][to_col] = B[from_row][from_col] + 1;
    new_visited[*num_new_visited] = new_p;
    (*num_new_visited)++;
  }
}

// void print_board(board B, int rows, int cols) {
//   int i, j;
//   for (i = 0; i < rows; i++) {
//     for (j = 0; j < cols; j++)
//       printf("%3d", B[i][j]);
//     printf("\n");
//   }
// }

void BFS(board B, int rows, int cols, Pos* kp) {
  static visited cur_visited, new_visited;
  int num_cur, num_new;
  int i, from_row, from_col;

  /* start point */
  B[kp->row][kp->col] = 0;
  cur_visited[0] = (Pos){kp->row, kp->col};
  num_cur = 1;

  /* positions found in last round */
  while (num_cur > 0) {
    num_new = 0;
    /* for each current postion, try to visit on step forward */
    for (i = 0; i < num_cur; i++) {
      from_row = cur_visited[i].row;
      from_col = cur_visited[i].col;

      /* try to visit more */
      visit_p(B, rows, cols, from_row, from_col, from_row + 1, from_col + 2,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row + 2, from_col + 1,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row + 2, from_col - 1,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row + 1, from_col - 2,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row - 1, from_col - 2,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row - 2, from_col - 1,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row - 2, from_col + 1,
              &num_new, new_visited);
      visit_p(B, rows, cols, from_row, from_col, from_row - 1, from_col + 2,
              &num_new, new_visited);
    }
    /* set num_cur to what we have found in this round */
    num_cur = num_new;
    for (i = 0; i < num_cur; i++)
      cur_visited[i] = new_visited[i];
  }
}

int min(int a, int b) { return a < b ? a : b; }

int main(void) {
  int rounds, rows, cols, i, j, k;
  Pos* kp = malloc(sizeof(Pos));
  Pos* pp = malloc(sizeof(Pos));
  scanf("%d", &rounds);
  static board B;
  for (k = 0; k < rounds; k++) {
    scanf("%d%d", &rows, &cols);
    scanf("%d%d", &pp->row, &pp->col);
    scanf("%d%d", &kp->row, &kp->col);
    /* reset board */
    for (i = 0; i < rows; i++)
      for (j = 0; j < cols; j++)
        B[i][j] = -1;

    /* find out the min moves for knight to get to all positions */
    // printf("start BFS from (%d, %d) at a broad size of %dx%d...\n",
    //        kp->row, kp->col, rows, cols);
    BFS(B, rows, cols, kp);
    /* print_board(B, rows, cols); */
    /* thus the knight can get to (i,j) in (Bij + 2n) steps, n=0,1,2,3,4
       but it cant get to there in (Bij + 2n + 1) steps(proved),
       because of the parity between two coordinates wont match then */

    /* grab results in (i,j), pawn's steps: i - pp->row */
    int p_moves, k_moves;
    for (i = pp->row, j = pp->col; i <= rows; i++) {
      p_moves = i - pp->row;
      k_moves = B[i][j];
      // printf("@B[%d][%d] knight's moves: %d, pawn's move: %d\n", i, j, k_moves, p_moves);
      if (k_moves >= 0 && p_moves >= k_moves && (p_moves - k_moves) % 2 == 0) {
        printf("Win in %d knight move(s).\n", p_moves);
        break;
      }
      if (i == rows) {
        printf("Loss in %d knight move(s).\n", rows - pp->row);
        break;
      }
      p_moves = B[i + 1][j];
      if (k_moves >= 0 && p_moves >= k_moves && (p_moves - k_moves) % 2 == 0) {
        printf("Stalemate in %d knight move(s).\n", p_moves);
        break;
      }
    }
  }
  return 0;
}
