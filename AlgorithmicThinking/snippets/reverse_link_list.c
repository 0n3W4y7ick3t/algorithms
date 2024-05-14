#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int v;
  struct Node* next;
} Node;

Node* reverse(Node* head) {
  Node* prev = NULL;
  Node* cur = head;
  Node* next = NULL;
  while (cur) {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  return prev;
}

void print_list(Node* head) {
  Node* n;
  n = head;
  while (n) {
    printf("%d\n", n->v);
    n = n->next;
  }
}

void insert(Node** head_ref, int i) {
  Node* n = malloc(sizeof(Node));
  n->v = i;
  n->next = (*head_ref);
  (*head_ref) = n;
}

int main() {
  Node* n = malloc(sizeof(Node));
  n->v = 1;
  insert(&n, 2);
  insert(&n, 3);
  insert(&n, 4);
  insert(&n, 5);

  print_list(n);
  n = reverse(n);
  print_list(n);
  return 0;
}
