#include "move.h"

// Getter for 'from'
uint32_t Move::get_from() const { return from; }

// Setter for 'from'
void Move::set_from(uint32_t f) { from = f; }

// Getter for 'to'
uint32_t Move::get_to() const { return to; }

// Setter for 'to'
void Move::set_to(uint32_t t) { to = t; }

// Getter for 'promotion'
Piece Move::get_promotion() const { return promotion; }

// Setter for 'promotion'
void Move::set_promotion(Piece p) { promotion = p; }

// Getter for 'captured_piece'
Piece Move::get_captured_piece() const { return captured_piece; }

// Setter for 'captured_piece'
void Move::set_captured_piece(Piece p) { captured_piece = p; }

// Check if 'previous_en_passant_square' existed
bool Move::previous_en_passant_square_existed() const {
  return previous_en_passent_square != static_cast<uint32_t>(-1);
}

// Getter for 'previous_en_passant_square'
uint32_t Move::get_previous_en_passant_square() const {
  return previous_en_passent_square;
}

// Setter for 'previous_en_passant_square'
void Move::set_previous_en_passant_square(uint32_t square) {
  previous_en_passent_square = square;
}

// Getter for 'castling'
Castling Move::get_castling() const { return castling; }

// Setter for 'castling'
void Move::set_castling(Castling c) { castling = c; }

// Getter for 'rook_destroyed_castle'
bool Move::is_rook_destroyed_castle() const { return rook_destroyed_castle; }

// Setter for 'rook_destroyed_castle'
void Move::set_rook_destroyed_castle(bool destroyed) {
  rook_destroyed_castle = destroyed;
}

// Getter for 'king_destroyed_short_castle'
bool Move::is_king_destroyed_short_castle() const {
  return king_destroyed_short_castle;
}

// Setter for 'king_destroyed_short_castle'
void Move::set_king_destroyed_short_castle(bool destroyed) {
  king_destroyed_short_castle = destroyed;
}

// Getter for 'king_destroyed_long_castle'
bool Move::is_king_destroyed_long_castle() const {
  return king_destroyed_long_castle;
}

// Setter for 'king_destroyed_long_castle'
void Move::set_king_destroyed_long_castle(bool destroyed) {
  king_destroyed_long_castle = destroyed;
}
