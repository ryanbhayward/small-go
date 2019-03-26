#include "Go.h"

#include <iostream>

Go::Go(int _n) : to_move(BLACK), n(_n) {
  Board::init_zobrist();
  Board b(_n);
  boards.push(b);
}

Go::~Go() {}

bool Go::make_move(int row, int col, Color color) {
  // first copy
  boards.push(boards.top());
  bool res = boards.top().move(row, col, color);
  if (!res) {
    boards.pop();
  } else {
    // move succeeded, check superko
    if (superko_hist.find(boards.top().h) != superko_hist.end()) {
      res = false;
      boards.pop();
    } else {
      superko_hist.insert(boards.top().h);
    }
  }
  return res;
}

bool Go::undo_move() {
  if (boards.size() <= 1) return false;
  const Board& old = boards.top();
  superko_hist.erase(old.h);
  boards.pop();
  switch_to_move();
  return true;
}

float Go::score(Color c) {
  return boards.top().score(c);
}

void Go::print_board() { 
  boards.top().print(); 
}

void Go::switch_to_move() {
  to_move = (to_move == BLACK) ? WHITE : BLACK;
}

long Go::get_legal_moves(Color color) {
  std::bitset<64> legal(boards.top().empty_points());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (legal.test(i*n + j)) {
        if (!make_move(i, j, color)) {
          legal.reset(i*n + j);
        } else {
          undo_move();
        }
      }
    }
  }
  return legal.to_ulong();
}
