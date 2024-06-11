/**
 * @author leon
 * @date Jun 11, 2024 at 22:26:19
 * @tag union find
 * @problem SPOJ KGSS - Maximum Sum
 * @link https://www.spoj.com/problems/KGSS/
 * @result AC
 */
#include <stdio.h>
#include <stdlib.h>

#define BIGGER(x, y) ((x) > (y) ? (x) : (y))

typedef struct SegNode {
  size_t left, right;
  size_t max, sum;
} SegNode;

typedef struct NodeInfo {
  size_t max, sum;
} NodeInfo;

void initSegTree(SegNode* tree, size_t index, size_t left, size_t right,
                 size_t* leaves) {
  tree[index].left = left;
  tree[index].right = right;
  if (left == right) {
    // use leaves to store the index in segtree of a leaf node
    leaves[left] = index;
    return;
  }

  size_t mid = (left + right) / 2;
  initSegTree(tree, 2 * index, left, mid, leaves);
  initSegTree(tree, 2 * index + 1, mid + 1, right, leaves);
}

// build segment tree recursively (store the range results)
NodeInfo buildSegTree(SegNode* tree, size_t index, size_t* numbers) {
  // base case
  if (tree[index].left == tree[index].right) {
    tree[index].max = numbers[tree[index].left];
    // indication of not possible
    return (NodeInfo){.max = tree[index].max, .sum = 0};
  }

  // chose from left_max + right_max, right_sum, left_sum
  NodeInfo left_info = buildSegTree(tree, 2 * index, numbers);
  NodeInfo right_info = buildSegTree(tree, 2 * index + 1, numbers);

  // parent's update should be done AFTER it's children
  tree[index].max = BIGGER(left_info.max, right_info.max);
  tree[index].sum = BIGGER(left_info.max + right_info.max,
                           BIGGER(left_info.sum, right_info.sum));
  return (NodeInfo){.max = tree[index].max, .sum = tree[index].sum};
}

void update(SegNode* tree, size_t index, size_t val) {
  tree[index].max = val;
  index = index >> 1;
  // update parents
  while (index) {
    tree[index].max = BIGGER(tree[2 * index].max, tree[2 * index + 1].max);
    tree[index].sum =
        BIGGER(tree[2 * index].max + tree[2 * index + 1].max,
               BIGGER(tree[2 * index].sum, tree[2 * index + 1].sum));
    index = index >> 1;
  }
}

NodeInfo query(SegNode* tree, size_t index, size_t left, size_t right) {
  if (right < tree[index].left || left > tree[index].right)
    // no overlap, cannot be used
    // if using 0 again here, we can't tell
    // if it's because the boundries are not matched,
    // or it's a leaf node, where we used 0
    // when building the segtree to indicate so.
    return (NodeInfo){.max = -1, .sum = -1};

  if (left <= tree[index].left && right >= tree[index].right)
    // fills all
    return (NodeInfo){.max = tree[index].max, .sum = tree[index].sum};

  NodeInfo left_info = query(tree, 2 * index, left, right);
  NodeInfo right_info = query(tree, 2 * index + 1, left, right);

  // it doesnt match the child's boundries,
  // we should not use its max, just return another's sum
  if (left_info.max == -1)
    return right_info;
  if (right_info.max == -1)
    return left_info;

  // should chose from sub queries, NOT from trees
  // (tree nodes are for fully matched ranges)
  return (NodeInfo){.max = BIGGER(left_info.max, right_info.max),
                    .sum = BIGGER(left_info.max + right_info.max,
                                  BIGGER(left_info.sum, right_info.sum))};
}

int main(void) {
  size_t n, ops;
  scanf("%zu", &n);
  size_t* numbers = malloc(sizeof(size_t[n + 1]));
  for (size_t i = 1; i <= n; ++i)
    scanf("%zu", &numbers[i]);

  // build the segment tree
  SegNode* segtree = malloc(sizeof(SegNode[4 * n + 1]));
  size_t* leaves = malloc(sizeof(size_t[n + 1]));
  initSegTree(segtree, 1, 1, n, leaves);
  buildSegTree(segtree, 1, numbers);
  free(numbers);

  scanf("%zu", &ops);
  char op;
  size_t a, b;
  for (size_t i = 1; i <= ops; ++i) {
    scanf(" %c", &op);
    scanf("%zu%zu", &a, &b);
    switch (op) {
    case 'U':
      // leaves[a] is the index of node a in segtree
      update(segtree, leaves[a], b);
      break;
    case 'Q':
      printf("%zu\n", query(segtree, 1, a, b).sum);
      break;
    }
  }
  free(leaves);
  free(segtree);
  return EXIT_SUCCESS;
}
