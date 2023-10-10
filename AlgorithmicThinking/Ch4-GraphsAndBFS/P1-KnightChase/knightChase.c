/**
 * @author leon
 * @date Oct 10, 2023 at 19:10:05
 * @tag Graphs, Dynamic Programming
 * @problem DMOJ problem ccc95s4
 * @link https://dmoj.ca/problem/ccc99s4
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BORAD_SIZE 100

typedef struct {
  int row;
  int col;
} Pos;

void win(int x) { printf("Win in %d knight move(s).\n", x); }

void stalemate(int x) { printf("Stalemate in %d knight move(s).\n", x); }

void loss(int x) { printf("Loss in %d knight move(s).\n", x); }

/* from w=0 -> w=2pi, move_1 -> move_8 */
/* we take it the board starts in the left bottom corner */
void kmove_1(Pos *k) {
  k->row += 1;
  k->col += 2;
}

void kmove_2(Pos *k) {
  k->row += 2;
  k->col += 1;
}

void kmove_3(Pos *k) {
  k->row += 2;
  k->col -= 1;
}

void kmove_4(Pos *k) {
  k->row += 1;
  k->col -= 2;
}

void kmove_5(Pos *k) {
  k->row -= 1;
  k->col -= 2;
}

void kmove_6(Pos *k) {
  k->row -= 2;
  k->col -= 1;
}

void kmove_7(Pos *k) {
  k->row -= 2;
  k->col += 1;
}

void kmove_8(Pos *k) {
  k->row -= 1;
  k->col += 2;
}

/* pawn's only move */
void pmove(Pos *p) { p->row++; }

int main(void) {
  /*  */

  return 0;
}
