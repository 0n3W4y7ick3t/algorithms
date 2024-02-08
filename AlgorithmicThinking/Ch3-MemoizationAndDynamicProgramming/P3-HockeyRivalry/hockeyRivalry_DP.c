/**
 * @author leon
 * @date Tue, Oct 10 2023 at 14:11:08
 * @tag dynamic programming
 * @problem DMOJ problem cco18p1 (Canadian Computing Olympiad 2018 Day 1,
 * Problem 1)
 * @link https://dmoj.ca/problem/cco18p1
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_GAMES 1000

void *safe_malloc(int size) {
  char *mem = malloc(size);
  if (!mem) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  return mem;
}

int max(int i, int j) { return i > j ? i : j; }

int solve(char outcome_i[], char outcome_j[], int scores_i[], int scores_j[],
          int num_games) {

  int i, j, result_ij;
  static int dp[MAX_GAMES + 1][MAX_GAMES + 1];
  /* initial base case */
  for (i = 0; i <= MAX_GAMES; i++)
    dp[i][0] = 0;
  for (i = 1; i <= MAX_GAMES; i++)
    dp[0][i] = 0;

  for (i = 1; i <= num_games; i++)
    for (j = 1; j <= num_games; j++) {
      if ((outcome_i[i] == 'W' && outcome_j[j] == 'L' &&
           scores_i[i] > scores_j[j]) ||
          (outcome_i[i] == 'L' && outcome_j[j] == 'W' &&
           scores_i[i] < scores_j[j]))
        result_ij = dp[i - 1][j - 1] + scores_i[i] + scores_j[j];
      else
        result_ij = dp[i - 1][j - 1];
      /* result(i,j) is definately bigger than result(i-1,j-1), so we skip it */
      dp[i][j] = max(result_ij, max(dp[i][j - 1], dp[i - 1][j]));
    }

  /* printf("dp[%d][%d] = %d\n", i, j, dp[i][j]); */
  return dp[num_games][num_games];
}

int main(void) {
  int num_games, k, result;
  scanf("%d ", &num_games);

  char outcome_i[num_games + 1], outcome_j[num_games + 1];
  int scores_i[num_games + 1], scores_j[num_games + 1];
  /* begins at index 1 */
  for (k = 1; k <= num_games; k++)
    scanf("%c", &outcome_i[k]);
  for (k = 1; k <= num_games; k++)
    /* "%d " will skip the space but "%d" will not */
    scanf("%d ", &scores_i[k]);
  for (k = 1; k <= num_games; k++)
    scanf("%c", &outcome_j[k]);
  for (k = 1; k <= num_games; k++)
    scanf("%d ", &scores_j[k]);
  /* for (k = 1; k <= num_games; k++) */
  /*   printf("%c%d %c%d\n", outcome_i[k], scores_i[k], outcome_j[k],
   * scores_j[k]); */

  result = solve(outcome_i, outcome_j, scores_i, scores_j, num_games);
  printf("%d\n", result);
  return 0;
}
