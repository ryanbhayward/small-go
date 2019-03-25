#pragma once

/**
Command line interface that is compatible with programs such as gogui
via GTP
**/

#include <string>
#include <regex>
#include "Go.h"



class GTP_interface {
 private:
  Go *game;
  bool verbose;
  bool execute(std::string cmd);
  void msg_illegal(std::string cmd);
  // commands
  bool show_board_cmd();
  bool play_move_cmd(std::string cmd);
  // regex for command strings
  static std::regex show_reg;
  static std::regex move_reg;
 public:
  GTP_interface(Go *_game, bool _verbose) : game(_game), verbose(_verbose) {}
  void listen();
};

