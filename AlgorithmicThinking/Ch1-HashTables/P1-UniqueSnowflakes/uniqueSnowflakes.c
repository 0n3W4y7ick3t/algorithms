/**
 * @author leon
 * @date Sep 11, 2023 at 15:56:28
 * @link https://dmoj.ca/problem/cco07p2
 */
#include<stdio.h>
#include<stdlib.h>

#define WINGS_OF_SNOWFLAKE 6
#define MAX_NUM_OF_SNOWFLAKE 100000

typedef struct snowflake_node {
  int snow_flake[WINGS_OF_SNOWFLAKE];
  struct snowflake_node* next;
}snowflake_node;

int encode_snow(snowflake_node* node) {
  int sum = 0;
  int i;
  for(i = 0; i < WINGS_OF_SNOWFLAKE; i++){
    sum+= node->snow_flake[i];
  }
  return sum % MAX_NUM_OF_SNOWFLAKE;
}

int identical_backwards(int snow1[], int snow2[], int offset) {
  int i;
  for (i = 0; i < WINGS_OF_SNOWFLAKE; i++){
    /* comapre backwards */
    if (snow1[i] != snow2[(WINGS_OF_SNOWFLAKE - i + offset) % WINGS_OF_SNOWFLAKE]) {
      return 0;
      break;
    }
  }
  return 1;
}

int identical_forwards(int snow1[], int snow2[], int offset) {
  int i;
  for (i = 0; i < WINGS_OF_SNOWFLAKE; i++){
    /* comapre forwards */
    if (snow1[i] != snow2[(i + offset) % WINGS_OF_SNOWFLAKE]) {
      return 0;
      break;
    }
  }
  return 1;
}

int are_identical(int snow1[], int snow2[]) {
  int offset;
  for (offset = 0; offset < WINGS_OF_SNOWFLAKE; offset++) {
    if (identical_backwards(snow1, snow2, offset))
      return 1;
    if (identical_forwards(snow1, snow2, offset))
      return 1;
  }
  return 0;
}

void solve(snowflake_node *snows[]) {
  snowflake_node *snow1, *snow2;
  for (int i=0; i < MAX_NUM_OF_SNOWFLAKE; i++) {
    /* compare all snows in the same link list */
    snow1 = snows[i];
    while(snow1 != NULL) {
      snow2 = snow1 -> next;
      while(snow2 != NULL) {
        if (are_identical(snow1->snow_flake, snow2-> snow_flake)) {
          printf("Twin snowflakes found.\n");
          return;
        }
        snow2 = snow2->next;
      }
      snow1 = snow1->next;
    }
  }
  printf("No two snowflakes are alike.\n");
}

int main(void) {
  int n, code;
  scanf("%d", &n);
  static snowflake_node *snowflakes[MAX_NUM_OF_SNOWFLAKE] = {NULL};
  snowflake_node *snow;

  for (int i=0; i < n; i++) {
    snow = malloc(sizeof(snowflake_node));
    if(snow == NULL) {
      fprintf(stderr, "malloc err!\n");
      exit(1);
    }
    for (int k=0; k < WINGS_OF_SNOWFLAKE; k++)
      scanf("%d", &snow->snow_flake[k]);
    /* store this snow to array */
    code = encode_snow(snow);
    snow->next = snowflakes[code];
    snowflakes[code] = snow;
  }

  solve(snowflakes);
  return 0;
}

