#include "theorems.h"

bool Theorem::applies(const Board& b, Color c) {
  bool match = (position_mask & b.stones[c]) == position_mask;
  long empty = b.empty_points();
  for (int lib_mask : liberty_masks) {
    match = match && ((empty & lib_mask) != 0);
  }

  return match;
}

float Theorem::get_value() { return value; }
