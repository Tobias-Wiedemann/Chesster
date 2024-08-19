#pragma once

#include <vector>

#include "board.h"
#include "utils.h"

std::vector<Move> &generate_pawn_moves(std::vector<Move> &res, Position &p);

std::vector<Move> &generate_knight_moves(std::vector<Move> &res, Position &p);

std::vector<Move> &generate_rook_moves(std::vector<Move> &res, Position &p);

std::vector<Move> &generate_bishop_moves(std::vector<Move> &res, Position &p);

std::vector<Move> &generate_queen_moves(std::vector<Move> &res, Position &p);

std::vector<Move> &generate_king_moves(std::vector<Move> &res, Position &p);

std::vector<Move> generate_moves(Position &p);

bool is_move_valid(Move &m, Position &p);
