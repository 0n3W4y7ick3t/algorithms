/**
 * @author leon
 * @date Sep 27, 2023 at 21:50:36
 * @tag DP
 * @problem UVa 10465 - Homer Simpson
 * @link  https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=16&page=show_problem&problem=1406
 * @result AC
 */
#include <stdio.h>
#include <stdlib.h>

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

void solve(int m, int n, int t) {
  int sub;
  int finish_m, finish_n;
  int* dp = malloc(sizeof(int) * (t + 1));
  dp[0] = 0; /* no need to care about others */

  for (sub = 1; sub <= t; sub++) {
    if (sub >= m)
      /* see if job can be done by doing m in the end */
      finish_m = dp[sub - m];
    else
      finish_m = -1;
    if (sub >= n)
      finish_n = dp[sub - n];
    else
      finish_n = -1;

    /* tried solving with doing m or n once at the end */
    if (finish_n == -1 && finish_m == -1)
      dp[sub] = -1;
    else
      dp[sub] = max(finish_m, finish_n) + 1;
    /* printf("%d: %d\n", sub, dp[sub]); */
  }

  sub--; /* reset sub to t */
  while (dp[sub] == -1)
    sub--;
  if (sub == t)
    printf("%d\n", dp[sub]);
  else
    printf("%d %d\n", dp[sub], t - sub);
}

int main(void) {
  int m, n, t;
  while (scanf("%d%d%d", &m, &n, &t) != -1)
    solve(m, n, t);
  return 0;
}
