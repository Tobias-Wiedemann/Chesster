#pragma once

#include <vector>

#include "board.h"
#include "utils.h"

class MoveGenerator {
public:
  MoveGenerator(Position &pos);

  Position p;

  std::vector<Move> &generate_pawn_moves(std::vector<Move> &res);

  std::vector<Move> &generate_knight_moves(std::vector<Move> &res);

  std::vector<Move> &generate_rook_moves(std::vector<Move> &res);

  std::vector<Move> &generate_bishop_moves(std::vector<Move> &res);

  std::vector<Move> &generate_queen_moves(std::vector<Move> &res);

  std::vector<Move> &generate_king_moves(std::vector<Move> &res);

  std::vector<Move> generate_moves();

private:
  bool is_move_valid(Move &m);
};
