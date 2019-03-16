#pragma once

#include <stack>
#include <set>
#include <bitset>
#include <map>

enum Color { EMPTY = -1, BLACK = 0, WHITE = 1 };

struct Board {
  long stones[2]; // one for BLACK, one for WHITE
  long h; // zobrist hash value for positional superko testing

  Board() {
    stones[BLACK] = 0;
    stones[WHITE] = 0;
    h = 0;
  }

  Board(const Board& other) {
    stones[BLACK] = other.stones[BLACK];
    stones[WHITE] = other.stones[WHITE];
    h = other.h;
  }
};

class Go {

  std::stack<Board> board;
  std::set<long> superko_hist;
  int to_move;
  int n;

  // private methods
  long empty_points();
  void switch_to_move();

 public:
  Go(int n);
  ~Go(); 

  static std::map<int, char> color_chars;
  
  bool make_move(int row, int col, int color);
  void undo_move();
  float score();
  void print_board();
};


