/**
 * @author leon
 * @date Jun 06, 2024 at 20:18:08
 */
#include <stdlib.h>

typedef int (*Cmp)(size_t, size_t);

typedef struct Heap {
  size_t* items;
  size_t len;
  size_t cap;
  Cmp cmp;
} Heap;

int greater(size_t v1, size_t v2) { return v1 >= v2; }

int less(size_t v1, size_t v2) { return v1 <= v2; }

static inline size_t parent(size_t index) { return (index - 1) / 2; };
static inline size_t lc(size_t index) { return 2 * index + 1; }
static inline size_t rc(size_t index) { return 2 * index + 2; }

static inline void swap(size_t* a, size_t* b) {
  size_t tmp = *a;
  *a = *b;
  *b = tmp;
}

Heap* heap_new(size_t cap, Cmp cmp) {
  Heap* h = malloc(sizeof(Heap));
  h->len = 0;
  h->cmp = cmp;
  h->cap = cap;
  h->items = malloc(sizeof(size_t[h->cap]));
  return h;
}

void heap_grow(Heap* h) {
  h->cap *= 2;
  h->items = realloc(h->items, sizeof(size_t[h->cap]));
}

void heap_delete(Heap* h) {
  if (h) {
      free(h->items);
      free(h);
    }
}

void heap_up(Heap* h, size_t index) {
  while (index && !h->cmp(h->items[parent(index)], h->items[index])) {
      swap(&h->items[parent(index)], &h->items[index]);
      index = parent(index);
    }
}

void heap_down(Heap* h, size_t index) {
  while (1) {
      if (rc(index) <= h->len) {
          // with lc and rc
          size_t i;
          if (h->cmp(h->items[lc(index)], h->items[rc(index)]))
            i = lc(index); // lc satifies more
          else
            i = rc(index);
          if (!h->cmp(h->items[index], h->items[i])) {
              swap(&h->items[index], &h->items[i]);
              index = i;
            } else
            break;
        } else if (lc(index) == h->len) {
          // lc only
          if (!h->cmp(h->items[index], h->items[lc(index)])) {
              swap(&h->items[index], &h->items[lc(index)]);
              index = lc(index);
            } else
            break;
        } else
        break;
    }
}

size_t heap_top(Heap* h) { return h->items[0]; }

void heap_pop(Heap* h) {
  h->items[0] = h->items[--h->len];
  heap_down(h, 0);
}

void heap_push(Heap* h, size_t item) {
  if (h->len == h->cap)
    heap_grow(h);
  h->items[h->len++] = item;
  heap_up(h, h->len - 1);
}
