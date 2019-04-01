// Copyright 2019 Chris Solinas
#pragma once

#include<chrono>
#include <list>
#include "Go.h"

typedef std::chrono::system_clock Clock;
typedef std::chrono::duration<float> float_seconds;

struct Result {
  Result() : value(-10000), best_move(-1) {}
  float value;
  int best_move;
  std::list<int> pv;

  // overload relational operators to make search cleaner
  friend inline bool operator<(const Result& l, const Result& r) {
    return l.value < r.value;
  }
  friend inline bool operator>(const Result& l, const Result& r) {
    return r < l;
  }
  friend inline bool operator<=(const Result& l, const Result& r) {
    return !(l > r);
  }
  friend inline bool operator>=(const Result& l, const Result& r) {
    return !(l < r);
  }
};

class Solver {
 private:
  int nodes;
  bool verbose;
  Result alpha_beta(Go *game, Color c, float alpha, float beta, int depth);

 public:
  Solver() : nodes(0), verbose(true) {}
  int solve(Go *game, Color c);
};
