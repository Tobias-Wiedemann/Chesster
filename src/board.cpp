#include <bits/stdc++.h>
#include <sstream>

#include "attack_masks.h"
#include "board.h"

Position::Position() {
  piece_table.assign(64, Piece::Empty);
  color_table.assign(64, Color::Empty);
}

Position::Position(std::string fen_position) {
  piece_table.assign(64, Piece::Empty);
  color_table.assign(64, Color::Empty);

  std::stringstream fen_stream(fen_position);

  std::string token;
  std::vector<std::string> fen_elements;
  char delimiter = ' ';

  while (getline(fen_stream, token, delimiter)) {
    fen_elements.push_back(token);
  }

  std::string board = fen_elements[0];

  char file = 'a';
  int rank = 8;
  for (char c : board) {
    if (c == '/') {
      file = 'a';
      rank--;
      continue;
    }

    if (c >= '1' && c <= '8') {
      file += c - '0';
      continue;
    }

    // Black Pieces
    if (c == 'r') {
      this->set_piece(Piece::Rook, get_index(file, rank), Color::Black);
      file++;
      continue;
    }
    if (c == 'n') {
      this->set_piece(Piece::Knight, get_index(file, rank), Color::Black);
      file++;
      continue;
    }
    if (c == 'b') {
      this->set_piece(Piece::Bishop, get_index(file, rank), Color::Black);
      file++;
      continue;
    }
    if (c == 'q') {
      this->set_piece(Piece::Queen, get_index(file, rank), Color::Black);
      file++;
      continue;
    }
    if (c == 'k') {
      this->set_piece(Piece::King, get_index(file, rank), Color::Black);
      file++;
      continue;
    }
    if (c == 'p') {
      this->set_piece(Piece::Pawn, get_index(file, rank), Color::Black);
      file++;
      continue;
    }

    // White Pieces
    if (c == 'R') {
      this->set_piece(Piece::Rook, get_index(file, rank), Color::White);
      file++;
      continue;
    }
    if (c == 'N') {
      this->set_piece(Piece::Knight, get_index(file, rank), Color::White);
      file++;
      continue;
    }
    if (c == 'B') {
      this->set_piece(Piece::Bishop, get_index(file, rank), Color::White);
      file++;
      continue;
    }
    if (c == 'Q') {
      this->set_piece(Piece::Queen, get_index(file, rank), Color::White);
      file++;
      continue;
    }
    if (c == 'K') {
      this->set_piece(Piece::King, get_index(file, rank), Color::White);
      file++;
      continue;
    }
    if (c == 'P') {
      this->set_piece(Piece::Pawn, get_index(file, rank), Color::White);
      file++;
      continue;
    }
  }

  if (fen_elements[1] == "w") {
    side_to_move = Color::White;
  } else if (fen_elements[1] == "b") {
    side_to_move = Color::Black;
  } else {
    std::cout << "FEN error, invalid side to move\n";
    exit(1);
  }

  if (fen_elements[2].size() == 4) {
    white_kingside_castling_right = true;
    white_queenside_castling_right = true;
    black_kingside_castling_right = true;
    black_queenside_castling_right = true;
  } else {
    for (auto c : fen_elements[2]) {
      if (c == 'K')
        white_kingside_castling_right = true;
      if (c == 'Q')
        white_queenside_castling_right = true;

      if (c == 'k')
        black_kingside_castling_right = true;
      if (c == 'q')
        black_queenside_castling_right = true;
    }
  }
}

bool Position::is_check() {
  if (side_to_move == Color::White) {
    side_to_move = Color::Black;
  } else {
    side_to_move = Color::White;
  }

  bool res = position_is_legal() == false;

  if (side_to_move == Color::White) {
    side_to_move = Color::Black;
  } else {
    side_to_move = Color::White;
  }

  return res;
}

