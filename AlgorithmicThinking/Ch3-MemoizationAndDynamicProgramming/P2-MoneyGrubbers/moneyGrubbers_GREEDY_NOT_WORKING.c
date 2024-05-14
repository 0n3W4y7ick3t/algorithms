/**
 * @author leon
 * @date Sep 27, 2023 at 17:52:44
 * @problem UVa 10980 - Lowest Price in Town
 * @tag greedy, NOT FINISHED
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1921
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int quantity;
  int bought;
  float price;
  float avgPrice;
} scheme;

int got(scheme* schemes, int schemes_num) {
  int i, got;
  got = 0;
  for (i = 0; i <= schemes_num; i++)
    got += schemes[i].bought;
  return got;
}

void optimize(scheme* scs, int scs_n, int costy, int cheap, int surplus) {
  int better_buy;
  better_buy = scs[costy].bought * scs[costy].quantity / scs[cheap].quantity;
  if (better_buy * scs[cheap].quantity >=
      scs[costy].bought * scs[costy].quantity - surplus) {
    /* we are not short, happy~ */
    scs[costy].bought = 0;
    return;
  }
}

void buy(scheme* schemes, int schemes_num, int wanted) {
  int i, j, next, surplus, try;

  /* try to optimize schemes using schemes[i] */
  for (i = 0; i <= schemes_num; i++) {
    try = wanted / schemes[i].quantity;
  }
  for (j = 0; j < next; j++)
    /* schemes[j].bought != 0 */
    if (schemes[next].avgPrice < schemes[j].avgPrice &&
        schemes[j].bought != 0) {
      /* maybe we can replace some j with next
         to get a lower cost */
      surplus = got(schemes, schemes_num) - wanted;
      if (surplus < 0)
        exit(-13);
      optimize(schemes, schemes_num, j, next, surplus);
    }
}

float costs(scheme* schemes, int schemes_num) {
  int i;
  float cost;
  cost = 0;
  for (i = 0; i <= schemes_num; i++)
    cost += schemes[i].bought * schemes[i].price;
  return cost;
}

int cmp(scheme* a, scheme* b) {
  if (a->avgPrice > b->avgPrice)
    return -1;
  else if (a->avgPrice < b->avgPrice)
    return 1;
  else
    return 0;
}

int main(void) {
  float singlePrice;
  int schemes_num;
  int i, j, case_n = 0;
  while (scanf("%f %d", &singlePrice, &schemes_num) != -1) {
    case_n++;
    printf("Case %d:\n", case_n);
    scheme schemes[schemes_num + 1];
    schemes[0].price = singlePrice;
    schemes[0].avgPrice = singlePrice;
    schemes[0].quantity = 1;
    schemes[0].bought = 0;

    for (i = 1; i <= schemes_num; i++) {
      scanf("%d %f", &schemes[i].quantity, &schemes[i].price);
      schemes[i].avgPrice = schemes[i].price / schemes[i].quantity;
      schemes[i].bought = 0;
    }

    /* sort with avgPrice */
    qsort(schemes, schemes_num + 1, sizeof(scheme),
          (int (*)(const void*, const void*))cmp);

    int wanted;
    while (scanf("%d", &wanted) != -1) {
      float cost;
      buy(schemes, schemes_num, wanted);
      cost = costs(schemes, schemes_num);
      printf("Bought %d for $%.2f\n", wanted, cost);
    }
  }
  return 0;
}
