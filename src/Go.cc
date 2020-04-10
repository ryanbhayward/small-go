// Copyright 2019 Chris Solinas
#include "Go.h"

#include <iostream>
#include <cassert>

Go::Go(int _n) : to_move(BLACK), n(_n) {
  Board::init_zobrist();
  boards.push(Board(_n));
  passes.push(0);
}

Go::~Go() {}

int Go::size() { return n; }

bool Go::game_over() {
  return passes.top() > 1;
}

bool Go::last_move_was_pass() { return passes.top() > 0; }

bool Go::make_move(int point_ind, Color color) {
  // first copy
  boards.push(boards.top());

  // check for a pass
  if (point_ind == PASS_IND) {
    passes.push(passes.top() + 1);
    if (to_move == color) {
      switch_to_move();
    }
    return true;
  }

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
    // all checks done, reset pass counter
    passes.push(0);

    // other player's turn next
    if (to_move == color) {
      switch_to_move();
    }
  }

  return res;
}

bool Go::undo_move() {
  if (boards.size() <= 1) return false;
  const Board& old = boards.top();
  int last_passes = passes.top();
  passes.pop();
  // don't erase superko hist if popping a pass
  if (last_passes <= passes.top())  superko_hist.erase(old.h);
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

/**
 * Get the possible moves on the current board.
 *
 * nullptr is a valid parameter value for moves if we just care to test
 * that there are moves available
 **/
long Go::get_moves(std::vector<int> *moves) {
  if (moves != nullptr)  moves->clear();
  std::bitset<64> legal(boards.top().empty_points());
  for (int i = 0; i < n*n; i++) {
    if (legal.test(i)) {
      if (moves != nullptr)
        moves->push_back(i);
    }
  }

  // include pass move
  if (moves != nullptr) moves->push_back(PASS_IND);

  return legal.to_ulong();
}

bool Go::fills_eye(int point_ind, Color c) {
  if (point_ind < 0) return false;
  return boards.top().fills_eye(point_ind, c);
}

Board& Go::get_board() { return boards.top(); }

Color Go::opponent(Color c) { return Board::opponent(c); }
