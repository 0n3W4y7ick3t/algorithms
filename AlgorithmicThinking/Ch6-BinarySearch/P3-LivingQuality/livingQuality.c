/**
 * @author leon
 * @date Jun 05, 2024 at 21:01:07
 * @tag binary search, memo, find median
 * @problem IOI '10 P3 - Quality of Living
 * @link https://dmoj.ca/problem/ioi10p3
 * @result AC
 *
 * This is a great problem, the using of BS, and the range sum query speed up
 */

// this is not an acm style problem, we are only required to implement a
// function, whose signature is
// int rectangle(int r, int c, int h, int w, int q[3001][3001])

#include <stdbool.h>
#include <stdlib.h>

// whether we can find a (h, w) sized matrix, whose median is lesser than m
bool can_find_less_than(int m, int r, int c, int h, int w, int q[3001][3001]) {
  // set [0][x] and [x][0] all to zero for convenience
  static int range_sums[3001][3001] = {0};
  for (size_t i = 1; i < r + 1; ++i)
    for (size_t j = 1; j < c + 1; ++j) {
      // range_sums[x][y] is the sum of matrix (0, 0) to (x, y)
      // index in range_sums start with 0

      // others
      range_sums[i][j] = range_sums[i - 1][j] + range_sums[i][j - 1] -
                         range_sums[i - 1][j - 1];
      // current one:
      if (q[i - 1][j - 1] < m + 1)
        --range_sums[i][j]; // treat as -1 if it's smaller than m
      else
        ++range_sums[i][j]; // 1
    }

  // test each matrix
  int range_sum;
  size_t top, left, bottom, right;
  for (top = 0; top < r - h + 1; ++top)
    for (left = 0; left < c - w + 1; ++left) {
      bottom = top + h;
      right = left + w;
      // negative values indicate there are more numbers lesser than/equal to m,
      // so the median is actually smaller than m, so m is feasible.
      // (we can find a median <= m in current matrix)
      // otherwise, keep trying the rest matrix.
      range_sum = range_sums[bottom][right] + range_sums[top][left] -
                  range_sums[bottom][left] - range_sums[top][right];
      if (range_sum <= 0)
        return true;
    }
  return false;
}

// returns the min median of all (h, w) sized matrix in the (r, c) sized matrix
int rectangle(int r, int c, int h, int w, int q[3001][3001]) {
  // lo is infeasible, hi is feasible
  int lo = 0, hi = r * c + 1, mid;
  while (hi - lo > 1) {
    mid = (lo + hi) >> 1;
    if (can_find_less_than(mid, r, c, h, w, q))
      hi = mid;
    else
      lo = mid;
  }
  return hi;
}
