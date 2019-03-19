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

  bool move(int row, int col, Color color);
  // return the neighboring points of group
  long get_neighbors(long group);
  // return the liberties of group
  long get_liberties(long group);
  // return the group of stones stone at position point is part of
  long get_group(long board_point);
  float score(Color color);
  Color opponent(Color color);

  // helper functions
  long empty_points();
  void print();
};
