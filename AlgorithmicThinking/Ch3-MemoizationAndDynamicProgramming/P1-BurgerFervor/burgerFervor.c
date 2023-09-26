/**
 * @author leon
 * @date Sep 26, 2023 at 15:30:51
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=16&page=show_problem&problem=1406
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int solve_t(int m, int n, int t) {
  if (t == 0)
    return 0;

  int finish_m;
  if (t >= m) /* see if job can be done by doing m in the end */
    finish_m = solve_t(m, n, t - m);
  else
    finish_m = -1; /* no time finishing with m */
  int finish_n;
  if (t >= n)
    finish_n = solve_t(m, n, t - n);
  else
    finish_n = -1;

  /* so far tried solving with m and n. */
  if (finish_n == -1 && finish_m == -1)
    return -1;
  return max(finish_m, finish_n) + 1;
}

int main(void) {
  /* dealing inputs */
  // solve();
  return 0;
}

