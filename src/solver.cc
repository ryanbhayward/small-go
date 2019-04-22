// Copyright 2019 Chris Solinas
#include "solver.h"
#include <iostream>
#include <algorithm>

Solver::Solver() : nodes(0), verbose(true) { init_theorems_3x3(); }

int Solver::solve(Go *game, Color c) {
  nodes = 0;
  start = Clock::now();
  int max_score = game->size() * game->size();
  int max_depth = 0;
  Result r;

  while (r.is_undefined()) {
    r = alpha_beta(game, c, -1.0 * max_score, 1.0 * max_score, 0, ++max_depth);
    r.pv.push_front(r.best_move);
    if (verbose) {
      display_results(r, max_depth);
    }
  }
  return r.best_move;
}

Result Solver::alpha_beta(Go *game, Color c, float alpha, float beta, int d,
    int max_depth) {

  Result best;
  if (d > max_depth) return best;

  nodes += 1;

  if (game->game_over() || (MAX_NODES > 0 && nodes > MAX_NODES)) {
    best.value = game->score(c);
    best.terminal = true;
    return best;
  }

  if (game->size() == 3) {
    for (Theorem t : theorems_3x3) {
      if (t.applies(game->get_board(), c)) {
        best.value = t.get_value();
        best.terminal = true;
        best.benson = true;
        return best;
      }
    }
  }

  // generate and sort moves
  std::vector<int> moves;
  game->get_moves(&moves);
  // could use some polymorphism here with a general ordering object
  if (game->size() == 3) {
    std::sort(moves.begin(), moves.end(),
      move_ordering_3x3(game->get_board(), c));
  } else if (game->size() == 2) {
    std::sort(moves.begin(), moves.end(), move_ordering_2x2());
  }

  bool undefined = false;
  for (auto move : moves) {
    bool legal = game->make_move(move, c);
    if (!legal) continue;
    Result r = alpha_beta(game, Go::opponent(c), -1 * beta, -1 * alpha, d + 1,
        max_depth);

    r.pv.push_front(r.best_move);
    r.best_move = move;
    // negamax variant
    r.value *= -1;
    game->undo_move();

    if (r.is_undefined()) {
      undefined = true;
      continue;
    }

    if (r > best) best = r;

    if (r.value > alpha) {
      alpha = r.value;
    }
    // pruning
    if (alpha >= beta) break;
  }

  if (!best.benson && undefined) {
    // clear the best move because we can't say anything yet
    best.reset();
  }

  best.benson = false;

  return best;
}

void Solver::display_intermediate() {
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "nodes: " << nodes;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;
}

void Solver::display_results(Result r, int max_depth) {
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "d: " << max_depth;
  if (!r.is_undefined())
    std::cout << " value: " << r.value << " move: " << r.best_move;
  else
    std::cout << " undefined";
  std::cout << " nodes: " << nodes;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;

  if (!r.is_undefined()) {
    std::cout << "pv:";
    for (int m : r.pv) {
      if (m >= -1) std::cout << " " << m << " ";
    }
    std::cout << std::endl;
  }
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