bool Position::position_is_legal() {
  // Test if side_to_move could capture the king

  uint64_t king = side_to_move == Color::Black ? white_kings : black_kings;

  int king_index = fast_log_2(king);

  // Pawns
  int index = king_index;

  if (index % 8 < 7) {
    // rightwards (from white's perspective)
    int attacked_index = side_to_move == Color::White ? index - 7 : index + 9;
    if (piece_table[attacked_index] == Piece::Pawn &&
        color_table[attacked_index] == side_to_move) {
      return false;
    }
  }
  if (index % 8 > 0) {
    // leftwards (from white's perspective)
    int attacked_index = side_to_move == Color::White ? index - 9 : index + 7;
    if (piece_table[attacked_index] == Piece::Pawn &&
        color_table[attacked_index] == side_to_move) {
      return false;
    }
  }
  // Pawns survived

  // Knights
  uint64_t possible_knight_checks = knight_masks[king_index];
  while (possible_knight_checks) {
    int current_index = fast_log_2(possible_knight_checks);
    if (color_table[current_index] != side_to_move) {
      // no danger here
    } else if (piece_table[current_index] == Piece::Knight) {
      // king dead
      return false;
    }
    possible_knight_checks ^= 1ULL << current_index;
  }

  // king survived knights

  // now "queen move gen" for king

  // first bishop style

  index = king_index;
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
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Bishop ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        left_down_steps = 0;
      } else {
        // empty
        left_down_steps--;
      }
    }
    if (left_up_steps) {
      int current_index = index + current_offset * 7;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Bishop ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        left_up_steps = 0;
      } else {
        // empty
        left_up_steps--;
      }
    }
    if (right_down_steps) {
      int current_index = index - current_offset * 7;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Bishop ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        right_down_steps = 0;
      } else {
        // empty
        right_down_steps--;
      }
    }
    if (right_up_steps) {
      int current_index = index + current_offset * 9;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Bishop ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        right_up_steps = 0;
      } else {
        // empty
        right_up_steps--;
      }
    }
    current_offset++;
  }

  // now rook style
  index = king_index;

  // true until blocked in that direction
  int down_steps = index / 8;
  int up_steps = 7 - down_steps;
  int left_steps = index % 8;
  int right_steps = 7 - left_steps;

  // Important, go from inside outwards
  current_offset = 1;

  while (down_steps || up_steps || left_steps || right_steps) {
    if (down_steps) {
      int current_index = index - current_offset * 8;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Rook ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        down_steps = 0;
      } else {
        // empty
        down_steps--;
      }
    }
    if (up_steps) {
      int current_index = index + current_offset * 8;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Rook ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        up_steps = 0;
      } else {
        // empty
        up_steps--;
      }
    }
    if (left_steps) {
      int current_index = index - current_offset;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Rook ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        left_steps = 0;
      } else {
        // empty
        left_steps--;
      }
    }
    if (right_steps) {
      int current_index = index + current_offset;
      uint64_t current_square = (1ULL << current_index) & empty_squares;
      if (current_square == 0) {
        // hit something
        if (color_table[current_index] == side_to_move) {
          // danger
          if (piece_table[current_index] == Piece::Rook ||
              piece_table[current_index] == Piece::Queen) {
            // dead
            return false;
          }
        }
        right_steps = 0;
      } else {
        // empty
        right_steps--;
      }
    }
    current_offset++;
  }

  return true;
}

