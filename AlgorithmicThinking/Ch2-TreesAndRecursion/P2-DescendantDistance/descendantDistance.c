/**
 * @author leon
 * @created Sep 21, 2023 at 11:09:45
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

typedef struct node
{
  char *name;
  struct node **children;
  struct node *next; // used in hash table
  int num_children;
  int score; /* num of decendants ar certain depth */
} node;

typedef int (*Sorter)(node **, int, int);

void *safe_malloc(int size)
{
  char *mem = malloc(size);
  if (!mem)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  return mem;
}

node *new_node(char *name)
{
  node *n = safe_malloc(sizeof(node));
  n->name = name;
  n->num_children = 0;
  n->score = 0;
  return n;
}

unsigned long oaat(char *key, unsigned long len, unsigned long bits)
{
  /* the hash function */
  unsigned long hash, i;
  for (hash = 0, i = 0; i < len; i++)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash & hashmask(bits);
}

node *search_hash_table(node *hash_table[], char *find, unsigned find_len)
{
  unsigned word_hash = oaat(find, find_len, NUM_BITS);
  node *wordptr = hash_table[word_hash];
  while (wordptr)
  {
    if (strlen(wordptr->name) == find_len &&
        strncmp((wordptr->name), find, find_len) == 0)
      return wordptr;
    wordptr = wordptr->next;
  }
  /* when wordptr is null leaves while loop */
  return NULL;
}

void count_descendant(node *p, int depth, int *cnt)
{
  // set p->score at depth
  if (depth == 1)
  {
    *cnt += p->num_children;
    return;
  }
  // go down a level and call this again on children
  int i;
  for (i = 0; i < p->num_children; i++)
    count_descendant(p->children[i], depth - 1, cnt);
}

int sorter_score(node *nodes[], int i, int j)
{
  return nodes[i]->score < nodes[j]->score;
}

int sorter_name(node *nodes[], int i, int j)
{
  printf("comparing from %d to %d\n", i, j);
  return strcmp(nodes[i]->name, nodes[j]->name);
}

// sort nodes in place from index i to j using sorter function
// void sort(node *nodes[], int i, int j, int(*sorter)(node**, int, int))
void sort(node *nodes[], int i, int j, Sorter sorter)
{
  int a, b;
  node *temp;
  for (a = i; a < j; a++)
  {
    for (b = a; b < j; b++)
    {
      if (sorter(nodes, a, b))
      {
        temp = nodes[a];
        nodes[a] = nodes[b];
        nodes[b] = temp;
      }
    }
  }
}

void solve_tree(node *hash_table[], node **parents, int num_parents, int depth)
{
  int pos;
  for (pos = 0; pos < num_parents; pos++)
  {
    int cnt = 0;
    count_descendant(parents[pos], depth, &cnt);
    parents[pos]->score = cnt;
  }

  // sort parents by parents->score: high -> low
  sort(parents, 0, num_parents, sorter_score);

  // sort those have same score by name
  int i = 0, j = 0;
  while (1)
  {
    while (parents[j]->score == parents[i]->score)
    {
      j++;
      if (j == num_parents)
        break;
    }

    // int a=i, b=j;
    // for (i = a; i < b; i++)
    // {
    //   for (j = i; j < b; j++)
    //   {
    //     if (parents[i]->name > parents[j]->name)
    //     {
    //       temp = parents[i];
    //       parents[i] = parents[j];
    //       parents[j] = temp;
    //     }
    //   }
    // }
    // now [i, j) are with the same score.
    sort(parents, i, j, sorter_name);
    // process next segmant
    i = j;
    if (i == num_parents)
      break;
    // j = i + 1;
  }

  pos = 0;
  int last_score = parents[pos]->score, cur_score, total = 0;
  while (pos < num_parents)
  {
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

char *read_line(int size)
{
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (!str)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != '\n'))
  {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if (len == size)
    {
      size = size * 2;
      str = realloc(str, size);
      if (!str)
      {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

int main(void)
{
  int trees;
  scanf("%d", &trees);
  getchar();
  int i;

  for (i = 0; i < trees; i++)
  {
    int j, c;
    /* hash table to look up nodes */
    node *hash_table[1 << NUM_BITS] = {NULL};
    int num_nodes, depth;
    int pos = 0;
    scanf("%d %d", &num_nodes, &depth);
    getchar();
    node **parents = safe_malloc(sizeof(node *) * num_nodes);
    char *line;
    for (j = 1; j <= num_nodes; j++)
    {
      /* read each line */
      line = read_line(MAXNAME);
      // split line into words by space
      char *word = strtok(line, " ");
      if (!word)
      {
        fprintf(stderr, "error: cant split line by space\n");
        exit(1);
      }
      node *parent = search_hash_table(hash_table, word, strlen(word));
      if (!parent)
      {
        long parent_hash = oaat(word, strlen(word), NUM_BITS);
        parent = new_node(word);
        parent->next = hash_table[parent_hash];
        hash_table[parent_hash] = parent;
        parents[pos++] = parent;
      }

      char *num_of_children = strtok(NULL, " ");
      parent->num_children = atoi(num_of_children);
      parent->children = safe_malloc(sizeof(node *) * parent->num_children);

      // start reading children
      node *child;
      int c = 0;
      word = strtok(NULL, " ");
      while (word)
      {
        child = search_hash_table(hash_table, word, strlen(word));
        if (!child)
        {
          // new child, add child to hash table
          long child_hash = oaat(word, strlen(word), NUM_BITS);
          child = new_node(word);
          child->next = hash_table[child_hash];
          hash_table[child_hash] = child;
        }
        // now child point to the correct child node
        parent->children[c++] = child;
        word = strtok(NULL, " ");
      }
    }

    printf("Tree %d:\n", i + 1);
    solve_tree(hash_table, parents, pos, depth);
    if (i < trees)
      printf("\n");
    // int f, p;
    // for(f = 0; f < pos; f++){
    //   // free parent
    //   for (p = 0; p < parents[f]->num_children; p++){
    //     free(parents[f]->children[p]);
    //     parents[f]->children[p] = 0;
    //   }
    //   free(parents[f]);
    //   parents[f] = 0;
    // }
    // free(parents);
    // parents = 0;
    // free(line);
    // line = 0;
  }
  return 0;
}
