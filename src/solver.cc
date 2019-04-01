// Copyright 2019 Chris Solinas
#include "solver.h"

float Solver::solve(Go *game) {
  nodes = 0;
  Result r = alpha_beta(game,  -10000, 10000, 0);
  return r.value;
}

Result Solver::alpha_beta(Go *game, float alpha, float beta, int depth) {
  Result r;
  return r;
}
