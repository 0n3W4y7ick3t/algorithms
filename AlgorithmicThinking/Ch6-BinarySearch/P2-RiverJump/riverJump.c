/**
 * @author leon
 * @date Jun 04, 2024 at 20:54:42
 * @tag binary search
 * @problem River Hopscotch
 * @link http://poj.org/problem?id=3258
 * @result AC, FREAKING STRANGE WHEN I CHANGED int TO size_t I GOT TLE, WHAT THE HECK POJ???
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// whether we can achieve the goal by removing n rocks
bool can_make_by_remove_n(int goal, int length, int n,
                          int n_rocks, int rocks[n_rocks]) {
  if (goal > length)
    return false;
  int current_rock, prev_rock = 0, n_removed = 0;
  for (int i = 0; i < n_rocks; ++i) {
    current_rock = rocks[i];
    if (current_rock - prev_rock < goal)
      ++n_removed; // remove current_rock
    else
      prev_rock = current_rock; // move on
  }
  // whether the last rock is too close to its right(the ending)
  if (length - prev_rock < goal)
    ++n_removed;
  return n_removed <= n;
}

void solve(int n, int length, int n_rocks, int rocks[n_rocks]) {
  // invariant built here: lo is always possible, hi is always not possible
  // this is the most likely place where BS can go wrong
  int lo = 0, hi = length + 1, mid;
  while (hi > lo + 1) {
    mid = (lo + hi) / 2;
    if (can_make_by_remove_n(mid, length, n, n_rocks, rocks))
      lo = mid;
    else
      hi = mid;
  }
  printf("%d\n", lo);
}

int cmp(void const* v1, void const* v2) {
  int x = *(int const*)v1;
  int y = *(int const*)v2;
  return x - y;
}

int main(void) {
  int length, n_rocks, n_remove;
  scanf("%d%d%d", &length, &n_rocks, &n_remove);
  int* rocks = malloc(sizeof(int[n_rocks]));
  for (int i = 0; i < n_rocks; ++i)
    scanf("%d", &rocks[i]);
  qsort(rocks, n_rocks, sizeof(int), cmp);
  solve(n_remove, length, n_rocks, rocks);
  free(rocks);
  return EXIT_SUCCESS;
}
