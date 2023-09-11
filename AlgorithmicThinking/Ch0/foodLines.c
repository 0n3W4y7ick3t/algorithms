// https://dmoj.ca/problem/lkp18c2p1
#include <stdio.h>
#define _MAX_LINES_ 100

int min_index(int lines[], int len) {
  int index = 0;
  for (int k=0; k<len; k++)
    if (lines[k] < lines[index])
      index = k;
  return index;
}


void solve(int lines[], int m, int n) {
  // m is the length of lines[],
  // n is the number of person to come
  int minIndex;
  for(int i=0; i<n; i++) {
    minIndex = min_index(lines, m);
    printf("%d\n", lines[minIndex]);
    lines[minIndex]++;
  }
}

int main() {
  int m, n;
  int lines[_MAX_LINES_];
  scanf("%d%d", &m, &n);
  for(int i=0; i<m; i++)
    scanf("%d", &lines[i]);
  solve(lines, m, n);
  return 0;
}

