#include "search.h"
#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "utils.h"

int negamax(Position &p, int depth, Move &best_move, int starting_depth) {
  if (depth == 0)
    return evaluate(p);

  int max = -1000000;

  std::vector<Move> moves = generate_moves(p);
  if (moves.size() == 0) {
    // basicly checks for mate
    if (p.position_is_legal())
      return max + 1;
    return 0;
  }
  for (auto &m : moves) {
    p.make_move(m);
    int score = -negamax(p, depth - 1, best_move, starting_depth);
    p.unmake_move();

    if (score > max) {
      max = score;
      if (depth == starting_depth) {
        best_move.from = m.from;
        best_move.to = m.to;
        best_move.promotion = m.promotion;
      }
    }
  }
  return max;
}

Move search(Position &p, int depth) {
  Move best_move(0, 0);
  negamax(p, depth, best_move, depth);
  return best_move;
}
