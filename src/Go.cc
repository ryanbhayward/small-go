#include "Go.h"

Go::Go() {}

Go::~Go() {}

bool Go::make_move(int, int, int) {
  return false;
}

void Go::undo_move() {
  board.pop();
  switch_to_move();
}

float Go::score() {
  return 0.0;
}

long Go::empty_points() {
  return ~ (board.top().stones[BLACK] | board.top().stones[WHITE]);
}

void Go::switch_to_move() {
  to_move = (to_move == BLACK) ? WHITE : BLACK;
}
