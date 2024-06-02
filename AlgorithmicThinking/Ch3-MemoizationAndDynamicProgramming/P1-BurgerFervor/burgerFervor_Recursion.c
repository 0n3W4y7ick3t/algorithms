/**
 * @author leon
 * @date Sep 26, 2023 at 15:30:51
 * @tag DP
 * @problem UVa 10465 - Homer Simpson
 * @link https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=16&page=show_problem&problem=1406
 * @result TLE as expected
 */
#include <stdio.h>

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int solve_t(int m, int n, int t) {
  printf("solving %d\n", t);
  if (t == 0)
    return 0;

  int finish_m, finish_n;
  if (t >= m)
    /* see if job can be done by doing m in the end */
    finish_m = solve_t(m, n, t - m);
  else
    finish_m = -1;
  if (t >= n)
    finish_n = solve_t(m, n, t - n);
  else
    finish_n = -1;

  /* tried solving with doing m or n once at the end */
  if (finish_n == -1 && finish_m == -1)
    return -1;
  return max(finish_m, finish_n) + 1;
}

void solve(int m, int n, int t) {
  int result;
  result = solve_t(m, n, t);
  if (result != -1)
    /* t can be finished all by m and n */
    printf("%d\n", result);
  else {
    int rest;
    rest = t - 1;
    result = solve_t(m, n, rest);
    while (result == -1)
      result = solve_t(m, n, --rest);
    /* now we found a solution for t minutes */
    /* the most minutes can be divided with m and n */
    printf("%d %d\n", result, t - rest);
  }
}

int main(void) {
  int m, n, t;
  while (scanf("%d%d%d", &m, &n, &t) != -1)
    solve(m, n, t);
  return 0;
}