void Position::set_piece(Piece piece, int index, Color col) {
  uint64_t bit = 1ULL << index;

  // remove the previous piece from bitboards
  if (piece_table[index] != Piece::Empty) {
    if (color_table[index] == Color::White) {
      switch (piece_table[index]) {
      case Piece::Pawn:
        white_pawns ^= bit;
        break;
      case Piece::Rook:
        white_rooks ^= bit;
        break;
      case Piece::Knight:
        white_knights ^= bit;
        break;
      case Piece::Bishop:
        white_bishops ^= bit;
        break;
      case Piece::Queen:
        white_queens ^= bit;
        break;
      case Piece::King:
        white_kings ^= bit;
        break;
      default:
        std::cout << "PIECE NOT FOUND 2 1";
        return;
      }
    } else {
      switch (piece_table[index]) {
      case Piece::Pawn:
        black_pawns ^= bit;
        break;
      case Piece::Rook:
        black_rooks ^= bit;
        break;
      case Piece::Knight:
        black_knights ^= bit;
        break;
      case Piece::Bishop:
        black_bishops ^= bit;
        break;
      case Piece::Queen:
        black_queens ^= bit;
        break;
      case Piece::King:
        black_kings ^= bit;
        break;
      default:
        std::cout << "PIECE NOT FOUND 3 1";
        return;
      }
    }
  }

  piece_table[index] = piece;
  color_table[index] = col;

  // innserting in Bitboards
  // Update individual Bitboards
  if (col == Color::White) {
    switch (piece) {
    case Piece::Pawn:
      white_pawns |= bit;
      break;
    case Piece::Rook:
      white_rooks |= bit;
      break;
    case Piece::Knight:
      white_knights |= bit;
      break;
    case Piece::Bishop:
      white_bishops |= bit;
      break;
    case Piece::Queen:
      white_queens |= bit;
      break;
    case Piece::King:
      white_kings |= bit;
      break;
    case Piece::Empty:
      break;
    default:
      std::cout << "PIECE NOT FOUND 2";
      return;
    }
  } else {
    switch (piece) {
    case Piece::Pawn:
      black_pawns |= bit;
      break;
    case Piece::Rook:
      black_rooks |= bit;
      break;
    case Piece::Knight:
      black_knights |= bit;
      break;
    case Piece::Bishop:
      black_bishops |= bit;
      break;
    case Piece::Queen:
      black_queens |= bit;
      break;
    case Piece::King:
      black_kings |= bit;
      break;
    case Piece::Empty:
      break;
    default:
      std::cout << "PIECE NOT FOUND 3";
      return;
    }
  }

  // Update combined Bitboards (lazily)
  occupied_squares = white_pawns | white_knights | white_rooks | white_bishops |
                     white_queens | white_kings | black_pawns | black_knights |
                     black_rooks | black_bishops | black_queens | black_kings;
  empty_squares = ~occupied_squares;
}

void Position::make_move(Move m) {
  move_history.push_back(m);
  Piece moving_piece = piece_table[m.from];

  // delete from initial square
  set_piece(Piece::Empty, m.from, Color::Empty);

  if (moving_piece == Piece::Empty) {
    print_full_board(*this);
    std::cout << m.from;
    std::cout << "moving an empty square";
    std::cout << m.to;
    std::cout << "\nwhite ks castle" << white_kingside_castling_right;
    std::cout << "\nwhite qs castle" << white_queenside_castling_right;
    std::cout << "\nblack ks castle" << black_kingside_castling_right;
    std::cout << "\nblack qs castle" << black_queenside_castling_right;
    for (auto moves : move_history)
      print_move(moves);
    exit(1);
  }

  // insert to new square
  if (m.type == Move_Type::Promotion || m.type == Move_Type::Capture_Promotion)
    moving_piece = m.promotion;
  set_piece(moving_piece, m.to, side_to_move);

  // En Passent
  if (m.type == Move_Type::En_Passent) {
    int index_to_capture = side_to_move == Color::White ? m.to - 8 : m.to + 8;
    set_piece(Piece::Empty, index_to_capture, Color::Empty);
  } else if (m.type == Move_Type::Short_Castle) {
    int king_index = side_to_move == Color::White ? 4 : 60;
    set_piece(Piece::Empty, king_index, Color::Empty);
    set_piece(Piece::Empty, king_index + 3, Color::Empty);
    set_piece(Piece::King, king_index + 2, side_to_move);
    set_piece(Piece::Rook, king_index + 1, side_to_move);
  } else if (m.type == Move_Type::Long_Castle) {
    int king_index = side_to_move == Color::White ? 4 : 60;
    set_piece(Piece::Empty, king_index, Color::Empty);
    set_piece(Piece::Empty, king_index - 4, Color::Empty);
    set_piece(Piece::King, king_index - 2, side_to_move);
    set_piece(Piece::Rook, king_index - 1, side_to_move);
  }

  if (side_to_move == Color::White) {
    if (m.destroyed_kingside_castling)
      white_kingside_castling_right = false;
    if (m.destroyed_queenside_castling)
      white_queenside_castling_right = false;
  } else {
    if (m.destroyed_kingside_castling)
      black_kingside_castling_right = false;
    if (m.destroyed_queenside_castling)
      black_queenside_castling_right = false;
  }

  if (side_to_move == Color::White) {
    side_to_move = Color::Black;
  } else {
    side_to_move = Color::White;
  }
}

