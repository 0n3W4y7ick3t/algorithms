/**
 * @author leon
 * @date Sep 12, 2023 at 18:39:46
 * @link https://codeforces.com/problemset/problem/39/J
 * FAILED test 16, didn't figure out where the problem is
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000000

int prefix_len(char s1[], char s2[]) {
  int i = 1;
  while (s1[i] == s2[i])
    i++;
  return i - 1;
}

int suffix_len(char s1[], char s2[], int len) {
  /* s1 > s2, len of s1 */
  int i = len;
  while (i >= 2 && s1[i] == s2[i - 1])
    i--;
  return len - i;
}

int main(void) {
  static char s1[SIZE + 2], s2[SIZE + 2];
  int len, prefix, suffix, total;
  fgets(&s1[1], SIZE + 1, stdin);
  s1[strcspn(s1, "\n")] = '\0';
  fgets(&s2[1], SIZE + 1, stdin);
  s2[strcspn(s2, "\n")] = '\0';

  len = strlen(&s1[1]);
  prefix = prefix_len(s1, s2);
  suffix = suffix_len(s1, s2, len);
  total = prefix + 1 - (len - suffix) + 1;

  if (total < 0)
    total = 0;

  /* printf("%d%d%d%d\n", len, prefix, suffix, total); */
  printf("%d\n", total);
  int i;
  for (i = 0; i < total; i++) {
    printf("%d", i + len - suffix);
    if (i < total + 1)
      printf(" ");
    else
      printf("\n");
  }
  return 0;
}
