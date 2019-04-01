// Copyright 2019 Chris Solinas
#pragma once

#include "Go.h"

struct Result {
  Result() : value(0), best_move(-1) {}
  float value;
  int best_move;
};

class Solver {
 private:
  int nodes;
  Result alpha_beta(Go *game, float alpha, float beta, int depth);

 public:
  Solver() : nodes(0) {}
  float solve(Go *game);
};
