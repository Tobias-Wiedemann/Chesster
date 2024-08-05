#include <bits/stdc++.h>
#include <cassert>

struct Move;
struct Position;

uint64_t number_of_captures = 0;

enum class Piece{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Empty
};

enum class Color{
    White,
    Black,
    Empty
};

enum class Move_Type{
    Regular,
    Capture,
    En_Passent,
    Short_Castle,
    Long_Castle,
    Promotion,
    Capture_Promotion
};

std::string to_string(Move_Type m) {
    switch (m) {
        case Move_Type::Regular:
            return "Regular";
        case Move_Type::En_Passent:
            return "En_Passent";
        case Move_Type::Capture:
            return "Capture";
        case Move_Type::Capture_Promotion:
            return "Capture_Promotion";
        case Move_Type::Promotion:
            return "Promotion";
        case Move_Type::Short_Castle:
            return "Short_Castle";
        case Move_Type::Long_Castle:
            return "Long_Castle";
        default:
            return "MOVE TYPE NOT FOUND";
    }
}

std::string to_string(Piece p) {
    switch (p) {
        case Piece::Pawn:
            return "Pawn";
        case Piece::Rook:
            return "Rook";
        case Piece::Knight:
            return "Knight";
        case Piece::Bishop:
            return "Bishop";
        case Piece::Queen:
            return "Queen";
        case Piece::King:
            return "King";
        case Piece::Empty:
            return "Empty";
        default:
            return "PIECE NOT FOUND 1";
    }
}

std::string to_string(Color c) {
    switch (c) {
        case Color::White:
            return "White";
        case Color::Black:
            return "Black";
        case Color::Empty:
            return "Empty";
        default:
            return "Color NOT FOUND";
    }
}

void print_full_board(Position &p);

int get_index(char file, int rank) {
    int index = file - 'a' + 8 * (rank - 1);
    assert(index >= 0);
    assert(index < 64);
    return index;
}

void print_coords_from_index(int index) {
    std::cout << (char)('a' + index % 8) << (index / 8) + 1;
}

uint64_t get_bitboard_bit(char file, int rank) {
    int index = get_index(file, rank);
    uint64_t bit = (uint64_t) std::pow(2, index);
    return bit;
}

