// Copyright 2019 Chris Solinas
#include "solver.h"
#include <iostream>

int Solver::solve(Go *game, Color c) {
  nodes = 0;
  start = Clock::now();
  Result r = alpha_beta(game, c, -1 * MAX_VAL, MAX_VAL, 0);
  if (verbose) {
    display_results(r);
  }
  return r.best_move;
}

Result Solver::alpha_beta(Go *game, Color c, float alpha, float beta, int d) {
  Result best;
  nodes += 1;

  if (game->game_over()) {
    best.value = game->score(c);
    return best;
  }

  // TODO(chris): endgame db

  // generate and sort moves
  std::vector<int> legal_moves;
  game->get_legal_moves(c, &legal_moves);

  // for now just use reverse iterator to do pass first heuristic
  for (auto it = legal_moves.rbegin(); it != legal_moves.rend(); ++it) {
    int move = *it;
    game->make_move(move, c);
    Result r = alpha_beta(game, Go::opponent(c), -1 * beta, -1 * alpha, d + 1);
    r.best_move = move;
    // negamax variant
    r.value *= -1;
    if (move != PASS_IND) game->undo_move();

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

void Solver::display_results(Result r) {
  auto dur = std::chrono::duration_cast<float_seconds>(Clock::now() - start);
  std::cout << "value: " << r.value << " move: " << r.best_move;
  std::cout << " nodes/sec: " << nodes / dur.count() << std::endl;
  std::cout << "pv:";
  for (int m : r.pv) {
    std::cout << " " << m << " ";
  }
  std::cout << std::endl;
}

