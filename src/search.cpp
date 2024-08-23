#include "search.h"
#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "utils.h"
#include <algorithm>

void order_moves(Position &p, std::vector<Move> moves) {
  sort(moves.begin(), moves.end(), [&](Move &m1, Move &m2) {
    bool isCapture1 = p.piece_table[m1.to] != Piece::Empty;
    bool isCapture2 = p.piece_table[m2.to] != Piece::Empty;
    return isCapture1 > isCapture2;
  });
}

int search_captures(Position &p, int alpha, int beta) {
  int evaluation = evaluate(p);
  if (evaluation >= beta)
    return beta;
  alpha = std::max(evaluation, alpha);

  // generate captures
  std::vector<Move> moves = generate_captures(p);
  if (moves.size() == 0) {
    // basicly checks for mate
    if (p.position_is_legal())
      return -2000000000;
    return 0;
  }
  for (auto &m : moves) {
    p.make_move(m);
    evaluation = -search_captures(p, -beta, -alpha);
    p.unmake_move();

    if (evaluation >= beta)
      break;

    alpha = std::max(evaluation, alpha);
  }
  return alpha;
}

int minimax(Position &p, int depth, Move &best_move, int starting_depth,
            int alpha, int beta) {
  if (depth == 0)
    return search_captures(p, alpha, beta);

  int max = alpha;

  std::vector<Move> moves = generate_moves(p);
  order_moves(p, moves);
  if (moves.size() == 0) {
    // basicly checks for mate
    if (p.position_is_legal())
      return -2000000000;
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
  minimax(p, depth, best_move, depth, -2000000001, 2000000001);
  return best_move;
}
