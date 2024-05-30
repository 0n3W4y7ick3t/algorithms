#include "ring.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

ring* ring_new(size_t cap) {
  ring* r = malloc(sizeof(ring));
  r->start = 0;
  r->len = 0;
  ring_init(r, cap);
  return r;
}

void ring_delete(ring* r) {
  if (r) {
    ring_deinit(r);
    free(r);
  }
}

void ring_deinit(ring* r) {
  if (r) {
    if (r->data) {
      free(r->data);
      r->len = 0;
      r->cap = 0;
    }
  }
}

ring* ring_init(ring* r, size_t cap) {
  if (r) {
    if (cap) {
      *r = (ring){
          .cap = cap,
          .data = malloc(sizeof(double[cap])),
      };
      if (!r->data)
        r->cap = 0;
    } else
      *r = (ring){0};
  }
  return r;
}

size_t ring_len(ring* r) { return r->len; }

size_t ring_cap(ring* r) { return r->cap; }

size_t ring_pos(ring* r, size_t index) {
  index += r->start;
  index %= r->cap;
  return index;
}

double* ring_at(ring* r, size_t index) {
  double* ret = 0;
  if (r && index < r->cap) {
    ret = &(r->data[ring_pos(r, index)]);
  }
  return ret;
}

ring* ring_push(ring* r, double item) {
  if (r) {
    if (r->len < r->cap)
      r->data[ring_pos(r, r->len++)] = item;
    else {
      ring_resize(r, r->cap * 2);
      ring_push(r, item);
    }
  } else {
    return 0;
  }
  return r;
}

double ring_pop(ring* r) {
  if (r) {
    if (r->len) {
      double item = r->data[r->start++];
      --r->len;
      return item;
    }
  }
  return 0;
}

bool ring_pop_s(ring* r, double* out) {
  if (r->len) {
    *out = r->data[r->start++];
    --r->len;
  } else
    return false;
  return true;
}

/*
left    right
4 5 6 0 1 2
4 5 6 0 1 2 xxxxxxxxxxxxxxx

4 5 6 0 1 2 4 5 xxxxxxxxxxxx
*/

ring* ring_resize(ring* r, size_t ncap) {
  if (r) {
    size_t ocap = r->cap;
    if (ncap > ocap) {
      size_t len = r->len;
      size_t ostart = r->start;
      size_t nstart = ostart;
      double* odata = r->data;
      double* ndata = realloc(odata, sizeof(double[ncap]));

      // the data is truncated
      if (ostart + len > ocap) {
        size_t rlen = ocap - ostart;
        size_t llen = len - rlen;

        if ((ncap - ocap) >= llen) {
          // the new space fits the left part
          memcpy(ndata + ocap, ndata, llen * sizeof(double));
        } else {
          // move the right part
          nstart = ncap - rlen;
          // memmove allows overlap, while memcpy not
          memmove(ndata + nstart, ndata + ostart, rlen * sizeof(double));
        }
      }

      *r = (ring){
          .start = nstart,
          .len = len,
          .cap = ncap,
          .data = ndata,
      };
    } else
      return 0;
  }
  return r;
}
