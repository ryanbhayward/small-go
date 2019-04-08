#pragma once

#include <vector>
#include "board.h"

class Theorem {
 private:
  long position_mask;
  std::vector<long> liberty_masks;
 
 public:
  Theorem(long pos_mask, std::vector<long> lib_masks) 
    : position_mask(pos_mask), liberty_masks(lib_masks) {}
  
  bool applies(const Board& b, Color c);
};
