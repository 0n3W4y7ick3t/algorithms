/**
 * @author leon
 * @date Jun 12, 2024 at 15:25:59
 * @tag binary search
 * @problem IOI'13P4 - Cave
 * @link https://dmoj.ca/problem/ioi13p4
 * @result AC
 */

#include <stdbool.h>
#include <stdlib.h>

// returns to the first close door or -1
int tryCombination(int S[]);
// submit anwsers
void answer(int S[], int D[]);

// flip switches in [left, right], ignore switches[i] if found[i]
static inline
void flip_switches(int* switches, size_t left, size_t right, bool* found) {
  for (size_t i = left; i <= right; ++i)
    switches[i] = (1 >> found[i]) ^ switches[i];
}

// find the switch of door, and set its switch to open it.
void set_switch(size_t door, int* switches, int* doors, bool* found, size_t N) {
  size_t lo = 0, hi = N - 1, mid;
  size_t result = tryCombination(switches);
  // make sure the i-th door is closed before searching
  if (result != door)
    flip_switches(switches, 0, N - 1, found);

  while (lo != hi) {
      mid = (lo + hi) / 2;
      flip_switches(switches, lo, mid, found);
      result = tryCombination(switches);
      if (result != door) {
          // the door is opened, it's in [lo, mid]
          flip_switches(switches, lo, mid, found); // flip back
          hi = mid;
        } else
        // the door is still closed, it's in [mid + 1, hi]
        lo = mid + 1;
    }

  // doors of switch lo is door
  doors[lo] = door;
  found[lo] = true;
  // reopen it
  switches[lo] = 1 ^ switches[lo];
}

// the implementation
void exploreCave(int N) {
  int* switches = calloc(N, sizeof(int));
  bool* found = calloc(N, sizeof(bool));
  int* doors = malloc(sizeof(int[N]));

  for (size_t i = 0; i < N; ++i)
    set_switch(i, switches, doors, found, N);
  answer(switches, doors);

  free(switches);
  free(doors);
  free(found);
}
