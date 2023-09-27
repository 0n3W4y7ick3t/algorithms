/**
 * @author leon
 * @date Sep 27, 2023 at 10:19:40
 * @link UVa 10465 - Homer Simpson
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=16&page=show_problem&problem=1406
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define MAXMINUTES 10000

int max(int n1, int n2) { return n1 > n2 ? n1 : n2; }

int solve_t(int m, int n, int t, int *memo) {
  /* printf("solving: %d\n", t); */
  if (memo[t] != 0)
    return memo[t];

  if (t == 0)
    return 0;

  int finish_m, finish_n;
  if (t >= m) {
    /* see if job can be done by doing m in the end */
    if (memo[t - m] != 0)
      finish_m = memo[t - m];
    else {
      finish_m = solve_t(m, n, t - m, memo);
      memo[t - m] = finish_m;
    }
  } else
    finish_m = -1;

  if (t >= n) {
    if (memo[t - n] != 0)
      finish_n = memo[t - n];
    else {
      finish_n = solve_t(m, n, t - n, memo);
      memo[t - n] = finish_n;
    }
  } else
    finish_n = -1;

  /* tried solving with doing m or n once at the end */
  if (finish_n == -1 && finish_m == -1) {
    /* no solution */
    memo[t] = -1;
    return -1;
  }
  memo[t] = max(finish_m, finish_n) + 1;
  return memo[t];
}

void solve(int m, int n, int t) {
  int memo[MAXMINUTES + 1] = {0};
  /* memo[i] = 0 means not solved yet,
     also the answer for t=0 is 0,
     but its a simple case can be neglected */
  int result;
  result = solve_t(m, n, t, memo);
  if (result != -1)
    /* t can be finished all by m and n */
    printf("%d\n", result);
  else {
    int rest;
    rest = t - 1;
    result = solve_t(m, n, rest, memo);
    while (result == -1)
      result = solve_t(m, n, --rest, memo);
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

