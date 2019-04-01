// Copyright 2019 Chris Solinas
#include <iostream>

#include "Go.h"
#include "gtp_interface.h"

int main() {
  Go game(2);
  Solver solver;
  GTP_interface gtp(&game, &solver, true);
  gtp.listen();
  return 0;
}
