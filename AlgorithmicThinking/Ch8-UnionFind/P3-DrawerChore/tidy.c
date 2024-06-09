/**
 * @author leon
 * @date Jun 09, 2024 at 15:01:26
 * @tag union find
 * @problem Kattis - ladice
 * @link https://open.kattis.com/problems/ladice
 * @result AC
 */
#include <stdio.h>
#include <stdlib.h>

#define DRAWER() printf("LADICA\n")
#define TRASH() printf("SMECE\n")

size_t find(size_t* drawers, size_t x) {
  size_t root = x;
  while (drawers[root] != root)
    root = drawers[root];

  size_t temp = x;
  while (drawers[x] != root) {
    temp = drawers[x];
    drawers[x] = root;
    x = temp;
  }
  return root;
}

// we put drawers with interchangable items together into one set,
// 3 5 7 means 3 and 5 are filled, and 7 is available
// 2 4 0 means 2 and 4 are filled(0 is for indication)
// 7 and 0 are representive of their sets
void union_sets(size_t* drawers, size_t a, size_t b) {
  size_t root_a = find(drawers, a);
  size_t root_b = find(drawers, b);
  if (root_a == root_b) {
    // in the same set

    if (root_a != 0) {
      // put it in the end and append 0,
      // indicates there is no room for this set
      DRAWER();
      drawers[root_a] = 0;
    } else
      // 3 1 2 0 means 3 1 2 are all filled
      // so an item 1 3 can not be filled
      TRASH();

  } else {
    // in diff sets

    if (root_a != 0) {
      // the first set can be filled
      // union set a into b
      drawers[root_a] = root_b;
      DRAWER();
    } else {
      if (root_b != 0) {
        // room left for b
        drawers[root_b] = 0;
        DRAWER();
      } else
        TRASH();
    }
  }
}

int main(void) {
  // number of items, drawers
  size_t n, l;
  scanf("%zu%zu", &n, &l);
  // the union find set for drawers
  size_t* drawers = malloc(sizeof(size_t[l + 1]));
  for (size_t i = 0; i <= l; ++i)
    drawers[i] = i;

  size_t d1, d2;
  for (size_t i = 0; i < n; ++i) {
    scanf("%zu%zu", &d1, &d2);
    union_sets(drawers, d1, d2);
  }

  return EXIT_SUCCESS;
}
