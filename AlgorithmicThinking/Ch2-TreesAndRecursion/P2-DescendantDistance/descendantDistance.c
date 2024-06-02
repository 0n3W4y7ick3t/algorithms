/**
 * @author leon
 * @date Sep 21, 2023 at 21:09:45
 * @tag trees
 * @problem ICPC ECNA 2005 B - Countdown
 * @link https://dmoj.ca/problem/ecna05b
 * @result WA on test case #1, 0/100
 * diffrent output with local one (same test case)
 * cant figure out whats wrong
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXNODES 1000
#define MAXNAME 10
#define NUM_BITS 8 /* bits size of hash */
#define RANKING 3
#define hashsize(n) ((unsigned long)1 << n)
#define hashmask(n) (hashsize(n) - 1) /* 100 -> 011 */

typedef struct node {
  char* name;
  struct node** children;
  struct node* next; // used in hash table
  int num_children;
  int score; /* num of decendants ar certain depth */
} node;

node* new_node(char* name) {
  node* n = malloc(sizeof(node));
  n->name = name;
  n->num_children = 0;
  n->score = 0;
  return n;
}

unsigned long oaat(char* key, unsigned long len, unsigned long bits) {
  /* the hash function */
  unsigned long hash, i;
  for (hash = 0, i = 0; i < len; i++) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash & hashmask(bits);
}

node* search_hash_table(node* hash_table[], char* find) {
  int find_len = strlen(find);
  unsigned word_hash = oaat(find, find_len, NUM_BITS);
  node* wordptr = hash_table[word_hash];
  while (wordptr) {
    if (strlen(wordptr->name) == find_len &&
        strncmp((wordptr->name), find, find_len) == 0)
      return wordptr;
    wordptr = wordptr->next;
  }
  /* when wordptr is null leaves while loop */
  return NULL;
}

int count_descendants(node* p, int depth) {
  // set p->score at depth
  int total = 0;
  if (depth == 1)
    return p->num_children;

  // go down a level and call this again on children
  for (int i = 0; i < p->num_children; i++)
    total += count_descendants(p->children[i], depth - 1);
  return total;
}

int qcompare(const void* v1, const void* v2) {
  const node* n1 = *(const node**)v1;
  const node* n2 = *(const node**)v2;
  if (n1->score == n2->score)
    return strcmp(n1->name, n2->name);
  return n2->score - n1->score;
}

void score_tree(node** nodes, int num_nodes, int depth) {
  int pos;
  for (pos = 0; pos < num_nodes; pos++)
    nodes[pos]->score = count_descendants(nodes[pos], depth);
}

void print_nodes(node** nodes, int num_nodes) {

  int pos = 0;
  int last_score = nodes[pos]->score, cur_score, total = 0;
  while (pos < num_nodes) {
    cur_score = nodes[pos]->score;
    // dont print those have zero descendant
    if (cur_score == 0)
      break;

    // droped a ranking down && printed enough
    if (cur_score < last_score && total == RANKING)
      break;

    printf("%s %d\n", nodes[pos]->name, cur_score);
    last_score = cur_score;
    total++;
    pos++;
  }
}

int read_tree(node** nodes, int num_lines) {
  int n = 0;
  /* hash table to look up nodes */
  node* hash_table[1 << NUM_BITS] = {NULL};
  char *parent_name, *child_name;
  node *parent, *child;
  int num_children, hash;
  for (int i = 0; i < num_lines; i++) {
    // 1. read parent
    parent_name = malloc(MAXNAME + 1);
    scanf("%s", parent_name);
    // 2. read num of children
    scanf("%d", &num_children);

    parent = search_hash_table(hash_table, parent_name);
    if (!parent) {
      // new parent and store it into hash_table
      parent = new_node(parent_name);
      hash = oaat(parent_name, strlen(parent_name), NUM_BITS);
      parent->next = hash_table[hash];
      hash_table[hash] = parent;
      nodes[n++] = parent;
    }
    parent->num_children = num_children;
    parent->children = malloc(sizeof(node*) * num_children);

    // 3. read children
    for (int j = 0; j < num_children; j++) {
      child_name = malloc(MAXNAME + 1);
      scanf("%s", child_name);

      child = search_hash_table(hash_table, child_name);
      if (!child) {
        // new child and store it into hash_table
        child = new_node(child_name);
        hash = oaat(child_name, strlen(child_name), NUM_BITS);
        child->next = hash_table[hash];
        hash_table[hash] = child;
        nodes[n++] = child;
      }
      parent->children[j] = child;
    }
  }
  return n;
}

int main(void) {
  int trees;
  scanf("%d", &trees);

  for (int i = 0; i < trees; i++) {
    int num_lines, depth;
    scanf("%d %d", &num_lines, &depth);
    node** nodes = malloc(sizeof(node*) * MAXNODES);
    int num_nodes = read_tree(nodes, num_lines);

    printf("Tree %d:\n", i + 1);

    score_tree(nodes, num_nodes, depth);
    qsort(nodes, num_nodes, sizeof(node*), qcompare);
    print_nodes(nodes, num_nodes);

    if (i < trees)
      printf("\n");

    // free nodes here
    for (int j = 0; j < num_nodes; j++)
      free(nodes[j]);
  }
  return 0;
}
