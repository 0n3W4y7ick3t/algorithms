/**
 * @author leon
 * @date Jun 08, 2024 at 11:17:16
 * @tag union find
 * @problem UVa problem 10158 - War
 * @link
 * https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=13&page=show_problem&problem=1099
 * @result AC
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t find(size_t* friends, size_t x) {
  size_t root = x;
  while (friends[root] != root)
    root = friends[root];

  /* path compression */
  size_t temp;
  while (friends[x] != root) {
    temp = friends[x];
    friends[x] = root;
    x = temp;
  }
  return root;
}

size_t union_set(size_t* friends, size_t* sizes, size_t x, size_t y) {
  size_t root_x = find(friends, x);
  size_t root_y = find(friends, y);
  if (root_x == root_y)
    return root_x;

  if (sizes[root_x] > sizes[root_y]) {
    friends[root_y] = root_x;
    sizes[root_x] += sizes[root_y];
    return root_x;
  } else {
    friends[root_x] = root_y;
    sizes[root_y] += sizes[root_x];
    return root_y;
  }
}

bool are_friends(size_t* friends, size_t x, size_t y) {
  return find(friends, x) == find(friends, y);
}

bool are_enemies(size_t* friends, size_t x, size_t y, size_t* enemies) {
  /* if y's friends reprentative reprentative of x' enemies? */
  size_t root_x = find(friends, x);
  if (enemies[root_x] == -1)
    return false;
  return find(friends, enemies[root_x]) == find(friends, y);
}

void set_friends(size_t* friends, size_t* sizes, size_t x, size_t y,
                 size_t* enemies) {
  size_t root_x = find(friends, x);
  size_t root_y = find(friends, y);
  size_t root = union_set(friends, sizes, x, y);

  /* union common enemies */
  if (enemies[root_y] != -1 && enemies[root_x] != -1)
    union_set(friends, sizes, enemies[root_y], enemies[root_x]);

  /* x or y had enemies, set it to the new united set */
  /* this is easy to miss */
  if (enemies[root] == -1)
    enemies[root] = enemies[root == root_x ? root_y : root_x];
}

void set_enemies(size_t* friends, size_t* sizes, size_t x, size_t y,
                 size_t* enemies) {
  size_t root_x = find(friends, x);
  size_t root_y = find(friends, y);
  if (enemies[root_x] != -1)
    union_set(friends, sizes, enemies[root_x], y);
  else
    enemies[root_x] = root_y;

  if (enemies[root_y] != -1)
    union_set(friends, sizes, enemies[root_y], x);
  else
    enemies[root_y] = root_x;
}

int main(void) {
  size_t node_n, op, x, y;
  scanf("%zu", &node_n);

  /* friends */
  size_t* friends = malloc(sizeof(size_t[node_n]));
  size_t* sizes = malloc(sizeof(size_t[node_n]));
  size_t i;
  for (i = 0; i < node_n; ++i) {
    friends[i] = i;
    sizes[i] = 1;
  }
  /* store one enemy(others can be identidied by its reprentative) */
  size_t* enemies = malloc(sizeof(size_t[node_n]));
  memset(enemies, -1, sizeof(size_t[node_n]));

  scanf("%zu%zu%zu", &op, &x, &y);
  /* stop when op is 0 */
  while (op) {
    switch (op) {
    case 1:
      if (are_enemies(friends, x, y, enemies))
        printf("-1\n");
      else
      set_friends(friends, sizes, x, y, enemies);
      break;
    case 2:
      if (are_friends(friends, x, y ))
        printf("-1\n");
      else
        set_enemies(friends, sizes, x, y, enemies);
      break;
    case 3:
      if (are_friends(friends, x, y))
        printf("1\n");
      else
        printf("0\n");
      break;
    case 4:
      if (are_enemies(friends, x, y, enemies))
        printf("1\n");
      else
        printf("0\n");
      break;
    }
    scanf("%zu%zu%zu", &op, &x, &y);
  }
  return EXIT_SUCCESS;
}
