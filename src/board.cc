#include "board.h"

#include <iostream>

std::map<int, char> color_chars = {
  {EMPTY, '.'}, {BLACK, 'b'}, {WHITE, 'w'} 
};

enum Border { LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3 };
long edge_mask(unsigned short n, Border border) {
  long mask = 0;
  for (unsigned short i = 0; i < n; i++) {
    if (border == BOTTOM) {
      mask |= 1 << i;
    } else if (border == TOP) {
      mask |= 1 << (n*(n-1) + i);
    } else if (border == LEFT) {
      mask |= 1 << (n*(i+1) - 1);
    } else if (border == RIGHT){
      mask |= 1 << (n*i);
    }
  }
  return mask; 
}

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

long Board::get_neighbors(long group) {
  long neighbors = 0;
  // shift left, remove right border neighbors
  neighbors |= (group << 1) & ~edge_mask(n, RIGHT); 
  // shift right, remove left border neighbors
  neighbors |= (group >> 1) & ~edge_mask(n, LEFT); 
  // shift down, remove top border neighbors
  neighbors |= (group << n) & ~edge_mask(n, TOP); 
  // shift up, remove bottomom border neighbors
  neighbors |= (group >> n) & ~edge_mask(n, BOTTOM); 
  return neighbors & ~group;
}

long Board::liberties(long group) {
  return get_neighbors(group) & ~(stones[BLACK] | stones[WHITE]);
}

long Board::group(unsigned short point) {
  long group = 0;
  long board_point = 1 << point;
  long black_mask = board_point & stones[BLACK];
  long white_mask = board_point & stones[WHITE];
  
  // check if point empty
  if (!(white_mask | black_mask)) return group;

  // figure out which color it actually is
  long color_mask = black_mask != 0 ? black_mask : white_mask;

  // iteratively compute neighboring points of same color by computing
  // neighbors and masking based on color
  // if we get no change, we're done
  long old =  board_point;
  while (group != old) {
    old = group;
    group = get_neighbors(old) & color_mask;
  }

  return group;
}

float Board::score(int) {
  return 0.0;
}

void Board::print() {
  std::bitset<64> b(stones[BLACK]);
  std::bitset<64> w(stones[BLACK]);
  Color color;
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
