#pragma once

#include <vector>
#include "board.h"

class Theorem {
 private:
  long position_mask;
  std::vector<long> liberty_masks;
  float value;
 
 public:
  Theorem(long pos_mask, std::vector<long> lib_masks, float _value) 
    : position_mask(pos_mask), liberty_masks(lib_masks), value(_value){}
  
  bool applies(const Board& b, Color c);
  float get_value();
};
