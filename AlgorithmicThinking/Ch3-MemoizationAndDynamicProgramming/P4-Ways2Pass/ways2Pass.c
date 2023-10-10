/**
 * @author leon
 * @date Oct 10, 2023 at 16:22:26
 * @tag dynamic programming
 * @problem UVa problem 10910
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1851
 */
#include <stdio.h>

#define SIZE 70

int helper(int items, int buskets, int memo[SIZE + 1][SIZE + 1]) {
  int result, m;
  if (memo[items][buskets] != -1)
    return memo[items][buskets];
  if (buskets == 0 && items == 0)
    return 1;
  if (buskets == 0)
    return 0;
  result = 0;
  for (m = 0; m <= items; m++)
    result += helper(items - m, buskets - 1, memo);
  memo[items][buskets] = result;
  return memo[items][buskets];
}

int solve(int n, int t, int p) {
  /* put num of surplus items in n, how many ways? */
  int i, j;
  int memo[SIZE + 1][SIZE + 1];
  for (i = 0; i <= SIZE; i++)
    for (j = 0; j <= SIZE; j++)
      memo[i][j] = -1;
  return helper(t - n * p, n, memo);
}

int main(void) {
  int cases, n, t, p;
  scanf("%d ", &cases);
  int k;
  for (k = 0; k < cases; k++) {
    scanf("%d%d%d", &n, &t, &p);
    printf("%d\n", solve(n, t, p));
  }
  return 0;
}
