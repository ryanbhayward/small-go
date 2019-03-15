#pragma once

#include <stack>

enum Color { EMPTY = -1, BLACK = 0, WHITE = 1 };

struct Board {
  long stones[2]; // one for BLACK, one for WHITE
  long h;
};

class Go {

  Go();
  ~Go(); 

  std::stack<Board> board;
  int to_move;

  // private methods
  long empty_points();
  void switch_to_move();

 public:
  bool make_move(int row, int col, int color);
  void undo_move();
  float score();
};


