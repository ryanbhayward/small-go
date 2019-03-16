#include "Go.h"

#include <iostream>

std::map<int, char> Go::color_chars = {
  {EMPTY, '.'}, {BLACK, 'b'}, {WHITE, 'w'} 
};

Go::Go(int _n) : to_move(BLACK), n(_n) {}

Go::~Go() {}

bool Go::make_move(int, int, int) {
  return false;
}

void Go::undo_move() {
  const Board& old = board.top();
  superko_hist.erase(old.h);
  board.pop();
  switch_to_move();
}

float Go::score() {
  return 0.0;
}

void Go::print_board() {
  std::bitset<64> b(board.top().stones[BLACK]);
  std::bitset<64> w(board.top().stones[BLACK]);
  int color;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      color = EMPTY;
      if (b.test(i*n + j)) color = BLACK;
      else if (w.test(i*n + j)) color = WHITE;
      std::cout << color_chars[color];
    }
    std::cout << std::endl;
  }
}

long Go::empty_points() {
  return ~ (board.top().stones[BLACK] | board.top().stones[WHITE]);
}

void Go::switch_to_move() {
  to_move = (to_move == BLACK) ? WHITE : BLACK;
}
