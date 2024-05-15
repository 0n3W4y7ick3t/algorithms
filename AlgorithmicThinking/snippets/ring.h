/* This file defines a ring buffer
 * @author leon
 * @date May 15, 2024 at 16:30:24
 */

#include <stddef.h>
#include <stdbool.h>

typedef struct ring ring;
struct ring {
  size_t start;
  size_t len;
  size_t cap;
  double* data;
};

ring* ring_init(ring* r, size_t cap);
void ring_deinit(ring* r);

ring* ring_new(size_t len);
void ring_delete(ring* r);


/* push an item at the end, returns r if ok, 0 otherwise */
ring* ring_push(ring* r, double item);

/* pop an item from the beginning, save it into out, returns false if failed */
bool ring_pop_s(ring* r, double* out);

/* pop an item from the beginning, returns 0 if failed */
double ring_pop(ring* r);

/* returns the ref to the pos' item, or 0 if not found */
double* ring_at(ring* r, size_t index);

/* get inner position of index' item */
size_t ring_pos(ring* r, size_t index);

size_t ring_len(ring* r);

size_t ring_cap(ring* r);

ring* ring_resize(ring* r, size_t ncap);
