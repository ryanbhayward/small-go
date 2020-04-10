// Copyright 2019 Chris Solinas
#include "solver.h"
#include <iostream>
#include <algorithm>

Solver::Solver() : nodes(0), verbose(true) { init_theorems_3x3(); }

int Solver::solve(Go *game, Color c) {
  int max_score = game->size() * game->size();
  return solve(game, c, max_score);
}

int Solver::solve(Go *game, Color c, int max_score) {
  nodes = 0;
  start = Clock::now();
  int max_depth = 0;

  Result r;

  while (r.is_undefined()) {
    TT.clear();
    std::fill(theorem_hits.begin(), theorem_hits.end(), 0);
    r = alpha_beta(game, c, -1.0 * max_score, 1.0 * max_score, 0, ++max_depth);
    r.pv.push_front(r.best_move);
    if (verbose) {
      display_results(r, max_depth, game->size());
    }
  }
  return r.best_move;
}

Result Solver::alpha_beta(Go *game, Color c, float alpha, float beta, int d,
    int max_depth) {

  Result best;
  if (d > max_depth) return best;

  if (game->game_over() || (MAX_NODES > 0 && nodes > MAX_NODES)) {
    best.value = game->score(c);
    best.terminal = true;
    return best;
  }

  if (game->size() == 3) {
    for (size_t i = 0; i < theorems_3x3.size(); i++) {
      Theorem *t = theorems_3x3[i];
      if (t->applies(game->get_board(), Go::opponent(c))) {
        theorem_hits[i] += 1;
        best.value = -1 * t->get_value();
        best.terminal = true;
        best.benson = true;
        return best;
      }
    }
  }

  nodes += 1;

  // generate and sort moves
  std::vector<int> moves;
  game->get_moves(&moves);

  if (game->size() == 3) {
    std::sort(moves.begin(), moves.end(),
      move_ordering_3x3(game->get_board(), c, d));
  } else if (game->size() == 2) {
    std::sort(moves.begin(), moves.end(), move_ordering_2x2());
  }

  bool undefined = false;
  for (auto move : moves) {
    if (game->fills_eye(move, c)) {
      continue;
    }
    bool legal = game->make_move(move, c);
    if (!legal) continue;
    Result r = alpha_beta(game, Go::opponent(c), -1 * beta, -1 * alpha, d + 1,
        max_depth);

    r.pv.push_front(r.best_move);
    r.best_move = move;
    // negamax variant
    r.value *= -1;
    game->undo_move();

    if (d == 0 && verbose) {
      std::cout << Board::get_point_coord(move, game->size()) << " ";
      std::cout << r.value << std::endl;
    }

    if (r.is_undefined()) {
      undefined = true;
      continue;
    }

    if (r > best) best = r;

    if (r.value > alpha) {
      alpha = r.value;
    }
    // pruning
    if (alpha >= beta) {
      killer_table[d] = r.best_move;
      break;
    }
  }

  if (undefined) best.reset();
  best.benson = false;

  return best;
}

void Solver::display_results(Result r, int max_depth, int board_size) {
  std::cout << "theorem hits: [";
  for (int hn : theorem_hits) {
    std::cout << " " << hn;
  }
  std::cout << " ]" << std::endl;
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "d: " << max_depth;
  if (!r.is_undefined()) {
    std::cout << " value: " << r.value << " move: ";
    std::cout << Board::get_point_coord(r.best_move, board_size);
  } else {
    std::cout << " undefined";
  }
  std::cout << " nodes: " << nodes;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;

  if (!r.is_undefined()) {
    std::cout << "pv:";
    for (int m : r.pv) {
      std::cout << " " << Board::get_point_coord(m, board_size) << " ";
    }
    std::cout << std::endl;
  }
}

void Solver::init_theorems_3x3() {
  theorems_3x3.push_back(new Middle3x3());
  theorems_3x3.push_back(new Corner3x3());
  theorems_3x3.push_back(new SideOnly3x3());
  theorems_3x3.push_back(new SideSingle3x3());
  theorems_3x3.push_back(new SideDouble3x3());
  theorems_3x3.push_back(new CornerSingle3x3());
  theorem_hits.resize(theorems_3x3.size());
  std::fill(theorem_hits.begin(), theorem_hits.end(), 0);
}

void Solver::clean_theorems_3x3() {
  for (auto t : theorems_3x3) {
    delete t;
  }
}
