#include "evaluate.h"
#include "board.h"
#include "utils.h"

int piece_value(Piece p) {
  switch (p) {
  case Piece::Pawn:
    return 1;
  case Piece::Knight:
    return 3;
  case Piece::Bishop:
    return 3;
  case Piece::Rook:
    return 5;
  case Piece::Queen:
    return 9;
  default:
    return 0;
  }
}

int evaluate_material(Position &p) {
  int res = 0;

  for (int i = 0; i < 64; i++) {
    // std::cout << to_string(p.piece_table[i]) << " " <<
    // to_string(p.color_table[i]) << "\n";

    if (p.color_table[i] == Color::Empty)
      continue;

    if (p.color_table[i] == Color::White) {
      res += piece_value(p.piece_table[i]);
    } else {
      res -= piece_value(p.piece_table[i]);
    }
  }

  return res;
}

int evaluate(Position &p) {
  int res = 0;

  res += evaluate_material(p);

  return res;
  return p.side_to_move == Color::White ? res : -res;
}
