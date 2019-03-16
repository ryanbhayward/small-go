#pragma once

#include <bitset>
#include <map>

// Globals to help callers use consistent values
enum Color { EMPTY = -1, BLACK = 0, WHITE = 1 };
extern std::map<int, char> color_chars; 

/*
 * Bit board used to represent states in the game
 * Works for board up to 8x8 
 * */
struct Board {
  long stones[2]; // one for BLACK, one for WHITE
  long h; // zobrist hash value for positional superko testing
  unsigned short n;
  long size_mask;

  Board(int _n);
  Board(const Board& other);

  bool move(int row, int col, int color);
  float score(int color);

  // helper functions
  long empty_points();
  void print();
};
