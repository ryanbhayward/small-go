#pragma once

#include <stack>
#include <set>

#include "board.h"


/*
 * Play games of Go using a fast bitboard implementation
 *
 * Uses the least significant bits for boards smaller than 8x8.
 * The bottom right corner of the board is always the least significant
 * digit

 * */
class Go {

  std::stack<Board> boards;
  std::set<long> superko_hist;
  int to_move;
  int n;

  // private methods
  void switch_to_move();

 public:
  Go(int n);
  ~Go(); 

  bool make_move(int row, int col, Color color);
  void undo_move();
  float score(Color color);
  void print_board();
};


