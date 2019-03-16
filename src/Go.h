#pragma once

#include <stack>
#include <set>
#include <bitset>
#include <map>

enum Color { EMPTY = -1, BLACK = 0, WHITE = 1 };

struct Board {
  long stones[2]; // one for BLACK, one for WHITE
  long h;
};

class Go {

  Go(int n);
  ~Go(); 

  std::stack<Board> board;
  std::set<long> superko_hist;
  int to_move;
  int n;

  // private methods
  long empty_points();
  void switch_to_move();

 public:
  static std::map<int, char> color_chars;
  bool make_move(int row, int col, int color);
  void undo_move();
  float score();
  void print_board();
};


