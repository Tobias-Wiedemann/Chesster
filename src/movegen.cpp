#include <csignal>
#include <vector>

#include "attack_masks.h"
#include "board.h"
#include "movegen.h"
#include "utils.h"

bool MoveGenerator::is_move_valid(Move &m) {
  bool res = false;
  p.make_move(m);
  if (p.position_is_legal())
    res = true;
  p.unmake_move();
  return res;
}

MoveGenerator::MoveGenerator(Position &pos) : p(pos) {}

std::vector<Move> &MoveGenerator::generate_pawn_moves(std::vector<Move> &res) {
  uint64_t white_pawns_starting_mask = 0x000000000000FF00ULL;
  uint64_t black_pawns_starting_mask = 0x00FF000000000000ULL;

  uint64_t pawns;
  uint64_t pushed_pawns;
  uint64_t pushable_pawns;
  uint64_t double_pushable_pawns;

  if (p.side_to_move == Color::White) {
    pawns = p.white_pawns;
    pushed_pawns = p.white_pawns << 8;
    pushable_pawns = (pushed_pawns & p.empty_squares) >> 8;
    double_pushable_pawns =
        (((pushable_pawns & white_pawns_starting_mask) << 16) &
         p.empty_squares) >>
        16;
  } else {
    pawns = p.black_pawns;
    pushed_pawns = p.black_pawns >> 8;
    pushable_pawns = (pushed_pawns & p.empty_squares) << 8;
    double_pushable_pawns =
        (((pushable_pawns & black_pawns_starting_mask) >> 16) & p.empty_squares)
        << 16;
  }

  while (pushable_pawns) {
    int index = fast_log_2(pushable_pawns);

    Move m(index, index);
    m.to = p.side_to_move == Color::White ? index + 8 : index - 8;
    if (m.to < 56 && m.to > 8) {
      if (is_move_valid(m))
        res.push_back(m);
    } else {
      m.promotion = Piece::Queen;
      if (is_move_valid(m))
        res.push_back(m);
      m.promotion = Piece::Knight;
      if (is_move_valid(m))
        res.push_back(m);
      m.promotion = Piece::Rook;
      if (is_move_valid(m))
        res.push_back(m);
      m.promotion = Piece::Bishop;
      if (is_move_valid(m))
        res.push_back(m);
    }

    pushable_pawns ^= 1ULL << index;
  }

  while (double_pushable_pawns) {
    int index = fast_log_2(double_pushable_pawns);

    Move m(index, index);
    m.to = p.side_to_move == Color::White ? index + 16 : index - 16;
    if (is_move_valid(m))
      res.push_back(m);

    double_pushable_pawns ^= 1ULL << index;
  }

  // captures
  while (pawns) {
    int index = fast_log_2(pawns);

    if (index % 8 < 7) {
      // rightwards (from white's perspective)
      int attacked_index =
          p.side_to_move == Color::White ? index + 9 : index - 7;
      if (p.piece_table[attacked_index] != Piece::Empty &&
          p.color_table[attacked_index] != p.side_to_move) {
        Move m(index, attacked_index);
        m.captured_piece = p.piece_table[attacked_index];
        if (attacked_index < 56 && attacked_index > 8) {
          if (is_move_valid(m))
            res.push_back(m);
        } else {
          m.promotion = Piece::Queen;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Knight;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Rook;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Bishop;
          if (is_move_valid(m))
            res.push_back(m);
        }
      }
    }

    if (index % 8 > 0) {
      // leftwards (from white's perspective)
      int attacked_index =
          p.side_to_move == Color::White ? index + 7 : index - 9;
      if (p.piece_table[attacked_index] != Piece::Empty &&
          p.color_table[attacked_index] != p.side_to_move) {
        Move m(index, attacked_index);
        m.captured_piece = p.piece_table[attacked_index];
        if (attacked_index < 56 && attacked_index > 8) {
          if (is_move_valid(m))
            res.push_back(m);
        } else {
          m.promotion = Piece::Queen;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Knight;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Rook;
          if (is_move_valid(m))
            res.push_back(m);
          m.promotion = Piece::Bishop;
          if (is_move_valid(m))
            res.push_back(m);
        }
      }
    }

    pawns ^= 1ULL << index;
  }

  // En Passent

  if (p.en_passent_square != -1) {
    if (p.en_passent_square % 8 < 7) {
      int from_index = p.side_to_move == Color::White ? p.en_passent_square - 7
                                                      : p.en_passent_square + 9;
      if (p.piece_table[from_index] == Piece::Pawn &&
          p.color_table[from_index] == p.side_to_move) {
        Move m(from_index, p.en_passent_square);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }
    if (p.en_passent_square % 8 > 0) {
      int from_index = p.side_to_move == Color::White ? p.en_passent_square - 9
                                                      : p.en_passent_square + 7;
      if (p.piece_table[from_index] == Piece::Pawn &&
          p.color_table[from_index] == p.side_to_move) {
        Move m(from_index, p.en_passent_square);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }
  }

  return res;
}

std::vector<Move> &
MoveGenerator::generate_knight_moves(std::vector<Move> &res) {
  uint64_t knights =
      p.side_to_move == Color::White ? p.white_knights : p.black_knights;

  while (knights) {
    int index = fast_log_2(knights);
    if (p.piece_table[index] != Piece::Knight) {
      std::cout << to_string(p.piece_table[index]);
      std::cout << to_string(p.color_table[index]);
      std::cout << to_string(p.side_to_move);
    }
    uint64_t possible_squares = knight_masks[index];

    while (possible_squares) {
      int current_index = fast_log_2(possible_squares);

      if (p.piece_table[current_index] == Piece::Empty) {
        Move m(index, current_index);
        if (is_move_valid(m))
          res.push_back(m);
      } else if (p.color_table[current_index] != p.side_to_move) {
        // capture
        Move m(index, current_index);
        if (p.color_table[current_index] != Color::Empty) {
          m.captured_piece = p.piece_table[current_index];
        }
        if (is_move_valid(m))
          res.push_back(m);
      }
      possible_squares ^= 1ULL << current_index;
    }

    knights ^= 1ULL << index;
  }

  return res;
}

std::vector<Move> &MoveGenerator::generate_rook_moves(std::vector<Move> &res) {
  uint64_t rooks =
      p.side_to_move == Color::White ? p.white_rooks : p.black_rooks;

  while (rooks != 0) {
    int index = fast_log_2(rooks);

    // true until blocked in that direction
    int down_steps = index / 8;
    int up_steps = 7 - down_steps;
    int left_steps = index % 8;
    int right_steps = 7 - left_steps;

    // Important, go from inside outwards
    int current_offset = 1;

    while (down_steps || up_steps || left_steps || right_steps) {
      if (down_steps) {
        int current_index = index - current_offset * 8;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          down_steps--;
        }
      }
      if (up_steps) {
        int current_index = index + current_offset * 8;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          up_steps--;
        }
      }
      if (left_steps) {
        int current_index = index - current_offset;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_steps--;
        }
      }
      if (right_steps) {
        int current_index = index + current_offset;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_steps--;
        }
      }
      current_offset++;
    }
    rooks ^= 1ULL << index;
  }

  return res;
}

