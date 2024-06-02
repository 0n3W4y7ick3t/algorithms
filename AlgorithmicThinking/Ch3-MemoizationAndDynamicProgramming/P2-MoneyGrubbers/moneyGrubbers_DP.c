/**
 * @author leon
 * @date Oct 04, 2023 at 00:16:07
 * @tag DP
 * @problem UVa 10980 - Lowest Price in Town
 * @link https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1921
 * @result AC
 */
#include <stdio.h>

#define MAX_SCHEMES 20
#define MAX_TRY 200

int read_number(int* number) {
  /* reads in "23 134 15", one number a time, into arg
    return 1 when there are more numbers left
    return 0 when there is none left */
  int ch, ret;
  ret = 0;
  ch = getchar();
  while (ch != ' ' && ch != '\n') {
    /* read in 0-9 here */
    ret = ret * 10 + ch - '0';
    ch = getchar();
  }
  *number = ret;
  if (ch == ' ')
    /* return 1 when there is more */
    return 1;
  return 0;
}

double min(double a, double b) { return a < b ? a : b; }

double buy(double prices[], int num[], int num_schemes, double unit_price,
           int want) {
  int try, buy;
  double best, sub_best;
  double dp[MAX_TRY];
  dp[0] = 0;
  for (buy = 1; buy < MAX_TRY; buy++) {
    sub_best = dp[buy - 1];
    best = sub_best + unit_price;
    for (try = 0; try < num_schemes; try++) {
      if (buy >= num[try]) {
        /* try other schemes, will they be better? */
        sub_best = dp[buy - num[try]];
        best = min(best, sub_best + prices[try]);
      }
    }
    /* best is now the best of buy */
    dp[buy] = best;
  }

  best = dp[want];
  for (buy = want + 1; buy < MAX_TRY; buy++)
    /* search in  those bought more than want */
    best = min(best, dp[buy]);

  return best;
}

int main(void) {
  int test_case, num_schemes, want, more_want, i;
  double unit_price, result;
  int num[MAX_SCHEMES] = {0};       /* to store the nums of schemes */
  double prices[MAX_SCHEMES] = {0}; /* to store the prices of schemes */

  test_case = 0;
  while (scanf("%lf%d", &unit_price, &num_schemes) != -1) {
    test_case++;
    for (i = 0; i < num_schemes; i++)
      scanf("%d%lf", &num[i], &prices[i]);
    scanf(" "); /* skip CR */
    printf("Case %d:\n", test_case);

    more_want = read_number(&want);
    while (more_want) {
      /* buy want of apples */
      result = buy(prices, num, num_schemes, unit_price, want);
      printf("Buy %d for $%.2lf\n", want, result);
      more_want = read_number(&want);
    }
    /* left */
    result = buy(prices, num, num_schemes, unit_price, want);
    printf("Buy %d for $%.2lf\n", want, result);
  }

  return 0;
}
