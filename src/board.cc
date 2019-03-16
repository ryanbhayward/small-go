#include "board.h"

#include <iostream>

std::map<int, char> color_chars = {
  {EMPTY, '.'}, {BLACK, 'b'}, {WHITE, 'w'} 
};

Board::Board(int _n) : n(_n) {
  stones[BLACK] = 0;
  stones[WHITE] = 0;
  h = 0;
  size_mask = (1 << n*n) - 1;
}

Board::Board(const Board& other) {
  stones[BLACK] = other.stones[BLACK];
  stones[WHITE] = other.stones[WHITE];
  h = other.h;
  n = other.n;
}

bool Board::move(int, int, int) {
  return false;
}

float Board::score(int) {
  return 0.0;
}

void Board::print() {
  std::bitset<64> b(stones[BLACK]);
  std::bitset<64> w(stones[BLACK]);
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

long Board::empty_points() {
  return size_mask & ~(stones[BLACK] | stones[WHITE]);
}
