// Copyright 2019 Chris Solinas
#pragma once

#include<chrono>
#include "Go.h"

typedef std::chrono::system_clock Clock;
typedef std::chrono::duration<float> float_seconds;

struct Result {
  Result() : value(0), best_move(-1) {}
  float value;
  int best_move;
};

class Solver {
 private:
  int nodes;
  bool verbose;
  Result alpha_beta(Go *game, float alpha, float beta, int depth);

 public:
  Solver() : nodes(0), verbose(true) {}
  float solve(Go *game);
};