void print_bitboard(uint64_t bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (bitboard & (1ULL << square)) {
                std::cout << "1 ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

union long_double {
    double x;
    long y;
};

int fast_log_2(double num) {
    union long_double x = {num};
    return ((x.y >> 52) + 1) & 0x3ff;
}

struct Move {
    Move(int f, int t, Move_Type m = Move_Type::Regular, Piece p = Piece::Empty) : from(f), to(t), type(m), promotion(p), captured_piece(Piece::Empty) {}
    int from;
    int to;
    Piece promotion;
    Move_Type type;
    Piece captured_piece;
};

void print_move(Move m) {
    std::cout << "\nFrom: " << m.from << " to: " << m.to << "\nPromotion: " << to_string(m.promotion) << "\nType: " << to_string(m.type) << "\nCaptured: " << to_string(m.captured_piece) << "\n";
}

struct Position {
    Position() {
        piece_table.assign(64, Piece::Empty);
        color_table.assign(64, Color::Empty);
    }

    Color side_to_move = Color::White;

    // Bitboards
    // White pieces
    uint64_t white_pawns = 0ULL;
    uint64_t white_knights = 0ULL;
    uint64_t white_bishops = 0ULL;
    uint64_t white_rooks = 0ULL;
    uint64_t white_queens = 0ULL;
    uint64_t white_kings = 0ULL;

    // Black pieces
    uint64_t black_pawns = 0ULL;
    uint64_t black_knights = 0ULL;
    uint64_t black_bishops = 0ULL;
    uint64_t black_rooks = 0ULL;
    uint64_t black_queens = 0ULL;
    uint64_t black_kings = 0ULL;

    // Full Board Bitboards
    uint64_t empty_squares = ~0ULL;
    uint64_t occupied_squares = 0ULL;

    // 8x8 Board
    std::vector<Piece> piece_table;
    std::vector<Color> color_table;

    std::vector<Move> move_history;

    bool position_is_legal() {
        std::vector<uint64_t> knight_mask(64, 0ULL);
        knight_mask[0] = 0x0000000000020400ULL;
        knight_mask[1] = 0x0000000000050800ULL;
        knight_mask[2] = 0x00000000000A1100ULL;
        knight_mask[3] = 0x0000000000142200ULL;
        knight_mask[4] = 0x0000000000284400ULL;
        knight_mask[5] = 0x0000000000508800ULL;
        knight_mask[6] = 0x0000000000A01000ULL;
        knight_mask[7] = 0x0000000000402000ULL;
        knight_mask[8] = 0x0000000002040004ULL;
        knight_mask[9] = 0x0000000005080008ULL;
        knight_mask[10] = 0x000000000A110011ULL;
        knight_mask[11] = 0x0000000014220022ULL;
        knight_mask[12] = 0x0000000028440044ULL;
        knight_mask[13] = 0x0000000050880088ULL;
        knight_mask[14] = 0x00000000A0100010ULL;
        knight_mask[15] = 0x0000000040200020ULL;
        knight_mask[16] = 0x0000000204000402ULL;
        knight_mask[17] = 0x0000000508000805ULL;
        knight_mask[18] = 0x0000000A1100110AULL;
        knight_mask[19] = 0x0000001422002214ULL;
        knight_mask[20] = 0x0000002844004428ULL;
        knight_mask[21] = 0x0000005088008850ULL;
        knight_mask[22] = 0x000000A0100010A0ULL;
        knight_mask[23] = 0x0000004020002040ULL;
        knight_mask[24] = 0x0000020400040200ULL;
        knight_mask[25] = 0x0000050800080500ULL;
        knight_mask[26] = 0x00000A1100110A00ULL;
        knight_mask[27] = 0x0000142200221400ULL;
        knight_mask[28] = 0x0000284400442800ULL;
        knight_mask[29] = 0x0000508800885000ULL;
        knight_mask[30] = 0x0000A0100010A000ULL;
        knight_mask[31] = 0x0000402000204000ULL;
        knight_mask[32] = 0x0002040004020000ULL;
        knight_mask[33] = 0x0005080008050000ULL;
        knight_mask[34] = 0x000A1100110A0000ULL;
        knight_mask[35] = 0x0014220022140000ULL;
        knight_mask[36] = 0x0028440044280000ULL;
        knight_mask[37] = 0x0050880088500000ULL;
        knight_mask[38] = 0x00A0100010A00000ULL;
        knight_mask[39] = 0x0040200020400000ULL;
        knight_mask[40] = 0x0204000402000000ULL;
        knight_mask[41] = 0x0508000805000000ULL;
        knight_mask[42] = 0x0A1100110A000000ULL;
        knight_mask[43] = 0x1422002214000000ULL;
        knight_mask[44] = 0x2844004428000000ULL;
        knight_mask[45] = 0x5088008850000000ULL;
        knight_mask[46] = 0xA0100010A0000000ULL;
        knight_mask[47] = 0x4020002040000000ULL;
        knight_mask[48] = 0x0400040200000000ULL;
        knight_mask[49] = 0x0800080500000000ULL;
        knight_mask[50] = 0x1100110A00000000ULL;
        knight_mask[51] = 0x2200221400000000ULL;
        knight_mask[52] = 0x4400442800000000ULL;
        knight_mask[53] = 0x8800885000000000ULL;
        knight_mask[54] = 0x100010A000000000ULL;
        knight_mask[55] = 0x2000204000000000ULL;
        knight_mask[56] = 0x0004020000000000ULL;
        knight_mask[57] = 0x0008050000000000ULL;
        knight_mask[58] = 0x00110A0000000000ULL;
        knight_mask[59] = 0x0022140000000000ULL;
        knight_mask[60] = 0x0044280000000000ULL;
        knight_mask[61] = 0x0088500000000000ULL;
        knight_mask[62] = 0x0010A00000000000ULL;
        knight_mask[63] = 0x0020400000000000ULL;

        // Test if side_to_move could capture the king

        uint64_t king = 
            side_to_move == Color::Black ? 
            white_kings : black_kings;

        int king_index = fast_log_2(king);

        // Pawns
        int index = king_index;

        if (index % 8 < 7) {
            // rightwards (from white's perspective)
            int attacked_index = side_to_move == Color::White ? index - 7 : index + 9;
            if (piece_table[attacked_index] != Piece::Pawn &&
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
        uint64_t possible_knight_checks = knight_mask[king_index];
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

        while (left_down_steps || left_up_steps || right_down_steps || right_up_steps) {
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

    void set_piece(Piece piece, int index, Color col) {
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
        occupied_squares = white_pawns | white_knights | white_rooks | white_bishops | white_queens | white_kings | black_pawns | black_knights | black_rooks | black_bishops | black_queens | black_kings;
        empty_squares = ~occupied_squares;
    }

    void make_move(Move m) {
        move_history.push_back(m);
        Piece moving_piece = piece_table[m.from];

        // delete from initial square
        piece_table[m.from] = Piece::Empty;
        color_table[m.from] = Color::Empty;
        if (side_to_move == Color::White) {
            switch(moving_piece) {
                case Piece::Pawn:
                    white_pawns ^= 1ULL << m.from;
                    break;
                case Piece::Knight:
                    white_knights ^= 1ULL << m.from;
                    break;
                case Piece::Rook:
                    white_rooks ^= 1ULL << m.from;
                    break;
                case Piece::Bishop:
                    white_bishops ^= 1ULL << m.from;
                    break;
                case Piece::Queen:
                    white_queens ^= 1ULL << m.from;
                    break;
                case Piece::King:
                    white_kings ^= 1ULL << m.from;
                    break;
                case Piece::Empty:
                    print_full_board(*this);
                    std::cout << m.from;
                    std::cout << "moving an empty square";
                    std::cout << m.to;
                    for (auto moves : move_history)
                        print_move(moves);
                    exit(1);
                    break;
            }
        } else {
            switch(moving_piece) {
                case Piece::Pawn:
                    black_pawns ^= 1ULL << m.from;
                    break;
                case Piece::Knight:
                    black_knights ^= 1ULL << m.from;
                    break;
                case Piece::Rook:
                    black_rooks ^= 1ULL << m.from;
                    break;
                case Piece::Bishop:
                    black_bishops ^= 1ULL << m.from;
                    break;
                case Piece::Queen:
                    black_queens ^= 1ULL << m.from;
                    break;
                case Piece::King:
                    black_kings ^= 1ULL << m.from;
                    break;
                case Piece::Empty:
                    print_full_board(*this);
                    std::cout << m.from;
                    std::cout << "moving an empty square";
                    std::cout << m.to;
                    for (auto moves : move_history)
                        print_move(moves);
                    exit(1);
                    break;
            }
        }


        // insert to new square
        if (m.type == Move_Type::Promotion ||
            m.type == Move_Type::Capture_Promotion)
            moving_piece = m.promotion;
        piece_table[m.to] = moving_piece;
        color_table[m.to] = side_to_move;
        if (side_to_move == Color::White) {
            switch(moving_piece) {
                case Piece::Pawn:
                    white_pawns ^= 1ULL << m.to;
                    break;
                case Piece::Knight:
                    white_knights ^= 1ULL << m.to;
                    break;
                case Piece::Rook:
                    white_rooks ^= 1ULL << m.to;
                    break;
                case Piece::Bishop:
                    white_bishops ^= 1ULL << m.to;
                    break;
                case Piece::Queen:
                    white_queens ^= 1ULL << m.to;
                    break;
                case Piece::King:
                    white_kings ^= 1ULL << m.to;
                    break;
                case Piece::Empty:
                    print_full_board(*this);
                    std::cout << m.to;
                    std::cout << "inserted nothing";
                    exit(1);
                    break;
            }
        } else {
            switch(moving_piece) {
                case Piece::Pawn:
                    black_pawns ^= 1ULL << m.to;
                    break;
                case Piece::Knight:
                    black_knights ^= 1ULL << m.to;
                    break;
                case Piece::Rook:
                    black_rooks ^= 1ULL << m.to;
                    break;
                case Piece::Bishop:
                    black_bishops ^= 1ULL << m.to;
                    break;
                case Piece::Queen:
                    black_queens ^= 1ULL << m.to;
                    break;
                case Piece::King:
                    black_kings ^= 1ULL << m.to;
                    break;
                case Piece::Empty:
                    print_full_board(*this);
                    std::cout << m.to;
                    std::cout << "inserted nothing";
                    exit(1);
                    break;
            }
        }




        // remove a captured piece
        if (m.type == Move_Type::Capture ||
            m.type == Move_Type::Capture_Promotion) {
            if (side_to_move == Color::Black) {
                switch(m.captured_piece) {
                    case Piece::Pawn:
                        white_pawns ^= 1ULL << m.to;
                        break;
                    case Piece::Knight:
                        white_knights ^= 1ULL << m.to;
                        break;
                    case Piece::Rook:
                        white_rooks ^= 1ULL << m.to;
                        break;
                    case Piece::Bishop:
                        white_bishops ^= 1ULL << m.to;
                        break;
                    case Piece::Queen:
                        white_queens ^= 1ULL << m.to;
                        break;
                    case Piece::King:
                        white_kings ^= 1ULL << m.to;
                        break;
                    case Piece::Empty:
                        print_full_board(*this);
                        std::cout << m.from;
                        std::cout << to_string(m.type);
                        std::cout << m.to;
                        std::cout << to_string(side_to_move);
                        std::cout << "removing an empty square\n";
                        print_bitboard(1ULL << m.from);
                        std::cout << "\n";
                        print_bitboard(1ULL << m.to);

                        for (auto moves : move_history)
                            print_move(moves);
                        exit(1);
                        break;
                }
            } else {
                switch(m.captured_piece) {
                    case Piece::Pawn:
                        black_pawns ^= 1ULL << m.to;
                        break;
                    case Piece::Knight:
                        black_knights ^= 1ULL << m.to;
                        break;
                    case Piece::Rook:
                        black_rooks ^= 1ULL << m.to;
                        break;
                    case Piece::Bishop:
                        black_bishops ^= 1ULL << m.to;
                        break;
                    case Piece::Queen:
                        black_queens ^= 1ULL << m.to;
                        break;
                    case Piece::King:
                        black_kings ^= 1ULL << m.to;
                        break;
                    case Piece::Empty:
                        print_full_board(*this);
                        std::cout << m.from;
                        std::cout << to_string(m.type);
                        std::cout << m.to;
                        std::cout << to_string(side_to_move);
                        std::cout << "removing an empty square\n";
                        print_bitboard(1ULL << m.from);
                        std::cout << "\n";
                        print_bitboard(1ULL << m.to);

                        for (auto moves : move_history)
                            print_move(moves);
                        exit(1);
                        break;
                }
            }
        }

        // En Passent
        if (m.type == Move_Type::En_Passent) {
            int index_to_capture = side_to_move == Color::White ? m.to - 8 : m.to + 8;

            if (side_to_move == Color::White) {
                black_pawns ^= 1ULL << index_to_capture;
            } else {
                white_pawns ^= 1ULL << index_to_capture;
            }

            piece_table[index_to_capture] = Piece::Empty;
            color_table[index_to_capture] = Color::Empty;

        }


        occupied_squares = white_pawns | white_knights | white_rooks | white_bishops | white_queens | white_kings | black_pawns | black_knights | black_rooks | black_bishops | black_queens | black_kings;
        empty_squares = ~occupied_squares;

        if (side_to_move == Color::White) {
            side_to_move = Color::Black;
        } else {
            side_to_move = Color::White;
        }
    }

    void unmake_move() {
        Move m = move_history.back();
        move_history.pop_back();

        // Important to note this happening here
        if (side_to_move == Color::White) {
            side_to_move = Color::Black;
        } else {
            side_to_move = Color::White;
        }

        if (m.type == Move_Type::Regular) {
            set_piece(piece_table[m.to], m.from, side_to_move);
            set_piece(Piece::Empty, m.to, Color::Empty);
        }

        if (m.type == Move_Type::Capture) {
            set_piece(piece_table[m.to], m.from, side_to_move);
            set_piece(m.captured_piece, m.to, side_to_move == Color::White ? Color::Black : Color::White);
        }

        if (m.type == Move_Type::En_Passent) {
            set_piece(Piece::Pawn, m.from, side_to_move);
            int captured_pawn_index = side_to_move == Color::White ? m.to - 8 : m.to + 8;
            set_piece(Piece::Pawn, captured_pawn_index , side_to_move);// == Color::White ? Color::Black : Color::White);
            set_piece(Piece::Empty, captured_pawn_index, Color::Empty);
        }



    }

    std::vector<Move> generate_pawn_moves() {
        return generate_pseudo_pawn_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_pawn_moves() {
        std::vector<Move> res;

        uint64_t white_pawns_starting_mask = 0x000000000000FF00ULL;
        uint64_t black_pawns_starting_mask = 0x00FF000000000000ULL;

        uint64_t pawns;
        uint64_t pushed_pawns;
        uint64_t pushable_pawns;
        uint64_t double_pushable_pawns;

        if (side_to_move == Color::White) {
            pawns = white_pawns;
            pushed_pawns = white_pawns << 8;
            pushable_pawns = (pushed_pawns & empty_squares) >> 8;
            double_pushable_pawns = (((pushable_pawns & white_pawns_starting_mask) << 16) & empty_squares) >> 16;
        } else {
            pawns = black_pawns;
            pushed_pawns = black_pawns >> 8;
            pushable_pawns = (pushed_pawns & empty_squares) << 8;
            double_pushable_pawns = (((pushable_pawns & black_pawns_starting_mask) >> 16) & empty_squares) << 16;
        }


        while (pushable_pawns) {
            int index = fast_log_2(pushable_pawns);

            Move m(index, index);
            m.to = side_to_move == Color::White ? index + 8 : index - 8;
            if (m.to < 56 && m.to > 8) {
                res.push_back(m);
            } else {
                m.type = Move_Type::Promotion;
                m.promotion = Piece::Queen;
                res.push_back(m);
                m.promotion = Piece::Knight;
                res.push_back(m);
                m.promotion = Piece::Rook;
                res.push_back(m);
                m.promotion = Piece::Bishop;
                res.push_back(m);
            }

            pushable_pawns ^= 1ULL << index;
        }

        while (double_pushable_pawns) {
            int index = fast_log_2(double_pushable_pawns);

            Move m(index, index);
            m.to = side_to_move == Color::White ? index + 16 : index - 16;
            res.push_back(m);

            double_pushable_pawns ^= 1ULL << index;
        }

        // captures
        while (pawns) {
            int index = fast_log_2(pawns);

            if (index % 8 < 7) {
                // rightwards (from white's perspective)
                int attacked_index = side_to_move == Color::White ? index + 9 : index - 7;
                if (piece_table[attacked_index] != Piece::Empty &&
                color_table[attacked_index] != side_to_move) {
                    Move m(index, attacked_index, Move_Type::Capture);
                    m.captured_piece = piece_table[attacked_index];
                    if (attacked_index < 56 && attacked_index > 8) {
                        res.push_back(m);
                    } else {
                        m.type = Move_Type::Capture_Promotion;
                        m.promotion = Piece::Queen;
                        res.push_back(m);
                        m.promotion = Piece::Knight;
                        res.push_back(m);
                        m.promotion = Piece::Rook;
                        res.push_back(m);
                        m.promotion = Piece::Bishop;
                        res.push_back(m);
                    }
                }
            }

            if (index % 8 > 0) {
                // leftwards (from white's perspective)
                int attacked_index = side_to_move == Color::White ? index + 7 : index - 9;
                if (piece_table[attacked_index] != Piece::Empty &&
                color_table[attacked_index] != side_to_move) {
                    Move m(index, attacked_index, Move_Type::Capture);
                    m.captured_piece = piece_table[attacked_index];
                    if (attacked_index < 56 && attacked_index > 8) {
                        res.push_back(m);
                    } else {
                        m.type = Move_Type::Capture_Promotion;
                        m.promotion = Piece::Queen;
                        res.push_back(m);
                        m.promotion = Piece::Knight;
                        res.push_back(m);
                        m.promotion = Piece::Rook;
                        res.push_back(m);
                        m.promotion = Piece::Bishop;
                        res.push_back(m);
                    }
                }
            }

            pawns ^= 1ULL << index;
        }

        // En Passent
        if (move_history.size() > 0) {
            Move last_move = move_history.back();
            if (last_move.type == Move_Type::Regular &&
                piece_table[last_move.to] == Piece::Pawn) {
                // capture black pawn
                if (last_move.to >= 32 && 
                    last_move.to <= 39 && 
                    last_move.from >= 48 &&
                    last_move.from <= 55) {

                    if (last_move.to % 8 < 7) {
                        int index = last_move.to + 1;
                        if (piece_table[index] == Piece::Pawn && 
                            color_table[index] == side_to_move) {
                            Move m(index, last_move.to + 8, Move_Type::En_Passent);
                            m.captured_piece = Piece::Pawn;
                            res.push_back(m);
                        }
                    }
                    if (last_move.to % 8 > 0) {
                        int index = last_move.to - 1;
                        if (piece_table[index] == Piece::Pawn && 
                            color_table[index] == side_to_move) {
                            Move m(index, last_move.to + 8, Move_Type::En_Passent);
                            m.captured_piece = Piece::Pawn;
                            res.push_back(m);
                        }
                    }
                }
                // capture white pawn
                if (last_move.to >= 24 && 
                    last_move.to <= 31 && 
                    last_move.from >= 8 &&
                    last_move.from <= 15) {

                    if (last_move.to % 8 < 7) {
                        int index = last_move.to + 1;
                        if (piece_table[index] == Piece::Pawn && 
                            color_table[index] == side_to_move) {
                            Move m(index, last_move.to - 8, Move_Type::En_Passent);
                            m.captured_piece = Piece::Pawn;
                            res.push_back(m);
                        }
                    }
                    if (last_move.to % 8 > 0) {
                        int index = last_move.to - 1;
                        if (piece_table[index] == Piece::Pawn && 
                            color_table[index] == side_to_move) {
                            Move m(index, last_move.to - 8, Move_Type::En_Passent);
                            m.captured_piece = Piece::Pawn;
                            res.push_back(m);
                        }
                    }

                }
            }
        }

        return res;
    }

    std::vector<Move> generate_knight_moves() {
        return generate_pseudo_knight_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_knight_moves() {
        // knight mask
        std::vector<uint64_t> knight_mask(64, 0ULL);
        knight_mask[0] = 0x0000000000020400ULL;
        knight_mask[1] = 0x0000000000050800ULL;
        knight_mask[2] = 0x00000000000A1100ULL;
        knight_mask[3] = 0x0000000000142200ULL;
        knight_mask[4] = 0x0000000000284400ULL;
        knight_mask[5] = 0x0000000000508800ULL;
        knight_mask[6] = 0x0000000000A01000ULL;
        knight_mask[7] = 0x0000000000402000ULL;
        knight_mask[8] = 0x0000000002040004ULL;
        knight_mask[9] = 0x0000000005080008ULL;
        knight_mask[10] = 0x000000000A110011ULL;
        knight_mask[11] = 0x0000000014220022ULL;
        knight_mask[12] = 0x0000000028440044ULL;
        knight_mask[13] = 0x0000000050880088ULL;
        knight_mask[14] = 0x00000000A0100010ULL;
        knight_mask[15] = 0x0000000040200020ULL;
        knight_mask[16] = 0x0000000204000402ULL;
        knight_mask[17] = 0x0000000508000805ULL;
        knight_mask[18] = 0x0000000A1100110AULL;
        knight_mask[19] = 0x0000001422002214ULL;
        knight_mask[20] = 0x0000002844004428ULL;
        knight_mask[21] = 0x0000005088008850ULL;
        knight_mask[22] = 0x000000A0100010A0ULL;
        knight_mask[23] = 0x0000004020002040ULL;
        knight_mask[24] = 0x0000020400040200ULL;
        knight_mask[25] = 0x0000050800080500ULL;
        knight_mask[26] = 0x00000A1100110A00ULL;
        knight_mask[27] = 0x0000142200221400ULL;
        knight_mask[28] = 0x0000284400442800ULL;
        knight_mask[29] = 0x0000508800885000ULL;
        knight_mask[30] = 0x0000A0100010A000ULL;
        knight_mask[31] = 0x0000402000204000ULL;
        knight_mask[32] = 0x0002040004020000ULL;
        knight_mask[33] = 0x0005080008050000ULL;
        knight_mask[34] = 0x000A1100110A0000ULL;
        knight_mask[35] = 0x0014220022140000ULL;
        knight_mask[36] = 0x0028440044280000ULL;
        knight_mask[37] = 0x0050880088500000ULL;
        knight_mask[38] = 0x00A0100010A00000ULL;
        knight_mask[39] = 0x0040200020400000ULL;
        knight_mask[40] = 0x0204000402000000ULL;
        knight_mask[41] = 0x0508000805000000ULL;
        knight_mask[42] = 0x0A1100110A000000ULL;
        knight_mask[43] = 0x1422002214000000ULL;
        knight_mask[44] = 0x2844004428000000ULL;
        knight_mask[45] = 0x5088008850000000ULL;
        knight_mask[46] = 0xA0100010A0000000ULL;
        knight_mask[47] = 0x4020002040000000ULL;
        knight_mask[48] = 0x0400040200000000ULL;
        knight_mask[49] = 0x0800080500000000ULL;
        knight_mask[50] = 0x1100110A00000000ULL;
        knight_mask[51] = 0x2200221400000000ULL;
        knight_mask[52] = 0x4400442800000000ULL;
        knight_mask[53] = 0x8800885000000000ULL;
        knight_mask[54] = 0x100010A000000000ULL;
        knight_mask[55] = 0x2000204000000000ULL;
        knight_mask[56] = 0x0004020000000000ULL;
        knight_mask[57] = 0x0008050000000000ULL;
        knight_mask[58] = 0x00110A0000000000ULL;
        knight_mask[59] = 0x0022140000000000ULL;
        knight_mask[60] = 0x0044280000000000ULL;
        knight_mask[61] = 0x0088500000000000ULL;
        knight_mask[62] = 0x0010A00000000000ULL;
        knight_mask[63] = 0x0020400000000000ULL;



        std::vector<Move> res;

        uint64_t knights = side_to_move == Color::White ? white_knights : black_knights;

        while(knights) {
            int index = fast_log_2(knights);
            if (piece_table[index] != Piece::Knight) {
                std::cout << to_string(piece_table[index]);
                std::cout << to_string(color_table[index]);
                std::cout << to_string(side_to_move);
            }
            uint64_t possible_squares = knight_mask[index];

            while(possible_squares) {
                int current_index = fast_log_2(possible_squares);

                if (piece_table[current_index] == Piece::Empty) {
                    Move m(index, current_index);
                    res.push_back(m);
                } else if (color_table[current_index] != side_to_move) {
                    // capture
                    Move m(index, current_index);
                    if(color_table[current_index] != Color::Empty) {
                        m.type = Move_Type::Capture;
                        m.captured_piece = piece_table[current_index];
                    }
                    res.push_back(m);
                }
                possible_squares ^= 1ULL << current_index;
            }

            knights ^= 1ULL << index;
        }

        return res;
    }


    std::vector<Move> generate_rook_moves() {
        return generate_pseudo_rook_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_rook_moves() {
        std::vector<Move> res;

        uint64_t rooks = side_to_move == Color::White ? white_rooks : black_rooks;

        while(rooks != 0) {
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
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        down_steps--;
                    }
                }
                if (up_steps) {
                    int current_index = index + current_offset * 8;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        up_steps--;
                    }
                }
                if (left_steps) {
                    int current_index = index - current_offset;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_steps--;
                    }
                }
                if (right_steps) {
                    int current_index = index + current_offset;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
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

    std::vector<Move> generate_bishop_moves() {
        return generate_pseudo_bishop_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_bishop_moves() {
        std::vector<Move> res;

        uint64_t bishops = side_to_move == Color::White ? white_bishops : black_bishops;

        while(bishops != 0) {
            int index = fast_log_2(bishops);

            // true until blocked in that direction
            int left_down_steps = std::min(index / 8, index % 8);
            int left_up_steps = std::min(7 - (index / 8), index % 8);
            int right_down_steps = std::min(index / 8, 7 - (index % 8));
            int right_up_steps = std::min(7 - (index / 8), 7 - (index % 8));

            // Important, go from inside outwards
            int current_offset = 1;

            while (left_down_steps || left_up_steps || right_down_steps || right_up_steps) {
                if (left_down_steps) {
                    int current_index = index - current_offset * 9;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_down_steps--;
                    }
                }
                if (left_up_steps) {
                    int current_index = index + current_offset * 7;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_up_steps--;
                    }
                }
                if (right_down_steps) {
                    int current_index = index - current_offset * 7;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        right_down_steps--;
                    }
                }
                if (right_up_steps) {
                    int current_index = index + current_offset * 9;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
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

    std::vector<Move> generate_queen_moves() {
        return generate_pseudo_queen_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_queen_moves() {
        std::vector<Move> res;

        uint64_t queens = side_to_move == Color::White ? white_queens : black_queens;

        // Bishop pattern first, then rook pattern
        while(queens != 0) {
            int index = fast_log_2(queens);

            // true until blocked in that direction
            int left_down_steps = std::min(index / 8, index % 8);
            int left_up_steps = std::min(7 - (index / 8), index % 8);
            int right_down_steps = std::min(index / 8, 7 - (index % 8));
            int right_up_steps = std::min(7 - (index / 8), 7 - (index % 8));


            // Important, go from inside outwards
            int current_offset = 1;

            while (left_down_steps || left_up_steps || right_down_steps || right_up_steps) {
                if (left_down_steps) {
                    int current_index = index - current_offset * 9;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_down_steps--;
                    }
                }
                if (left_up_steps) {
                    int current_index = index + current_offset * 7;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_up_steps--;
                    }
                }
                if (right_down_steps) {
                    int current_index = index - current_offset * 7;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        right_down_steps--;
                    }
                }
                if (right_up_steps) {
                    int current_index = index + current_offset * 9;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        right_up_steps--;
                    }
                }
                current_offset++;
            }
            queens ^= 1ULL << index;
        }

        // resetting for rook pattern
        queens = side_to_move == Color::White ? white_queens : black_queens;


        while(queens != 0) {
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
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        down_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        down_steps--;
                    }
                }
                if (up_steps) {
                    int current_index = index + current_offset * 8;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        up_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        up_steps--;
                    }
                }
                if (left_steps) {
                    int current_index = index - current_offset;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        left_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
                        res.push_back(m);
                        left_steps--;
                    }
                }
                if (right_steps) {
                    int current_index = index + current_offset;
                    uint64_t current_square = (1ULL << current_index) & empty_squares;
                    if (current_square == 0) {
                        // hit something
                        if (color_table[current_index] != side_to_move) {
                            Move m(index, current_index);
                            if(color_table[current_index] != Color::Empty) {
                                m.type = Move_Type::Capture;
                                m.captured_piece = piece_table[current_index];
                            }
                            res.push_back(m);
                        }
                        right_steps = 0;
                    } else {
                        // empty
                        Move m(index, current_index);
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


    std::vector<Move> generate_king_moves() {
        return generate_pseudo_king_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_king_moves() {
        // TODO: I included the square on which the king stands on. That's useless. Remove that for slight optimization
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


        std::vector<Move> res;

        uint64_t king = side_to_move == Color::White ? white_kings : black_kings;
        int index = fast_log_2(king);

        uint64_t moves = king_masks[index];

        while (moves != 0ULL) {
            int current_index = fast_log_2(moves);
            if (piece_table[current_index] == Piece::Empty) {
                Move m(index, current_index);
                res.push_back(m);
            } else if (color_table[current_index] != side_to_move &&
            color_table[current_index] != Color::Empty) {
                // capture
                Move m(index, current_index, Move_Type::Capture);
                res.push_back(m);
            }
            moves ^= 1ULL << current_index;
        }

        return res;
    }


    std::vector<Move> generate_moves() {

        std::vector<Move> pawn_moves = generate_pawn_moves();
        for (auto m : pawn_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "PAWNS BAD ";
        std::vector<Move> knight_moves = generate_knight_moves();
        for (auto m : knight_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "KNIGHTS BAD ";
        std::vector<Move> rook_moves = generate_rook_moves();
        for (auto m : rook_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "ROOKS BAD ";
        std::vector<Move> bishop_moves = generate_bishop_moves();
        for (auto m : bishop_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "BISHOPS BAD ";
        std::vector<Move> queen_moves = generate_queen_moves();
        for (auto m : queen_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "QUEENS BAD ";
        std::vector<Move> king_moves = generate_king_moves();
        for (auto m : king_moves) 
            if (piece_table[m.from] == Piece::Empty)
                std::cout << "KING BAD ";

//        std::cout << std::endl;
        std::vector<Move> res = pawn_moves;
        res.insert(res.end(), 
                   knight_moves.begin(), 
                   knight_moves.end());
        res.insert(res.end(), 
                   rook_moves.begin(), 
                   rook_moves.end());
        res.insert(res.end(), 
                   bishop_moves.begin(), 
                   bishop_moves.end());
        res.insert(res.end(), 
                   queen_moves.begin(), 
                   queen_moves.end());
        res.insert(res.end(), 
                   king_moves.begin(), 
                   king_moves.end());

        std::vector<Move> real_res;
        for (auto m : res) {
            make_move(m);
            if (position_is_legal())
                real_res.push_back(m);
            unmake_move();

        }
        return real_res;
    }
};

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
            char res = color == Color::White ? 'W' :
                       color == Color::Black ? 'B' : '.';
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

void starting_position(Position& p) {

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
    std::vector<uint64_t> knight_moves(64, 0ULL);
    knight_moves[0] = 0x0000000000020400ULL;
    knight_moves[1] = 0x0000000000050800ULL;
    knight_moves[2] = 0x00000000000A1100ULL;
    knight_moves[3] = 0x0000000000142200ULL;
    knight_moves[4] = 0x0000000000284400ULL;
    knight_moves[5] = 0x0000000000508800ULL;
    knight_moves[6] = 0x0000000000A01000ULL;
    knight_moves[7] = 0x0000000000402000ULL;
    knight_moves[8] = 0x0000000002040004ULL;
    knight_moves[9] = 0x0000000005080008ULL;
    knight_moves[10] = 0x000000000A110011ULL;
    knight_moves[11] = 0x0000000014220022ULL;
    knight_moves[12] = 0x0000000028440044ULL;
    knight_moves[13] = 0x0000000050880088ULL;
    knight_moves[14] = 0x00000000A0100010ULL;
    knight_moves[15] = 0x0000000040200020ULL;
    knight_moves[16] = 0x0000000204000402ULL;
    knight_moves[17] = 0x0000000508000805ULL;
    knight_moves[18] = 0x0000000A1100110AULL;
    knight_moves[19] = 0x0000001422002214ULL;
    knight_moves[20] = 0x0000002844004428ULL;
    knight_moves[21] = 0x0000005088008850ULL;
    knight_moves[22] = 0x000000A0100010A0ULL;
    knight_moves[23] = 0x0000004020002040ULL;
    knight_moves[24] = 0x0000020400040200ULL;
    knight_moves[25] = 0x0000050800080500ULL;
    knight_moves[26] = 0x00000A1100110A00ULL;
    knight_moves[27] = 0x0000142200221400ULL;
    knight_moves[28] = 0x0000284400442800ULL;
    knight_moves[29] = 0x0000508800885000ULL;
    knight_moves[30] = 0x0000A0100010A000ULL;
    knight_moves[31] = 0x0000402000204000ULL;
    knight_moves[32] = 0x0002040004020000ULL;
    knight_moves[33] = 0x0005080008050000ULL;
    knight_moves[34] = 0x000A1100110A0000ULL;
    knight_moves[35] = 0x0014220022140000ULL;
    knight_moves[36] = 0x0028440044280000ULL;
    knight_moves[37] = 0x0050880088500000ULL;
    knight_moves[38] = 0x00A0100010A00000ULL;
    knight_moves[39] = 0x0040200020400000ULL;
    knight_moves[40] = 0x0204000402000000ULL;
    knight_moves[41] = 0x0508000805000000ULL;
    knight_moves[42] = 0x0A1100110A000000ULL;
    knight_moves[43] = 0x1422002214000000ULL;
    knight_moves[44] = 0x2844004428000000ULL;
    knight_moves[45] = 0x5088008850000000ULL;
    knight_moves[46] = 0xA0100010A0000000ULL;
    knight_moves[47] = 0x4020002040000000ULL;
    knight_moves[48] = 0x0400040200000000ULL;
    knight_moves[49] = 0x0800080500000000ULL;
    knight_moves[50] = 0x1100110A00000000ULL;
    knight_moves[51] = 0x2200221400000000ULL;
    knight_moves[52] = 0x4400442800000000ULL;
    knight_moves[53] = 0x8800885000000000ULL;
    knight_moves[54] = 0x100010A000000000ULL;
    knight_moves[55] = 0x2000204000000000ULL;
    knight_moves[56] = 0x0004020000000000ULL;
    knight_moves[57] = 0x0008050000000000ULL;
    knight_moves[58] = 0x00110A0000000000ULL;
    knight_moves[59] = 0x0022140000000000ULL;
    knight_moves[60] = 0x0044280000000000ULL;
    knight_moves[61] = 0x0088500000000000ULL;
    knight_moves[62] = 0x0010A00000000000ULL;
    knight_moves[63] = 0x0020400000000000ULL;


    for (int i = 0; i < 64; i++) {
        uint64_t index = 1ULL << i;
        print_bitboard(index);
        std::cout << "\n";
        print_bitboard(knight_moves[i]);
        std::cout << "\npress to confirm\n";
        std::string s;
        std::cin >> s;

    }

}

void go_through_all_king_masks() {
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

uint64_t perft(int depth, Position &p) {
    if (depth == 0)
        return 1ULL;

    std::vector<Move> move_list = p.generate_moves();

    uint64_t nodes = 0ULL;

    for (int i = 0; i < move_list.size(); i++) {
        if (depth == 1) {
            number_of_captures += 
                move_list[i].type == Move_Type::Capture ||
                move_list[i].type == Move_Type::Capture_Promotion ||
                move_list[i].type == Move_Type::En_Passent
                ? 1 : 0;
        }
        p.make_move(move_list[i]);
        nodes += perft(depth - 1, p);
        p.unmake_move();
    }

    return nodes;
}

int main() {

    /*
    print_full_board(p);
    Move m(get_index('d', 2), get_index('d', 3));
    p.make_move(m);
    std::cout << "Move done\n";
    print_full_board(p);

    for (auto pm : p.generate_moves())
         print_move(pm);
         */

    int depth = 5;
//    p.side_to_move = Color::Black;
    for (int i = 1; i <= depth; i++) {
        Position p;
        starting_position(p);
        number_of_captures = 0;
        std::cout << "Perft on depth " << i << ": " << perft(i, p) << "\n";
        std::cout << "captures: " << number_of_captures << "\n";
    }

//    auto first_moves = p.generate_moves();

//    std::cout << "Number of Moves:" << first_moves.size() << std::endl;

    /*
    for (auto it : first_moves) {
        std::cout << "\n";
        print_coords_from_index(it.from);
        std::cout << " ";
        print_coords_from_index(it.to);
        std::cout << " ";
        if (it.promotion != Piece::Empty)
            std::cout << to_string(it.promotion);
    }


    print_full_board(p);
    Move m(get_index('e', 2), get_index('e', 4));
    p.make_move(m);
    print_full_board(p);
    m = Move (get_index('d', 7), get_index('d', 5));
    p.make_move(m);
    print_full_board(p);
    auto moves = p.generate_pawn_moves();
    for (auto g : moves) {
        if (g.type == Move_Type::Capture) {
            std::cout << moves.size();
        }
    }




    */
    std::cout << "\n";




    return 0;
}
