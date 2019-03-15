#pragma once

#include <stack>

enum Color { EMPTY = -1, BLACK = 0, WHITE = 1 };

struct Board {
  long stones[2]; // one for BLACK, one for WHITE
  long h;
};

class Go {

  std::stack<Board> board;

  // private methods
  long empty_points();

 public:
  bool make_move(int, int);
  void undo_move();
  float score();
};


