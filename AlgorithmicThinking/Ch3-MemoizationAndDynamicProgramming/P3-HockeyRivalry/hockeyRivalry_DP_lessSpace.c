/**
 * @author leon
 * @date Tue, Oct 10 2023 at 00:11:08
 * @tag DP
 * @problem CCO '18 P1 - Geese vs. Hawks
 * @link https://dmoj.ca/problem/cco18p1
 * @result AC
 */

#include <stdio.h>

#define MAX_GAMES 1000

int max(int i, int j) { return i > j ? i : j; }

int solve(char outcome_i[], char outcome_j[], int scores_i[], int scores_j[],
          int num_games) {

  int i, j, result_ij;
  int current[MAX_GAMES + 1], previous[MAX_GAMES + 1];
  /* initial base case */
  for (i = 0; i <= MAX_GAMES; i++)
    previous[i] = 0;

  for (i = 1; i <= num_games; i++) {
    for (j = 1; j <= num_games; j++) {
      if ((outcome_i[i] == 'W' && outcome_j[j] == 'L' &&
           scores_i[i] > scores_j[j]) ||
          (outcome_i[i] == 'L' && outcome_j[j] == 'W' &&
           scores_i[i] < scores_j[j]))
        result_ij = previous[j - 1] + scores_i[i] + scores_j[j];
      else
        result_ij = previous[j - 1];
      /* result(i,j) is definately bigger than result(i-1,j-1), so we skip it */
      current[j] = max(result_ij, max(previous[j], current[i - 1]));
    }
    for (j = 0; j <= num_games; j++)
      previous[j] = current[j];
  }

  /* printf("dp[%d][%d] = %d\n", i, j, dp[i][j]); */
  return current[num_games];
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
