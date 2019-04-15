// Copyright 2019 Chris Solinas
#include "solver.h"
#include <iostream>
#include <algorithm>

Solver::Solver() : nodes(0), verbose(true) { init_theorems_3x3(); }

int Solver::solve(Go *game, Color c) {
  nodes = 0;
  start = Clock::now();
  int max_score = game->size() * game->size();
  Result r = alpha_beta(game, c, -1.0 * max_score, 1.0 * max_score, 0);
  if (verbose) {
    display_results(r);
  }
  return r.best_move;
}

Result Solver::alpha_beta(Go *game, Color c, float alpha, float beta, int d) {
  Result best;
  nodes += 1;

  if (nodes % 100000000 == 0) display_intermediate();

  if (game->game_over() || (MAX_NODES > 0 && nodes > MAX_NODES)) {
    best.value = game->score(c);
    return best;
  }

  // TODO(chris): make this way faster by precomputing values
  if (game->size() == 3) {
    for (Theorem t : theorems_3x3) {
      if (t.applies(game->get_board(), c)) {
        best.value = t.get_value();
        return best;
      }
    }
  }

  // generate and sort moves
  std::vector<int> legal_moves;
  game->get_legal_moves(c, &legal_moves);
  // could use some polymorphism here with a general ordering object
  if (game->size() == 3) {
    std::sort(legal_moves.begin(), legal_moves.end(),
      move_ordering_3x3(game->get_board(), c));
  } else if (game->size() == 2) {
    std::sort(legal_moves.begin(), legal_moves.end(), move_ordering_2x2());
  }

  for (auto move : legal_moves) {
    game->make_move(move, c);
    Result r = alpha_beta(game, Go::opponent(c), -1 * beta, -1 * alpha, d + 1);
    r.best_move = move;
    // negamax variant
    r.value *= -1;
    game->undo_move();

    if (r > best) best = r;

    if (r.value > alpha) {
      alpha = r.value;
      best.pv.push_front(move);
    }
    // pruning
    if (alpha >= beta) break;
  }

  return best;
}

void Solver::display_intermediate() {
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "nodes: " << nodes;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;
}

void Solver::display_results(Result r) {
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "value: " << r.value << " move: " << r.best_move;
  std::cout << " nodes: " << nodes;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;
  std::cout << "pv:";
  for (int m : r.pv) {
    std::cout << " " << m << " ";
  }
  std::cout << std::endl;
}

void Solver::init_theorems_3x3() {
  // ...
  // xxx
  // ...
  std::vector<long> lib_masks;
  lib_masks.push_back(365);
  theorems_3x3.push_back(Theorem(146, lib_masks, 9));
  lib_masks.clear();
  lib_masks.push_back(455);
  theorems_3x3.push_back(Theorem(56, lib_masks, 9));

  // ...
  // xx.
  // .x.
  lib_masks.clear();
  lib_masks.push_back(1);
  lib_masks.push_back(484);
  theorems_3x3.push_back(Theorem(26, lib_masks, 9));
  lib_masks.clear();
  lib_masks.push_back(64);
  lib_masks.push_back(295);
  theorems_3x3.push_back(Theorem(152, lib_masks, 9));
  lib_masks.clear();
  lib_masks.push_back(4);
  lib_masks.push_back(79);
  theorems_3x3.push_back(Theorem(176, lib_masks, 9));
  lib_masks.clear();
  lib_masks.push_back(256);
  lib_masks.push_back(457);
  theorems_3x3.push_back(Theorem(50, lib_masks, 9));

}

