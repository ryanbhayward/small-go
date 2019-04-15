// Copyright 2019 Chris Solinas
#include "board.h"

#include <iostream>

std::map<int, char> color_chars = {
  {EMPTY, '.'}, {BLACK, 'b'}, {WHITE, 'w'}
};

enum Border { LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3 };
// will probably want to pre-compute these
long edge_mask(unsigned short n, Border border) {
  long mask = 0;
  for (unsigned short i = 0; i < n; i++) {
    if (border == BOTTOM) {
      mask |= 1 << i;
    } else if (border == TOP) {
      mask |= 1 << (n*(n-1) + i);
    } else if (border == LEFT) {
      mask |= 1 << (n*(i+1) - 1);
    } else if (border == RIGHT) {
      mask |= 1 << (n*i);
    }
  }
  return mask;
}

void Board::init_zobrist() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 64; j++) {
      z_table[i][j] = std::rand();
    }
  }
}

long Board::z_table[2][64] = {{0}};

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
  size_mask = other.size_mask;
}

void Board::update_zobrist(long group, Color color) {
  std::bitset<64> b(group);
  for (int i = 0; i < n*n; i++) {
    if (b.test(i)) h ^= z_table[color][i];
  }
}

Color Board::opponent(Color color) {
  return color == BLACK ? WHITE : BLACK;
}

bool Board::move(int point_ind, Color color) {
  if (point_ind < 0 || point_ind >= n*n) return false;

  long point = 1 << point_ind;
  if (!(point & empty_points())) return false;

  // place the stone
  stones[color] |= point;
  update_zobrist(point, color);
  Color opp = opponent(color);
  // find neighbors of opponent color and check if they are captured
  long opp_groups[4] = {0, 0, 0, 0};
  opp_groups[0] = (point << 1) & stones[opp] & ~edge_mask(n, RIGHT);
  opp_groups[1] = (point >> 1) & stones[opp] & ~edge_mask(n, LEFT);
  opp_groups[2] = (point << n) & stones[opp];
  opp_groups[3] = (point >> n) & stones[opp];

  long group;
  for (int i = 0; i < 4; i++) {
    if (opp_groups[i]) {
      group = get_group(opp_groups[i]);
      if (get_liberties(group) == 0) {
        // capture opponent stones
        stones[opp] &= ~group;
        update_zobrist(group, opp);
      }
    }
  }

  group = get_group(point);
  // check suicide
  return get_liberties(group) != 0;
}

long Board::get_neighbors(long group) {
  long neighbors = 0;
  // shift left, remove right border neighbors
  neighbors |= (group << 1) & ~edge_mask(n, RIGHT);
  // shift right, remove left border neighbors
  neighbors |= (group >> 1) & ~edge_mask(n, LEFT);
  // shift down, remove top border neighbors
  neighbors |= (group << n);
  // shift up, remove bottomom border neighbors
  neighbors |= (group >> n);
  return size_mask & neighbors & ~group;
}

long Board::get_liberties(long group) {
  return get_neighbors(group) & ~(stones[BLACK] | stones[WHITE]);
}

bool Board::atari(int point_ind) {
  long group = get_group(1 << point_ind);
  return __builtin_popcountl(get_liberties(group)) <= 1;
}

long Board::get_group(long board_point) {
  long group = board_point;
  long black_mask = board_point & stones[BLACK];
  long white_mask = board_point & stones[WHITE];

  // check if point empty
  if (!(white_mask | black_mask)) return group;

  // figure out which color it actually is
  long color_mask = black_mask != 0 ? stones[BLACK] : stones[WHITE];

  // iteratively compute neighboring points of same color by computing
  // neighbors and masking based on color
  // if we get no change, we're done
  long old = 0;
  while (group != old) {
    old = group;
    group = (group | get_neighbors(group)) & color_mask;
  }

  return group;
}

float Board::score(Color color) {
  // the following is not portable to non-GNU compilers, if this is a problem
  // we can find a workaround
  int b = __builtin_popcountl(stones[BLACK]);
  int w = __builtin_popcountl(stones[WHITE]);
  std::bitset<64> empty(empty_points());
  for (int i = 0; i < n*n; i++) {
    if (empty.test(i)) {
      // have an empty point, check if all neighbors are one color
      long point = 1 << i;
      long neighbors = get_neighbors(point);
      if ((neighbors & stones[BLACK]) == neighbors) b++;
      else if ((neighbors & stones[WHITE]) == neighbors) w++;
    }
  }

  float score = color == BLACK ? 1.0 * (b-w) : 1.0 * (w-b);
  return score;
}

void Board::print() {
  std::bitset<64> b(stones[BLACK]);
  std::bitset<64> w(stones[WHITE]);
  Color color;
  std::cout << std::endl;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      color = EMPTY;
      if (b.test(i*n + j)) color = BLACK;
      else if (w.test(i*n + j)) color = WHITE;
      std::cout << color_chars[color];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

long Board::empty_points() const {
  return size_mask & ~(stones[BLACK] | stones[WHITE]);
}
