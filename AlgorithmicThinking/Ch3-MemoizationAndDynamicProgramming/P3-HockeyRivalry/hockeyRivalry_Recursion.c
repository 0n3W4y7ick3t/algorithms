/**
 * @author leon
 * @date Tue, Oct 10 2023 at 12:01:25
 * @tag recursion
 * @problem DMOJ problem cco18p1 (Canadian Computing Olympiad 2018 Day 1,
 * Problem 1)
 * @link https://dmoj.ca/problem/cco18p1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *safe_malloc(int size) {
  char *mem = malloc(size);
  if (!mem) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  return mem;
}

int max(int i, int j) { return i > j ? i : j; }

/* clang-format off */
int solve(char outcome_i[], char outcome_j[], int scores_i[], int scores_j[],
          int i, int j) {

  if (i == 0 || j == 0)
    return 0;

  int case1, case2, case3, case4;
  case2 = solve(outcome_i, outcome_j, scores_i, scores_j, i, j - 1);
  case3 = solve(outcome_i, outcome_j, scores_i, scores_j, i - 1, j);
  case4 = solve(outcome_i, outcome_j, scores_i, scores_j, i - 1, j - 1);

  if ((outcome_i[i] == 'W' && outcome_j[j] == 'L' && scores_i[i] > scores_j[j]) ||
      (outcome_i[i] == 'L' && outcome_j[j] == 'W' && scores_i[i] < scores_j[j])) {
    case1 = case4 + scores_i[i] + scores_j[j];
  } else
    case1 = case4;

  return max(case1, max(case2, case3));
}

int main(void) {
  int num_games, k, result;
  scanf("%d ", &num_games);

  char *outcome_i = (char *)safe_malloc(num_games + 1);
  char *outcome_j = (char *)safe_malloc(num_games + 1);
  int *scores_i = (int *)safe_malloc(num_games + 1);
  int *scores_j = (int *)safe_malloc(num_games + 1);
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

  result =
      solve(outcome_i, outcome_j, scores_i, scores_j, num_games, num_games);
  printf("%d\n", result);
  return 0;
}
