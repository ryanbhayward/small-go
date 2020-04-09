// Copyright 2019 Chris Solinas
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
  long stones[2];  // one for BLACK, one for WHITE
  long h;  // zobrist hash value for positional superko testing
  unsigned short n;
  long size_mask;

  Board(int _n);
  Board(const Board& other);

  bool move(int point_ind, Color color);
  // return the neighboring points of group
  long get_neighbors(long group);
  // return the liberties of group
  long get_liberties(long group);
  bool atari(int point_ind);
  // return the group of stones stone at position point is part of
  long get_group(long board_point);
  float score(Color color);
  static Color opponent(Color color);
  void update_zobrist(long group, Color color);
  bool fills_eye(int move, Color c);

  // helper functions
  long empty_points() const;
  void print() const;
  static std::string get_point_coord(int point_ind, int _n);

  static long z_table[2][64];
  static void init_zobrist();
};

