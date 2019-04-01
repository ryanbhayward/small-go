// Copyright 2019 Chris Solinas
#include "Go.h"

#include <iostream>

Go::Go(int _n) : to_move(BLACK), n(_n), passes(0), old_passes(0) {
  Board::init_zobrist();
  Board b(_n);
  boards.push(b);
}

Go::~Go() {}

int Go::size() { return n; }

bool Go::game_over() {
  return passes > 1 || (get_legal_moves(BLACK, nullptr) == 0 &&
    get_legal_moves(WHITE, nullptr) == 0);
}

bool Go::make_move(int point_ind, Color color) {
  // check for a pass
  if (point_ind == PASS_IND) {
    ++passes;
    if (to_move == color) {
      switch_to_move();
    }
    return true;
  }

  // first copy
  boards.push(boards.top());
  bool res = boards.top().move(point_ind, color);
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

  if (res) {
    old_passes = passes;
    passes = 0;
    if (to_move == color) {
      switch_to_move();
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
  passes = old_passes;
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

/**
 * nullptr is a valid parameter value for moves if we just care to test
 * that there are legal moves
 **/
long Go::get_legal_moves(Color color, std::vector<int> *moves) {
  if (moves != nullptr)  moves->clear();
  std::bitset<64> legal(boards.top().empty_points());
  for (int i = 0; i < n*n; i++) {
    if (legal.test(i)) {
      if (make_move(i, color)) {
        undo_move();
        if (moves != nullptr) moves->push_back(i);
      } else {
        legal.reset(i);
      }
    }
  }

  // include pass move
  if (moves != nullptr) moves->push_back(PASS_IND);

  return legal.to_ulong();
}

Color Go::opponent(Color c) { return Board::opponent(c); }
