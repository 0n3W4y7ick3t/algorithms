/**
 * @author leon
 * @date Sep 21, 2023 at 11:09:45
 * @link https://dmoj.ca/problem/ecna05b
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
  char *name;
  struct node **children;
  struct node *next; // used in hash table
  int num_children;
  int score; /* num of decendants ar certain depth */
} node;

void *safe_malloc(int size) {
  char *mem = malloc(size);
  if (!mem) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  return mem;
}

node *new_node(char *name) {
  node *n = safe_malloc(sizeof(node));
  n->name = name;
  n->num_children = 0;
  n->score = 0;
  return n;
}

unsigned long oaat(char *key, unsigned long len, unsigned long bits) {
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

node *search_hash_table(node *hash_table[], char *find, unsigned find_len) {
  unsigned word_hash = oaat(find, find_len, NUM_BITS);
  node *wordptr = hash_table[word_hash];
  while (wordptr) {
    if (strlen(wordptr->name) == find_len &&
        strncmp((wordptr->name), find, find_len) == 0)
      return wordptr;
    wordptr = wordptr->next;
  }
  /* when wordptr is null leaves while loop */
  return NULL;
}

int count_descendants(node *p, int depth) {
  // set p->score at depth
  int total = 0;
  if (depth == 1)
    return p->num_children;

  // go down a level and call this again on children
  for (int i = 0; i < p->num_children; i++)
    total += count_descendants(p->children[i], depth - 1);
  return total;
}

int qcompare(const void* v1, const void* v2){
  const node* n1 = *(const node**)v1;
  const node* n2 = *(const node**)v2;
  if (n1->score == n2->score)
    return strcmp(n1->name, n2->name);
  return n2->score - n1->score;
}
//  qsort(parents, num_parents, sizeof(node*), qcompare);

void printNodes(node **nodes, int len, char *header) {
  printf("[DEBUG](%s) => ", header);
  for (int i = 0; i < len; i++)
    printf("[%d]%s(%d) ", i, nodes[i]->name, nodes[i]->score);
  printf("\n");
}

void solve_tree(node *hash_table[], node **parents, int num_parents,
                int depth) {
  int pos;
  for (pos = 0; pos < num_parents; pos++)
    parents[pos]->score = count_descendants(parents[pos], depth);

  // sort parents by parents->score: high -> low
  // sort(parents, 0, num_parents, compare);
  qsort(parents, num_parents, sizeof(node*), qcompare);

  pos = 0;
  int last_score = parents[pos]->score, cur_score, total = 0;
  while (pos < num_parents) {
    cur_score = parents[pos]->score;
    // dont print those have zero descendant
    if (cur_score == 0)
      break;

    // droped a ranking down && printed enough
    if (cur_score < last_score && total == RANKING)
      break;

    printf("%s %d\n", parents[pos]->name, cur_score);
    last_score = cur_score;
    total++;
    pos++;
  }
}

char *read_line(int size) {
  char *str;
  int ch;
  int len = 0;
  str = safe_malloc(size);
  while ((ch = getchar()) != EOF && (ch != '\n')) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (!str) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

int main(void) {
  int trees;
  scanf("%d", &trees);
  getchar();

  for (int i = 0; i < trees; i++) {
    /* hash table to look up nodes */
    node *hash_table[1 << NUM_BITS] = {NULL};
    int num_nodes, depth;
    int pos = 0;
    scanf("%d %d", &num_nodes, &depth);
    getchar();
    node **parents = safe_malloc(sizeof(node *) * num_nodes);
    char *line;
    for (int j = 1; j <= num_nodes; j++) {
      /* read each line */
      line = read_line(MAXNAME);
      // split line into words by space
      char *word = strtok(line, " ");
      node *parent = search_hash_table(hash_table, word, strlen(word));
      if (!parent) {
        long parent_hash = oaat(word, strlen(word), NUM_BITS);
        parent = new_node(word);
        parent->next = hash_table[parent_hash];
        hash_table[parent_hash] = parent;
      }
      // shoudl add to parents whether in hashtable or not
      parents[pos++] = parent;

      char *num_of_children = strtok(NULL, " ");
      parent->num_children = atoi(num_of_children);
      parent->children = safe_malloc(sizeof(node *) * parent->num_children);

      // start reading children
      node *child;
      int c = 0;
      word = strtok(NULL, " ");
      while (word) {
        child = search_hash_table(hash_table, word, strlen(word));
        if (!child) {
          // new child, add child to hash table
          long child_hash = oaat(word, strlen(word), NUM_BITS);
          child = new_node(word);
          child->next = hash_table[child_hash];
          hash_table[child_hash] = child;
        }
        // now child point to the correct child node
        parent->children[c++] = child;
        // process next word(child)
        word = strtok(NULL, " ");
      }
    }

    printf("Tree %d:\n", i + 1);

    // printNodes(parents, pos, "before");
    solve_tree(hash_table, parents, pos, depth);
    // printNodes(parents, pos, "final");

    if (i < trees)
      printf("\n");
  }
  return 0;
}