void Position::unmake_move() {
  Move m = move_history.back();

  // Important to note this happening here
  if (side_to_move == Color::White) {
    side_to_move = Color::Black;
  } else {
    side_to_move = Color::White;
  }

  if (m.type == Move_Type::Regular) {
    set_piece(piece_table[m.to], m.from, side_to_move);
    set_piece(Piece::Empty, m.to, Color::Empty);
  } else if (m.type == Move_Type::Capture) {
    set_piece(piece_table[m.to], m.from, side_to_move);
    set_piece(m.captured_piece, m.to,
              side_to_move == Color::White ? Color::Black : Color::White);
  } else if (m.type == Move_Type::En_Passent) {
    set_piece(Piece::Pawn, m.from, side_to_move);
    int captured_pawn_index =
        side_to_move == Color::White ? m.to - 8 : m.to + 8;
    set_piece(Piece::Pawn, captured_pawn_index,
              side_to_move == Color::White ? Color::Black : Color::White);
    set_piece(Piece::Empty, m.to, Color::Empty);
  } else if (m.type == Move_Type::Promotion) {
    set_piece(Piece::Pawn, m.from, side_to_move);
    set_piece(Piece::Empty, m.to, Color::Empty);
  } else if (m.type == Move_Type::Capture_Promotion) {
    set_piece(Piece::Pawn, m.from, side_to_move);
    set_piece(m.captured_piece, m.to,
              side_to_move == Color::White ? Color::Black : Color::White);
  } else if (m.type == Move_Type::Short_Castle) {
    int king_index = side_to_move == Color::White ? 6 : 62;
    set_piece(Piece::Empty, king_index, Color::Empty);
    set_piece(Piece::Empty, king_index - 1, Color::Empty);
    set_piece(Piece::King, king_index - 2, side_to_move);
    set_piece(Piece::Rook, king_index + 1, side_to_move);
  } else if (m.type == Move_Type::Long_Castle) {
    int king_index = side_to_move == Color::White ? 2 : 58;
    set_piece(Piece::Empty, king_index, Color::Empty);
    set_piece(Piece::Empty, king_index + 1, Color::Empty);
    set_piece(Piece::King, king_index + 2, side_to_move);
    set_piece(Piece::Rook, king_index - 2, side_to_move);
  }

  if (side_to_move == Color::White) {
    if (m.destroyed_kingside_castling)
      white_kingside_castling_right = true;
    if (m.destroyed_queenside_castling)
      white_queenside_castling_right = true;
  } else {
    if (m.destroyed_kingside_castling)
      black_kingside_castling_right = true;
    if (m.destroyed_queenside_castling)
      black_queenside_castling_right = true;
  }

  move_history.pop_back();
}

bool is_consistant(Position &p) {
  std::string bbs = "";
  for (int rank = 7; rank >= 0; --rank) {
    for (int file = 0; file < 8; ++file) {
      int square = rank * 8 + file;
      if (p.black_pawns & (1ULL << square)) {
        bbs += "p ";
      } else if (p.black_knights & (1ULL << square)) {
        bbs += "n ";
      } else if (p.black_bishops & (1ULL << square)) {
        bbs += "b ";
      } else if (p.black_rooks & (1ULL << square)) {
        bbs += "r ";
      } else if (p.black_queens & (1ULL << square)) {
        bbs += "q ";
      } else if (p.black_kings & (1ULL << square)) {
        bbs += "k ";
      } else if (p.white_pawns & (1ULL << square)) {
        bbs += "P ";
      } else if (p.white_knights & (1ULL << square)) {
        bbs += "N ";
      } else if (p.white_bishops & (1ULL << square)) {
        bbs += "B ";
      } else if (p.white_rooks & (1ULL << square)) {
        bbs += "R ";
      } else if (p.white_queens & (1ULL << square)) {
        bbs += "Q ";
      } else if (p.white_kings & (1ULL << square)) {
        bbs += "K ";
      } else {
        bbs += ". ";
      }
    }
    bbs += "\n";
  }

  std::string tbs = "";
  for (int rank = 8; rank >= 1; --rank) {
    for (char file = 'a'; file <= 'h'; ++file) {
      int index = get_index(file, rank);
      auto piece = p.piece_table[index];
      char res;
      switch (piece) {
      case Piece::Pawn:
        res = 'p';
        break;
      case Piece::Rook:
        res = 'r';
        break;
      case Piece::Knight:
        res = 'n';
        break;
      case Piece::Bishop:
        res = 'b';
        break;
      case Piece::Queen:
        res = 'q';
        break;
      case Piece::King:
        res = 'k';
        break;
      case Piece::Empty:
        res = '.';
        break;
      default:
        std::cout << "PANIC, invalid piece in 8x8\n";
      }
      if (p.color_table[index] == Color::White) {
        res -= 32;
      }
      tbs += res;

      tbs += " ";
    }
    tbs += "\n";
  }
  bool result = bbs == tbs;
  if (!result) {
    std::cout << "bbs\n" << bbs;
    std::cout << "\ntbs\n" << tbs;
  }
  return result;
}

