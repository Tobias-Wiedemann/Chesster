#include "search.h"
#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "utils.h"

int minimax(Position &p, int depth, Move &best_move, int starting_depth, int alpha, int beta) {
  if (depth == 0)
    return evaluate(p);

  int max = alpha;

  std::vector<Move> moves = generate_moves(p);
  if (moves.size() == 0) {
    // basicly checks for mate
    if (p.position_is_legal())
      return -1000000;
    return 0;
  }
  for (auto &m : moves) {
    p.make_move(m);
    int score = -minimax(p, depth - 1, best_move, starting_depth, -beta, -max);
    p.unmake_move();

    if (score > max) {
      max = score;
      if (depth == starting_depth) {
        best_move.from = m.from;
        best_move.to = m.to;
        best_move.promotion = m.promotion;
      }
      if (max >= beta)
        break;
    }
  }
  return max;
}

Move search(Position &p, int depth) {
  Move best_move(0, 0);
  minimax(p, depth, best_move, depth, -1000000, 1000000);
  return best_move;
}
