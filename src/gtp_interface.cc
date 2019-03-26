#include "gtp_interface.h"

#include <iostream>
#include "board.h"

std::regex GTP_interface::show_reg("showboard");
std::regex GTP_interface::move_reg("play (b|w) [[:alpha:]][[:digit:]]");
std::regex GTP_interface::genmove_reg("genmove (b|w)");  
std::regex GTP_interface::undo_reg("undo");
std::regex GTP_interface::legal_reg("legal_moves (b|w)");
std::regex GTP_interface::score_reg("score");

void GTP_interface::listen() {
  std::string cmd;
  while (1) {
    std::getline(std::cin, cmd);
    execute(cmd);
  }
}
  
bool GTP_interface::execute(std::string cmd) {
  bool legal = true;

  if (std::regex_match(cmd, show_reg)) {
    legal = show_board_cmd();
  } else if (std::regex_match(cmd, move_reg)) {
    legal = play_move_cmd(cmd);
  } else if (std::regex_match(cmd, genmove_reg)) {
    legal = gen_move_cmd(cmd);
  } else if (std::regex_match(cmd, undo_reg)) {
    legal = undo_move_cmd();
  } else if (std::regex_match(cmd, legal_reg)) {
    legal = get_legal_moves_cmd(cmd);
  } else if (std::regex_match(cmd, score_reg)) {
    legal = score_cmd();
  } else { 
    legal = false;
  }

  if (!legal) msg_illegal(cmd);
  return legal;
}

void GTP_interface::msg_illegal(std::string cmd) {
  std::cout << "Illegal cmd: " << cmd << std::endl;
}

bool GTP_interface::show_board_cmd() { 
  game->print_board();
  return true;
}

bool GTP_interface::play_move_cmd(std::string cmd) {
  // tokenize cmd string, kind of ugly but simple
  std::string tmp;
  char color, x, y;
  std::stringstream is(cmd);
  is >> tmp >> color >> x >> y;
  int row = x - 'a';
  int col = y - '1';
  Color c = color == 'b' ? BLACK : WHITE;
  return game->make_move(row * game->size() + col, c);
}
  
bool GTP_interface::gen_move_cmd(std::string) {
  return false;
}

bool GTP_interface::undo_move_cmd() { return game->undo_move(); }
 
bool GTP_interface::get_legal_moves_cmd(std::string) {
  return false;
}

bool GTP_interface::score_cmd() {
  std::cout << game->score(BLACK) << std::endl;
  return true;
}

