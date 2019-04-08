// Copyright 2019 Chris Solinas
#include <cassert>
#include <iostream>
#include "Go.h"

void test_pass() {
  Go g(5);
  g.make_move(0, BLACK);
  g.make_move(-1, WHITE);
  g.make_move(-1, BLACK);
  assert(g.game_over());
  g.undo_move();
  assert(!g.game_over());
  g.make_move(1, BLACK);
  g.make_move(4, WHITE);
  g.make_move(7, BLACK);
  g.make_move(10, WHITE);
  g.make_move(-1, BLACK);
  assert(!g.game_over());
  g.undo_move();
  g.undo_move();
  g.undo_move();
  g.undo_move();
  g.undo_move();
  g.make_move(-1, BLACK);
  assert(g.game_over());
}

int main() {
  test_pass();
  return 0;
}
