#include "Go.h"

bool Go::make_move(int, int) {
  return false;
}

void Go::undo_move() {
  board.pop();
}

float Go::score() {
  return 0.0;
}

long Go::empty_points() {
  return ~ (board.top().stones[BLACK] | board.top().stones[WHITE]);
}
