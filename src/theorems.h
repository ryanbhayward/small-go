// Copyright 2019 Chris Solinas
#pragma once

#include <vector>
#include "board.h"
#include <iostream>

class Theorem {
 protected:
  float value;

 public:
  Theorem() : value(0) {}
  virtual ~Theorem() {}
  virtual bool applies(const Board& , Color) { return false; }
  float get_value() { return value; }
};

class Corner3x3 : public Theorem {
  // ...
  // xx.
  // .x.
  // and isomorphic positions
 public:
  Corner3x3() { value = 9; }

  bool applies(const Board& b, Color c) {
    long positions[] = {26, 50, 152, 176};
    long liberties[] = {1, 4, 64, 256};

    long empty = b.empty_points();
    for (int i = 0; i < 4; i++) {
      long position = positions[i];
      // check that if matches the shape and has the corresponding corner
      // liberty first
      if ((position & b.stones[c]) == position) {
        long liberty = liberties[i];
        if ((liberty & empty) == liberty) {
          // matches required shape for corner theorem, just need
          // an additional liberty to make it safe
          long other = ~(liberty | position);
          if ((other & empty) != 0) {
            return true;
          }
        }
      }
    }

    return false;
  }
};

class Middle3x3 : public Theorem {
 public:
  Middle3x3() { value = 9; }

  bool applies(const Board& b, Color c) {
    long empty = b.empty_points();
    // .x.
    // .x.
    // .x.
    //
    // 2^1 + 2^4 + 2^7 = 146
    //
    //
    if ((b.stones[c] & 146) == 146) {
      // need to make sure there is at least one liberty on each side
     if (((146 << 1) & empty) != 0 && ((146 >> 1) & empty) != 0) {
       return true;
     }
    }

    // ...
    // xxx
    // ...
    if ((b.stones[c] & 56) == 56) {
     if (((56 << 3) & empty) != 0 && ((56 >> 3) & empty) != 0) {
       return true;
      }
    }

    return false;
  }
};

class SideSingle3x3 : public Theorem {
 public:
  SideSingle3x3() { value = 3; }

  bool applies(const Board &b, Color c) {
    long sides[] = { 2, 8, 32, 128 };
    Color opp = Go::opponent(c);
    if (b.stones[opp] != 0) return false;
    for (auto side : sides) {
      if (b.stones[c] == side) return true;
    }
    return false;
  }
};

class SideDouble3x3 : public Theorem {
 public:
  SideDouble3x3() { value = 3; }

  bool applies(const Board &b, Color c) {
    long sides[] = { 3, 6, 9, 36, 72, 192, 288, 384};
    Color opp = Go::opponent(c);
    if (b.stones[opp] != 0) return false;
    for (auto side : sides) {
     if (b.stones[c] == side) return true;
    }
    return false;
  }
};

class CornerSingle3x3 : public Theorem {
 public:
  CornerSingle3x3() { value = -9; }

  bool applies(const Board &b, Color c) {
    long sides[] = { 1, 4, 64, 256 };
    Color opp = Go::opponent(c);
    if (b.stones[opp] != 0) return false;
    for (auto side : sides) {
      if (b.stones[c] == side) return true;
    }
    return false;
  }
};

class SideOnly3x3 : public Theorem {
 public:
  SideOnly3x3() { value = -9; }

  bool applies(const Board& b, Color c) {
    long sides[] = { 5, 73, 292, 448 };
    long empty = b.empty_points();
    for (auto side : sides) {
      // 2^9 -1 = 511 (full board)
      if (b.stones[c] == side && (side | empty) == 511) {
        return true;
      }
    }
    return false;
  }
};