void print_all_bitboards(Position &p) {

  std::cout << "white pawns\n";
  print_bitboard(p.white_pawns);
  std::cout << "white knights\n";
  print_bitboard(p.white_knights);
  std::cout << "white bishops\n";
  print_bitboard(p.white_bishops);
  std::cout << "white rooks\n";
  print_bitboard(p.white_rooks);
  std::cout << "white queen\n";
  print_bitboard(p.white_queens);
  std::cout << "white king\n";
  print_bitboard(p.white_kings);

  std::cout << "black pawns\n";
  print_bitboard(p.black_pawns);
  std::cout << "black knights\n";
  print_bitboard(p.black_knights);
  std::cout << "black bishops\n";
  print_bitboard(p.black_bishops);
  std::cout << "black rooks\n";
  print_bitboard(p.black_rooks);
  std::cout << "black queen\n";
  print_bitboard(p.black_queens);
  std::cout << "black king\n";
  print_bitboard(p.black_kings);
}

void print_full_board(Position &p) {
  std::cout << "\nBitboard-based\n";
  for (int rank = 7; rank >= 0; --rank) {
    for (int file = 0; file < 8; ++file) {
      int square = rank * 8 + file;
      if (p.black_pawns & (1ULL << square)) {
        std::cout << "p ";
      } else if (p.black_knights & (1ULL << square)) {
        std::cout << "n ";
      } else if (p.black_bishops & (1ULL << square)) {
        std::cout << "b ";
      } else if (p.black_rooks & (1ULL << square)) {
        std::cout << "r ";
      } else if (p.black_queens & (1ULL << square)) {
        std::cout << "q ";
      } else if (p.black_kings & (1ULL << square)) {
        std::cout << "k ";
      } else if (p.white_pawns & (1ULL << square)) {
        std::cout << "P ";
      } else if (p.white_knights & (1ULL << square)) {
        std::cout << "N ";
      } else if (p.white_bishops & (1ULL << square)) {
        std::cout << "B ";
      } else if (p.white_rooks & (1ULL << square)) {
        std::cout << "R ";
      } else if (p.white_queens & (1ULL << square)) {
        std::cout << "Q ";
      } else if (p.white_kings & (1ULL << square)) {
        std::cout << "K ";
      } else {
        std::cout << ". ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "\n8x8-based\n";
  for (int rank = 8; rank >= 1; --rank) {
    for (char file = 'a'; file <= 'h'; ++file) {
      int index = get_index(file, rank);
      auto piece = p.piece_table[index];
      char res;
      switch (piece) {
      case Piece::Pawn:
        res = 'p';
        break;
      case Piece::Rook:
        res = 'r';
        break;
      case Piece::Knight:
        res = 'n';
        break;
      case Piece::Bishop:
        res = 'b';
        break;
      case Piece::Queen:
        res = 'q';
        break;
      case Piece::King:
        res = 'k';
        break;
      case Piece::Empty:
        res = '.';
        break;
      default:
        std::cout << "PANIC, invalid piece in 8x8\n";
        return;
      }
      if (p.color_table[index] == Color::White) {
        res -= 32;
      }
      std::cout << res << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "\n8x8-based color\n";
  for (int rank = 8; rank >= 1; --rank) {
    for (char file = 'a'; file <= 'h'; ++file) {
      int index = get_index(file, rank);
      auto color = p.color_table[index];
      char res = color == Color::White   ? 'W'
                 : color == Color::Black ? 'B'
                                         : '.';
      std::cout << res << " ";
    }
    std::cout << std::endl;
  }
}

Position starting_bitboards() {
  Position p;
  // White pieces
  p.white_pawns = 0x000000000000FF00ULL;
  p.white_knights = 0x0000000000000042ULL;
  p.white_bishops = 0x0000000000000024ULL;
  p.white_rooks = 0x0000000000000081ULL;
  p.white_queens = 0x0000000000000008ULL;
  p.white_kings = 0x0000000000000010ULL;

  // Black pieces
  p.black_pawns = 0x00FF000000000000ULL;
  p.black_knights = 0x4200000000000000ULL;
  p.black_bishops = 0x2400000000000000ULL;
  p.black_rooks = 0x8100000000000000ULL;
  p.black_queens = 0x0800000000000000ULL;
  p.black_kings = 0x1000000000000000ULL;

  return p;
}

void starting_position(Position &p) {

  p.set_piece(Piece::Pawn, get_index('a', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('b', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('c', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('d', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('e', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('f', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('g', 2), Color::White);
  p.set_piece(Piece::Pawn, get_index('h', 2), Color::White);

  p.set_piece(Piece::Rook, get_index('a', 1), Color::White);
  p.set_piece(Piece::Rook, get_index('h', 1), Color::White);

  p.set_piece(Piece::Knight, get_index('b', 1), Color::White);
  p.set_piece(Piece::Knight, get_index('g', 1), Color::White);

  p.set_piece(Piece::Bishop, get_index('c', 1), Color::White);
  p.set_piece(Piece::Bishop, get_index('f', 1), Color::White);

  p.set_piece(Piece::Queen, get_index('d', 1), Color::White);

  p.set_piece(Piece::King, get_index('e', 1), Color::White);

  p.set_piece(Piece::Pawn, get_index('a', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('b', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('c', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('d', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('e', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('f', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('g', 7), Color::Black);
  p.set_piece(Piece::Pawn, get_index('h', 7), Color::Black);

  p.set_piece(Piece::Rook, get_index('a', 8), Color::Black);
  p.set_piece(Piece::Rook, get_index('h', 8), Color::Black);

  p.set_piece(Piece::Knight, get_index('b', 8), Color::Black);
  p.set_piece(Piece::Knight, get_index('g', 8), Color::Black);

  p.set_piece(Piece::Bishop, get_index('c', 8), Color::Black);
  p.set_piece(Piece::Bishop, get_index('f', 8), Color::Black);

  p.set_piece(Piece::Queen, get_index('d', 8), Color::Black);

  p.set_piece(Piece::King, get_index('e', 8), Color::Black);

  p.white_kingside_castling_right = true;
  p.white_queenside_castling_right = true;
  p.black_kingside_castling_right = true;
  p.black_queenside_castling_right = true;
}

void cmdl_game_loop() {

  Position p;
  starting_position(p);

  print_full_board(p);

  char from;
  int from_row;
  char to;
  int to_row;

  for (;;) {
    std::cin >> from >> from_row >> to >> to_row;
    if (from < 'a' || from > 'h')
      continue;
    if (to < 'a' || to > 'h')
      continue;
    if (from_row < 1 || from_row > 8)
      continue;
    if (to_row < 1 || to_row > 8)
      continue;

    int from_index = get_index(from, from_row);
    int to_index = get_index(to, to_row);
    Move m(from_index, to_index);
    p.make_move(m);
    print_full_board(p);
  }
}

void go_through_all_knight_masks() {
  for (int i = 0; i < 64; i++) {
    uint64_t index = 1ULL << i;
    print_bitboard(index);
    std::cout << "\n";
    print_bitboard(knight_masks[i]);
    std::cout << "\npress to confirm\n";
    std::string s;
    std::cin >> s;
  }
}

void go_through_all_king_masks() {
  for (int i = 0; i < 64; i++) {
    uint64_t index = 1ULL << i;
    print_bitboard(index);
    std::cout << "\n" << i << "\n";
    print_bitboard(king_masks[i]);
    std::cout << "\npress to confirm\n";
    std::string s;
    std::cin >> s;
  }
}
