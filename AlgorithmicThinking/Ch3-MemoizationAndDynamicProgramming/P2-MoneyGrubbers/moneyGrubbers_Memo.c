/**
 * @author leon
 * @date Oct 04, 2023 at 14:16:07
 * @problem UVa 10980 - Lowest Price in Town
 * @tag memoization
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1921
 */
#include <stdio.h>

#define MAX_SCHEMES 20
#define MAX_WANT 100
#define MAX_TRY 200

int read_number(int *number) {
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

double buy_k(double prices[], int num[], int num_schemes, double unit_price,
             int want, double memo[]) {
  /* buy exactly k */
  if (memo[want] != -1)
    return memo[want];

  if (want == 0) {
    memo[want] = 0;
    return memo[want];
  }

  double best, result;
  int i;
  /* try unit scheme */
  result = buy_k(prices, num, num_schemes, unit_price, want - 1, memo);
  best = result + unit_price;
  for (i = 0; i < num_schemes; i++) {
    if (want >= num[i]) {
      /* try other schemes, will they be better? */
      result = buy_k(prices, num, num_schemes, unit_price, want - num[i], memo);
      best = min(best, result + prices[i]);
    }
  }
  memo[want] = best;
  return memo[want];
}

double buy(double prices[], int num[], int num_schemes, double unit_price,
           int want, double memo[]) {
  /* buy_k() only seeks buying exactly k, buy() seeks more solutions */
  int try;
  double result, result_try;
  result = buy_k(prices, num, num_schemes, unit_price, want, memo);
  for (try = want + 1; try < MAX_TRY; try++) {
    /* thanks to z3, my hero */
    /* ALWAYS CHECK BOUNDRIES!!! */
    result_try = buy_k(prices, num, num_schemes, unit_price, try, memo);
    result = min(result_try, result);
  }
  return result;
}

int main(void) {
  int test_case, num_schemes, want, more_want, i;
  double unit_price, result;
  int num[MAX_SCHEMES] = {0};       /* to store the nums of schemes */
  double prices[MAX_SCHEMES] = {0}; /* to store the prices of schemes */
  double memo[MAX_TRY];

  test_case = 0;
  while (scanf("%lf%d", &unit_price, &num_schemes) != -1) {
    test_case++;
    for (i = 0; i < num_schemes; i++)
      scanf("%d%lf", &num[i], &prices[i]);
    scanf(" "); /* skip CR */
    printf("Case %d:\n", test_case);
    for (i = 0; i < MAX_TRY; i++)
      /* reset memo */
      memo[i] = -1;

    more_want = read_number(&want);
    while (more_want) {
      /* buy want of apples */
      result = buy(prices, num, num_schemes, unit_price, want, memo);
      printf("Buy %d for $%.2lf\n", want, result);
      more_want = read_number(&want);
    }
    /* left */
    result = buy(prices, num, num_schemes, unit_price, want, memo);
    printf("Buy %d for $%.2lf\n", want, result);
  }

  return 0;
}
