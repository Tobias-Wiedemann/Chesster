#include "move.h"

// filled from left to right
// bits 1 to 6
uint32_t Move::get_from() const { return m & 0x3F; }
void Move::set_from(uint32_t from) { m = (m & 0xFFFFFFC0) | from; }

// bits 7 to 12
uint32_t Move::get_to() const { return (m & 0xFC0) >> 6; }
void Move::set_to(uint32_t to) { m = (m & 0xFFFFF03F) | (to << 6); }

// bits 13 to 15
Piece Move::get_promotion() const {
  switch (m & 0x7000) {
  case 0:
    return Piece::Empty;
  case 0x4000:
    return Piece::Queen;
  case 0x3000:
    return Piece::Knight;
  case 0x2000:
    return Piece::Rook;
  case 0x1000:
    return Piece::Bishop;
  default:
    std::cout << "Invalid promoted piece";
    exit(1);
  }
}
void Move::set_promotion(Piece promotion) {
  switch (promotion) {
  case Piece::Empty:
    m = m & 0xFFFF8FFF;
    break;
  case Piece::Queen:
    m = (m & 0xFFFF8FFF) | 0x4000;
    break;
  case Piece::Knight:
    m = (m & 0xFFFF8FFF) | 0x3000;
    break;
  case Piece::Rook:
    m = (m & 0xFFFF8FFF) | 0x2000;
    break;
  case Piece::Bishop:
    m = (m & 0xFFFF8FFF) | 0x1000;
    break;
  default:
    std::cout << "Invalid promoted piece in set";
    exit(1);
    break;
  }
}

// bits 16 to 18
Piece Move::get_captured_piece() const {
  switch (m & 0x38000) {
  case 0:
    return Piece::Empty;
  case 0x8000:
    return Piece::Pawn;
  case 0x10000:
    return Piece::Knight;
  case 0x18000:
    return Piece::Bishop;
  case 0x20000:
    return Piece::Rook;
  case 0x28000:
    return Piece::Queen;
  default:
    std::cout << "Invalid captured piece";
    exit(1);
  }
}
void Move::set_captured_piece(Piece captured_piece) {
  switch (captured_piece) {
  case Piece::Empty:
    m = (m & 0xFFFC7FFF);
    break;
  case Piece::Pawn:
    m = (m & 0xFFFC7FFF) | 0x8000;
    break;
  case Piece::Knight:
    m = (m & 0xFFFC7FFF) | 0x10000;
    break;
  case Piece::Bishop:
    m = (m & 0xFFFC7FFF) | 0x18000;
    break;
  case Piece::Rook:
    m = (m & 0xFFFC7FFF) | 0x20000;
    break;
  case Piece::Queen:
    m = (m & 0xFFFC7FFF) | 0x28000;
    break;
  default:
    std::cout << "Invalid captured piece";
    exit(1);
  }
}

// TODO: SWAP THIS
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

// bits 25 to 27
Castling Move::get_castling() const {
  switch (m & 0x7000000) {
  case 0:
    return Castling::None;
  case 0x1000000:
    return Castling::WhiteShort;
  case 0x2000000:
    return Castling::WhiteLong;
  case 0x3000000:
    return Castling::BlackShort;
  case 0x4000000:
    return Castling::BlackLong;
  default:
    std::cout << "Invalid castling type";
    exit(1);
  }
}
void Move::set_castling(Castling castling) {
  switch (castling) {
  case Castling::None:
    m = m & 0xF8FFFFFF;
    break;
  case Castling::WhiteShort:
    m = (m & 0xF8FFFFFF) | 0x1000000;
    break;
  case Castling::WhiteLong:
    m = (m & 0xF8FFFFFF) | 0x2000000;
    break;
  case Castling::BlackShort:
    m = (m & 0xF8FFFFFF) | 0x3000000;
    break;
  case Castling::BlackLong:
    m = (m & 0xF8FFFFFF) | 0x4000000;
    break;
  default:
    std::cout << "Invalid castling type";
    exit(1);
  }
}

// bit 28
bool Move::is_rook_destroyed_castle() const { return m & 0x10000000; }
void Move::set_rook_destroyed_castle(bool rook_destroyed_castle) {
  m = (m & 0xEFFFFFFF) | ((uint32_t)rook_destroyed_castle << 28);
}

// bit 29
bool Move::is_king_destroyed_short_castle() const { return m & 0x20000000; }
void Move::set_king_destroyed_short_castle(bool king_destroyed_short_castle) {
  m = (m & 0xDFFFFFFF) | ((uint32_t)king_destroyed_short_castle << 29);
}

// bit 30
bool Move::is_king_destroyed_long_castle() const { return m & 0x40000000; }
void Move::set_king_destroyed_long_castle(bool king_destroyed_long_castle) {
  m = (m & 0xBFFFFFFF) | ((uint32_t)king_destroyed_long_castle << 30);
}