std::vector<Move> &
MoveGenerator::generate_bishop_moves(std::vector<Move> &res) {
  uint64_t bishops =
      p.side_to_move == Color::White ? p.white_bishops : p.black_bishops;

  while (bishops != 0) {
    int index = fast_log_2(bishops);

    // true until blocked in that direction
    int left_down_steps = std::min(index / 8, index % 8);
    int left_up_steps = std::min(7 - (index / 8), index % 8);
    int right_down_steps = std::min(index / 8, 7 - (index % 8));
    int right_up_steps = std::min(7 - (index / 8), 7 - (index % 8));

    // Important, go from inside outwards
    int current_offset = 1;

    while (left_down_steps || left_up_steps || right_down_steps ||
           right_up_steps) {
      if (left_down_steps) {
        int current_index = index - current_offset * 9;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_down_steps--;
        }
      }
      if (left_up_steps) {
        int current_index = index + current_offset * 7;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_up_steps--;
        }
      }
      if (right_down_steps) {
        int current_index = index - current_offset * 7;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_down_steps--;
        }
      }
      if (right_up_steps) {
        int current_index = index + current_offset * 9;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_up_steps--;
        }
      }
      current_offset++;
    }
    bishops ^= 1ULL << index;
  }

  return res;
}

std::vector<Move> &MoveGenerator::generate_queen_moves(std::vector<Move> &res) {
  uint64_t queens =
      p.side_to_move == Color::White ? p.white_queens : p.black_queens;

  // Bishop pattern first, then rook pattern
  while (queens != 0) {
    int index = fast_log_2(queens);

    // true until blocked in that direction
    int left_down_steps = std::min(index / 8, index % 8);
    int left_up_steps = std::min(7 - (index / 8), index % 8);
    int right_down_steps = std::min(index / 8, 7 - (index % 8));
    int right_up_steps = std::min(7 - (index / 8), 7 - (index % 8));

    // Important, go from inside outwards
    int current_offset = 1;

    while (left_down_steps || left_up_steps || right_down_steps ||
           right_up_steps) {
      if (left_down_steps) {
        int current_index = index - current_offset * 9;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_down_steps--;
        }
      }
      if (left_up_steps) {
        int current_index = index + current_offset * 7;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_up_steps--;
        }
      }
      if (right_down_steps) {
        int current_index = index - current_offset * 7;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_down_steps--;
        }
      }
      if (right_up_steps) {
        int current_index = index + current_offset * 9;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_up_steps--;
        }
      }
      current_offset++;
    }
    queens ^= 1ULL << index;
  }

  // resetting for rook pattern
  queens = p.side_to_move == Color::White ? p.white_queens : p.black_queens;

  while (queens != 0) {
    int index = fast_log_2(queens);

    // true until blocked in that direction
    int down_steps = index / 8;
    int up_steps = 7 - down_steps;
    int left_steps = index % 8;
    int right_steps = 7 - left_steps;

    // Important, go from inside outwards
    int current_offset = 1;

    while (down_steps || up_steps || left_steps || right_steps) {
      if (down_steps) {
        int current_index = index - current_offset * 8;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          down_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          down_steps--;
        }
      }
      if (up_steps) {
        int current_index = index + current_offset * 8;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          up_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          up_steps--;
        }
      }
      if (left_steps) {
        int current_index = index - current_offset;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          left_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          left_steps--;
        }
      }
      if (right_steps) {
        int current_index = index + current_offset;
        uint64_t current_square = (1ULL << current_index) & p.empty_squares;
        if (current_square == 0) {
          // hit something
          if (p.color_table[current_index] != p.side_to_move) {
            Move m(index, current_index);
            if (p.color_table[current_index] != Color::Empty) {
              m.captured_piece = p.piece_table[current_index];
            }
            if (is_move_valid(m))
              res.push_back(m);
          }
          right_steps = 0;
        } else {
          // empty
          Move m(index, current_index);
          if (is_move_valid(m))
            res.push_back(m);
          right_steps--;
        }
      }
      current_offset++;
    }
    queens ^= 1ULL << index;
  }
  return res;
}

