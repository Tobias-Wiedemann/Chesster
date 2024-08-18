#pragma once

#include "utils.h"

class Move {
public:
  Move(uint32_t f, uint32_t t, Piece promotion = Piece::Empty)
      : m(0), from(f), to(t), promotion(promotion),
        captured_piece(Piece::Empty), previous_en_passent_square(-1),
        castling(Castling::None), rook_destroyed_castle(false),
        king_destroyed_short_castle(false), king_destroyed_long_castle(false) {}

  uint32_t get_from() const;
  void set_from(uint32_t from);

  uint32_t get_to() const;
  void set_to(uint32_t to);

  Piece get_promotion() const;
  void set_promotion(Piece promotion);

  Piece get_captured_piece() const;
  void set_captured_piece(Piece captured_piece);

  bool previous_en_passant_square_existed() const;
  uint32_t get_previous_en_passant_square() const;
  void set_previous_en_passant_square(uint32_t previous_en_passant_square);

  Castling get_castling() const;
  void set_castling(Castling castling);

  bool is_rook_destroyed_castle() const;
  void set_rook_destroyed_castle(bool rook_destroyed_castle);

  bool is_king_destroyed_short_castle() const;
  void set_king_destroyed_short_castle(bool king_destroyed_short_castle);

  bool is_king_destroyed_long_castle() const;
  void set_king_destroyed_long_castle(bool king_destroyed_long_castle);

private:
  uint32_t m;

  int from;
  int to;
  Piece promotion;
  Piece captured_piece;
  int previous_en_passent_square;
  Castling castling;
  bool rook_destroyed_castle;
  bool king_destroyed_short_castle;
  bool king_destroyed_long_castle;
};

void inline print_move(Move m) {
  std::cout << "\nFrom: " << m.get_from() << " to: " << m.get_to()
            << "\nPromotion: " << to_string(m.get_promotion())
            << "\nCaptured: " << to_string(m.get_captured_piece());
}

void inline print_move_compact(Move m) {
  std::cout << get_coords_from_index(m.get_from())
            << get_coords_from_index(m.get_to());
  switch (m.get_promotion()) {
  case Piece::Rook:
    std::cout << "r";
    break;
  case Piece::Knight:
    std::cout << "n";
    break;
  case Piece::Bishop:
    std::cout << "b";
    break;
  case Piece::Queen:
    std::cout << "q";
    break;
  default:
    break;
  }
}

std::string inline to_string(Move m) {
  return std::to_string(m.get_from()) + std::to_string(m.get_to());
}
