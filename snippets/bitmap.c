#include <limits.h>
#include <stdint.h>

typedef uint32_t word_t;
enum { BIT_PER_WORD = sizeof(word_t) * CHAR_BIT };
#define WORD_OFFSET(b) ((b) / BIT_PER_WORD)
#define BIT_OFFSET(b) ((b) % BIT_PER_WORD)

void set_bit(word_t* w, int n) { w[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n)); }

void clear_bit(word_t* w, int n) { w[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n)); }

int get_bit(word_t* w, int n) {
  word_t bit = w[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
  return bit != 0;
}
