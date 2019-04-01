// Copyright 2019 Chris Solinas
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
  bool gen_move_cmd(std::string cmd);
  bool undo_move_cmd();
  bool get_legal_moves_cmd(std::string cmd);
  bool score_cmd();
  // regex for command strings
  static std::regex show_reg;
  static std::regex move_reg;
  static std::regex genmove_reg;
  static std::regex undo_reg;
  static std::regex legal_reg;
  static std::regex score_reg;

 public:
  GTP_interface(Go *_game, bool _verbose) : game(_game), verbose(_verbose) {}
  void listen();
};

