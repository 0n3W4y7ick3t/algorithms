#include <stdio.h>
#include <stdlib.h>

char* read_word(int size) {
  char* str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (!str) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != '\n') && (ch != ' ') &&
         (ch != '\t')) {
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
