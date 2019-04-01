// Copyright 2019 Chris Solinas
#include <iostream>

#include "Go.h"
#include "gtp_interface.h"

int main() {
  Go game(3);
  GTP_interface gtp(&game, true);
  gtp.listen();
  return 0;
}
