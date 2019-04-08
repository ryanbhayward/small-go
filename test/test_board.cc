// Copyright 2019 Chris Solinas
#include <cassert>
#include <iostream>
#include "board.h"

void test_score() {
  Board b(2);
  assert(b.score(BLACK) == 0);
  assert(b.score(WHITE) == 0);

  // make a move for black and check normal stone scoring
  b.move(0, BLACK);
  assert(b.score(BLACK) == 1);
  assert(b.score(WHITE) == -1);
  // repeat for white
  b.move(1, WHITE);
  assert(b.score(BLACK) == 0);
  assert(b.score(WHITE) == 0);

  Board c(b);
  // board looks like:
  // ..
  // wb
  // test territory after black capture
  b.move(3, BLACK);
  assert(b.score(BLACK) == 4);
  assert(b.score(WHITE) == -4);

  // use old board to test neutral territory on more time
  c.move(2, BLACK);
  assert(c.score(BLACK) == 1);
  assert(c.score(WHITE) == -1);

  // use a 3x3 board to test inside surround (we know edges work)
  Board d(3);
  for (int i = 0; i < 9; i++) {
    if (i != 4) d.move(i, BLACK);
  }
  assert(d.score(BLACK) == 9);
  assert(d.score(WHITE) == -9);
  // same thing but side centers,
  Board e(3);
  for (int i = 0; i < 9; i++) {
    if (i % 2 == 0) e.move(i, BLACK);
  }
  assert(e.score(BLACK) == 9);
  assert(e.score(WHITE) == -9);
}

void test_moves_and_captures() {
  Board b(2);
  assert(b.move(1, BLACK));
  assert(!b.move(1, WHITE));
  assert(b.move(0, WHITE));
  assert(b.move(2, BLACK));
  // capture
  assert(b.empty_points() == 9);
}

void test_liberties() {
  Board b(3);
  b.move(1, BLACK);
  b.move(4, BLACK);
  assert(b.get_liberties(b.get_group(1 << 1)) == 173);
  b.move(7, WHITE);
  assert(b.get_liberties(b.get_group(1 << 1)) == 45);
  Board c(2);
  c.move(0, BLACK);
  assert(c.get_liberties(c.get_group(1 << 0)) == 6);
}

void test_empty_points() {
  Board b(2);
  assert(b.empty_points() == 15);
  b.move(0, BLACK);
  assert(b.empty_points() == 14);
  b.move(3, WHITE);
  assert(b.empty_points() == 6);
  b.move(1, BLACK);
  assert(b.empty_points() == 4);
}

void test_neighbors() {
  Board b(3);
  // center
  assert(b.get_neighbors(1 << 4) == 170);
  // side center
  assert(b.get_neighbors(1 << 1) == 21);
  assert(b.get_neighbors(1 << 5) == 276);
  // corner
  assert(b.get_neighbors(1) == 10);
  assert(b.get_neighbors(1 << 6) == 136);

  Board c(2);
  c.move(0, BLACK);
  assert(c.get_neighbors(1 << 0) == 6);
}

void test_groups() {
  Board b(3);
  b.move(1, BLACK);
  b.move(4, BLACK);
  assert(b.get_group(1 << 1) == 18);
  assert(b.get_group(1 << 4) == 18);
  b.move(6, BLACK);
  assert(b.get_group(1 << 1) == 18);
  assert(b.get_group(1 << 4) == 18);
  assert(b.get_group(1 << 6) == 64);
  b.move(3, WHITE);
  assert(b.get_group(1 << 1) == 18);
  assert(b.get_group(1 << 4) == 18);
  assert(b.get_group(1 << 6) == 64);
  assert(b.get_group(1 << 3) == 8);

  Board c(2);
  c.move(0, BLACK);
  assert(c.get_group(1 << 0) == 1);
}


int main() {
  test_empty_points();
  test_groups();
  test_neighbors();
  test_liberties();
  test_moves_and_captures();
  test_score();
  return 0;
}
