#include "Go.h"

Go::Go(int _n) : to_move(BLACK), n(_n) {
  Board b(_n);
  boards.push(b);
}

Go::~Go() {}

bool Go::make_move(int, int, int) {
  return false;
}

void Go::undo_move() {
  const Board& old = boards.top();
  superko_hist.erase(old.h);
  boards.pop();
  switch_to_move();
}

float Go::score() {
  return 0.0;
}

void Go::print_board() { boards.top().print(); }

void Go::switch_to_move() {
  to_move = (to_move == BLACK) ? WHITE : BLACK;
}