std::vector<Move> &MoveGenerator::generate_king_moves(std::vector<Move> &res) {
  // TODO: I included the square on which the king stands on. That's useless.
  // Remove that for slight optimization
  std::vector<uint64_t> king_masks(64, 0ULL);
  king_masks[0] = 0x0000000000000303ULL;
  king_masks[1] = 0x0000000000000707ULL;
  king_masks[2] = 0x0000000000000E0EULL;
  king_masks[3] = 0x0000000000001C1CULL;
  king_masks[4] = 0x0000000000003838ULL;
  king_masks[5] = 0x0000000000007070ULL;
  king_masks[6] = 0x000000000000E0E0ULL;
  king_masks[7] = 0x000000000000C0C0ULL;
  king_masks[8] = 0x0000000000030303ULL;
  king_masks[9] = 0x0000000000070707ULL;
  king_masks[10] = 0x00000000000E0E0EULL;
  king_masks[11] = 0x00000000001C1C1CULL;
  king_masks[12] = 0x0000000000383838ULL;
  king_masks[13] = 0x0000000000707070ULL;
  king_masks[14] = 0x0000000000E0E0E0ULL;
  king_masks[15] = 0x0000000000C0C0C0ULL;
  king_masks[16] = 0x0000000003030300ULL;
  king_masks[17] = 0x0000000007070700ULL;
  king_masks[18] = 0x000000000E0E0E00ULL;
  king_masks[19] = 0x000000001C1C1C00ULL;
  king_masks[20] = 0x0000000038383800ULL;
  king_masks[21] = 0x0000000070707000ULL;
  king_masks[22] = 0x00000000E0E0E000ULL;
  king_masks[23] = 0x00000000C0C0C000ULL;
  king_masks[24] = 0x0000000303030000ULL;
  king_masks[25] = 0x0000000707070000ULL;
  king_masks[26] = 0x0000000E0E0E0000ULL;
  king_masks[27] = 0x0000001C1C1C0000ULL;
  king_masks[28] = 0x0000003838380000ULL;
  king_masks[29] = 0x0000007070700000ULL;
  king_masks[30] = 0x000000E0E0E00000ULL;
  king_masks[31] = 0x000000C0C0C00000ULL;
  king_masks[32] = 0x0000030303000000ULL;
  king_masks[33] = 0x0000070707000000ULL;
  king_masks[34] = 0x00000E0E0E000000ULL;
  king_masks[35] = 0x00001C1C1C000000ULL;
  king_masks[36] = 0x0000383838000000ULL;
  king_masks[37] = 0x0000707070000000ULL;
  king_masks[38] = 0x0000E0E0E0000000ULL;
  king_masks[39] = 0x0000C0C0C0000000ULL;
  king_masks[40] = 0x0003030300000000ULL;
  king_masks[41] = 0x0007070700000000ULL;
  king_masks[42] = 0x000E0E0E00000000ULL;
  king_masks[43] = 0x001C1C1C00000000ULL;
  king_masks[44] = 0x0038383800000000ULL;
  king_masks[45] = 0x0070707000000000ULL;
  king_masks[46] = 0x00E0E0E000000000ULL;
  king_masks[47] = 0x00C0C0C000000000ULL;
  king_masks[48] = 0x0303030000000000ULL;
  king_masks[49] = 0x0707070000000000ULL;
  king_masks[50] = 0x0E0E0E0000000000ULL;
  king_masks[51] = 0x1C1C1C0000000000ULL;
  king_masks[52] = 0x3838380000000000ULL;
  king_masks[53] = 0x7070700000000000ULL;
  king_masks[54] = 0xE0E0E00000000000ULL;
  king_masks[55] = 0xC0C0C00000000000ULL;
  king_masks[56] = 0x0303000000000000ULL;
  king_masks[57] = 0x0707000000000000ULL;
  king_masks[58] = 0x0E0E000000000000ULL;
  king_masks[59] = 0x1C1C000000000000ULL;
  king_masks[60] = 0x3838000000000000ULL;
  king_masks[61] = 0x7070000000000000ULL;
  king_masks[62] = 0xE0E0000000000000ULL;
  king_masks[63] = 0xC0C0000000000000ULL;

  uint64_t king =
      p.side_to_move == Color::White ? p.white_kings : p.black_kings;
  int index = fast_log_2(king);

  uint64_t moves = king_masks[index];

  while (moves != 0ULL) {
    int current_index = fast_log_2(moves);
    if (p.piece_table[current_index] == Piece::Empty) {
      Move m(index, current_index);

      if (is_move_valid(m))
        res.push_back(m);
    } else if (p.color_table[current_index] != p.side_to_move &&
               p.color_table[current_index] != Color::Empty) {
      // capture
      Move m(index, current_index);
      m.captured_piece = p.piece_table[current_index];

      if (is_move_valid(m))
        res.push_back(m);
    }
    moves ^= 1ULL << current_index;
  }

  // castling
  // filtered for legality already

  if (p.is_check())
    return res;

  if (p.side_to_move == Color::White) {
    if (p.piece_table[4] != Piece::King)
      return res;
    if (p.white_kingside_castling_right) {
      bool can_castle_kingside = p.piece_table[5] == Piece::Empty &&
                                 p.piece_table[6] == Piece::Empty &&
                                 p.piece_table[7] == Piece::Rook &&
                                 p.color_table[7] == p.side_to_move;

      if (can_castle_kingside) {
        Move m(4, 5);
        p.make_move(m);
        can_castle_kingside &= p.position_is_legal();
        p.unmake_move();

        m.to = 6;
        p.make_move(m);
        can_castle_kingside &= p.position_is_legal();
        p.unmake_move();
      }

      if (can_castle_kingside) {
        Move m(4, 6);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }

    if (p.white_queenside_castling_right) {
      bool can_castle_queenside = p.piece_table[3] == Piece::Empty &&
                                  p.piece_table[2] == Piece::Empty &&
                                  p.piece_table[0] == Piece::Rook &&
                                  p.color_table[0] == p.side_to_move;

      if (can_castle_queenside) {
        Move m(4, 3);
        p.make_move(m);
        can_castle_queenside &= p.position_is_legal();
        p.unmake_move();

        m.to = 2;
        p.make_move(m);
        can_castle_queenside &= p.position_is_legal();
        p.unmake_move();
      }

      if (can_castle_queenside) {
        Move m(4, 2);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }
  } else {
    if (p.piece_table[60] != Piece::King)
      return res;
    if (p.black_kingside_castling_right) {
      bool can_castle_kingside = p.piece_table[61] == Piece::Empty &&
                                 p.piece_table[62] == Piece::Empty &&
                                 p.piece_table[63] == Piece::Rook &&
                                 p.color_table[63] == p.side_to_move;

      if (can_castle_kingside) {
        Move m(60, 61);
        p.make_move(m);
        can_castle_kingside &= p.position_is_legal();
        p.unmake_move();

        m.to = 62;
        p.make_move(m);
        can_castle_kingside &= p.position_is_legal();
        p.unmake_move();
      }

      if (can_castle_kingside) {
        Move m(60, 62);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }

    if (p.black_queenside_castling_right) {
      bool can_castle_queenside = p.piece_table[59] == Piece::Empty &&
                                  p.piece_table[58] == Piece::Empty &&
                                  p.piece_table[56] == Piece::Rook &&
                                  p.color_table[56] == p.side_to_move;

      if (can_castle_queenside) {
        Move m(60, 59);
        p.make_move(m);
        can_castle_queenside &= p.position_is_legal();
        p.unmake_move();

        m.to = 58;
        p.make_move(m);
        can_castle_queenside &= p.position_is_legal();
        p.unmake_move();
      }

      if (can_castle_queenside) {
        Move m(60, 58);
        if (is_move_valid(m))
          res.push_back(m);
      }
    }
  }

  return res;
}

std::vector<Move> MoveGenerator::generate_moves() {
  std::vector<Move> res;

  res = generate_pawn_moves(res);
  res = generate_knight_moves(res);
  res = generate_rook_moves(res);
  res = generate_bishop_moves(res);
  res = generate_queen_moves(res);
  res = generate_king_moves(res);

  return res;
}
