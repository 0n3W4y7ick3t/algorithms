/**
 * @author leon
 * @date Sep 12, 2023 at 14:43:59
 * @link https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=15&page=show_problem&problem=1332
 * UVa problem 10391
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUM_BITS 17 /* bits size of hash */
#define WORD_LENGTH 16
#define hashsize(n) ((unsigned long)1 << n)
#define hashmask(n) (hashsize(n) - 1) /* 100 -> 011 */

typedef struct word_node {
  char **word;
  struct word_node *next;
} word_node;

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

int in_hash_table(word_node *hash_table[], char *find, unsigned find_len) {
  unsigned word_hash;
  word_node *wordptr;
  word_hash = oaat(find, find_len, NUM_BITS);
  wordptr = hash_table[word_hash];
  while(wordptr) {
    if ( strlen(*(wordptr->word)) == find_len &&
        strncmp(*(wordptr->word), find, find_len) == 0)
      return 1;
    wordptr = wordptr->next;
  }
  /* when wordptr is null leaves while loop */
  return 0;
}

char* read_line(int size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (str == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while( (ch = getchar()) != EOF && (ch != '\n') ) {
    str[len++] = ch;
    /* len inc, check if size is too small */
    if(len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (str == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

void solve(char *words[], word_node *hash_table[], int total_words) {
  int i, j;
  for (i=0; i < total_words; i++) {
    /* split every word, see if both parts can be found in hash_table */
    int len = strlen(words[i]);
    for (j=0; j < len; j++) {
      /* by passing 3rd param to in_hash_table(), we are limit the range of cstring to be check on
         start at words[i], for a length of j
         start ar words[i][j], for a length of (len - j) */
      if (in_hash_table(hash_table, words[i], j) &&
          in_hash_table(hash_table, &words[i][j], len -j )) {
        /* both found */
        printf("%s\n", words[i]);
        break; /* no need to check words[i] for the rest splits */
      }
    }
  }
}

int main(void) {
  static char *words[ 1 << NUM_BITS ] = {NULL};
  static word_node *hash_table[1 << NUM_BITS] = {NULL};
  int total_words = 0;
  char *word;
  word_node *wordptr;
  unsigned length, word_hash;

  word = read_line(WORD_LENGTH);
  while (*word) {
    /* if read a word */
    words[total_words] = word;
    wordptr = malloc(sizeof(word_node));
    if (wordptr == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }
    length = strlen(word);
    word_hash = oaat(word, length, NUM_BITS);
    wordptr->word = &words[total_words];
    /* TODO: if change last line to
       wordptr->word = &word;
       why this failed to store word in hash_table
       table[somehash] is a one node link list,
       whose `word` is empty.
      */
    wordptr->next = hash_table[word_hash];
    hash_table[word_hash] = wordptr;
    total_words++;

    word = read_line(WORD_LENGTH); /* read next one */
  }

  solve(words, hash_table, total_words);
  return 0;
}

