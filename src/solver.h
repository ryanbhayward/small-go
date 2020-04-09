// Copyright 2019 Chris Solinas
#pragma once

#include<chrono>
#include <list>
#include <map>
#include "Go.h"
#include "theorems.h"

typedef std::chrono::system_clock Clock;
typedef std::chrono::duration<float> float_seconds;

constexpr long MAX_NODES = 0;
constexpr int UNDEFINED = -2;

struct Result {
  Result() : value(-1*MAX_VAL), best_move(UNDEFINED), terminal(false),
    benson(false) {}
  float value;
  int best_move;
  bool terminal;
  bool benson;
  std::list<int> pv;

  bool is_undefined() { return !terminal; }

  void reset() {
    value = -1 * MAX_VAL;
    best_move = UNDEFINED;
    terminal = false;
    benson = false;
  }

  // overload relational operators to make search cleaner
  friend inline bool operator<(const Result& l, const Result& r) {
    return l.value < r.value;
  }
  friend inline bool operator>(const Result& l, const Result& r) {
    return r < l;
  }
  friend inline bool operator<=(const Result& l, const Result& r) {
    return !(l > r);
  }
  friend inline bool operator>=(const Result& l, const Result& r) {
    return !(l < r);
  }
};

struct TT_entry {
  TT_entry() {}
  TT_entry(Result r, Color c, int d) : res(r), to_move(c), max_depth(d) {}
  Result res;
  Color to_move;
  int max_depth;
};

static constexpr int side_rank[9] = {0, 1, 0, 1, 2, 1, 0, 1, 0};

static int killer_table[MAX_DEPTH] = {-2};

struct move_ordering_3x3 {
 private:
  Board& b;
  Color c;
  int depth;

 public:
  move_ordering_3x3(Board& _b, Color _c, int d) : b(_b), c(_c), depth(d) {}

  // i > j functor for move ordering
  bool operator()(int i, int j) const {
    Board b_i(b), b_j(b);
    bool res_i, res_j;
    res_i = b_i.move(i, c);
    res_j = b_j.move(j, c);

    // if one of them is the killer and legal, it has highest priority
    if (res_i && killer_table[depth] == i && !b_i.atari(i)) return true;
    if (res_j && killer_table[depth] == j && !b_j.atari(j)) return false;


    // make sure the moves are legal and they don't put c into atari
    if (!res_i || b_i.atari(i)) return false;
    if (!res_j || b_j.atari(j)) return true;

    // compare scores for area gained heuristic
    float score_i = b_i.score(c);
    float score_j = b_j.score(c);
    if (score_i > score_j) return true;
    if (score_i < score_j) return false;

    if (i < 0) return true;
    if (j < 0) return false;

    // break ties with location of move heuristic
    return side_rank[i] > side_rank[j];
  }
};

struct move_ordering_2x2 {
  bool operator()(int i, int j) const {
    return i != j && i == PASS_IND;
  }
};

class Solver {
 private:
  long nodes;
  bool verbose;
  Clock::time_point start;
  std::vector<Theorem*> theorems_3x3;
  std::vector<int> theorem_hits;
  std::map<long, TT_entry> TT;
  Result alpha_beta(Go *game, Color c, float alpha, float beta, int depth,
      int max_depth);
  void display_results(Result r, int max_depth, int board_size);
  void init_theorems_3x3();
  void clean_theorems_3x3();

 public:
  Solver();
  int solve(Go *game, Color c);
  int solve(Go *game, Color c, int max_depth);
};
