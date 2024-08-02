#include <bits/stdc++.h>
#include <cassert>

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
            return "PIECE NOT FOUND";
    }
}

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

uint64_t get_bitboard_bit(int index) {
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
    Move(int f, int t, Piece p = Piece::Empty) : from(f), to(t), promotion(p) {}
    int from;
    int to;
    Piece promotion;
};

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

    void set_piece(Piece piece, char file, int rank, Color col) {
        //int index = file - 'a' + 8 * (rank - 1);
        int index = get_index(file, rank);
        uint64_t bit = get_bitboard_bit(file, rank);
        color_table[index] = col;

        // Bitboard
        // Update combined Bitboards
        empty_squares ^= bit;
        occupied_squares |= bit;

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
                default:
                    std::cout << "PIECE NOT FOUND";
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
                default:
                    std::cout << "PIECE NOT FOUND";
                    return;
            }
        }

        // 8x8 Board
        switch (piece) {
            case Piece::Pawn:
                piece_table[index] = Piece::Pawn;
                break;
            case Piece::Rook:
                piece_table[index] = Piece::Rook;
                break;
            case Piece::Knight:
                piece_table[index] = Piece::Knight;
                break;
            case Piece::Bishop:
                piece_table[index] = Piece::Bishop;
                break;
            case Piece::Queen:
                piece_table[index] = Piece::Queen;
                break;
            case Piece::King:
                piece_table[index] = Piece::King;
                break;
            default:
                std::cout << "PIECE NOT FOUND";
                return;
        }
    }

    void move_piece(Move m) {
        //int from_index = get_index(from_file, from_rank);
        //int to_index = get_index(to_file, to_rank);
        int from_index = m.from;
        int to_index = m.to;

        // 8x8 Board Move
        Piece piece = piece_table[from_index];
        Color moving_color = color_table[from_index];
        Piece moving_piece = piece_table[from_index];
        piece_table[from_index] = Piece::Empty;

        Color captured_color = color_table[to_index];
        Piece captured_piece = piece_table[to_index];
        piece_table[to_index] = moving_piece;

        color_table[from_index] = Color::Empty;
        color_table[to_index] = moving_color;

        uint64_t from_bit = get_bitboard_bit(from_index);
        uint64_t to_bit = get_bitboard_bit(to_index);

        // Bitboard
        // Update combined Bitboards
        empty_squares |= from_bit;
        occupied_squares ^= from_bit;

        // Update individual Bitboards
        if (moving_color == Color::White) {
            switch (piece) {
                case Piece::Pawn:
                    white_pawns ^= from_bit;
                    break;
                case Piece::Rook:
                    white_rooks ^= from_bit;
                    break;
                case Piece::Knight:
                    white_knights ^= from_bit;
                    break;
                case Piece::Bishop:
                    white_bishops ^= from_bit;
                    break;
                case Piece::Queen:
                    white_queens ^= from_bit;
                    break;
                case Piece::King:
                    white_kings ^= from_bit;
                    break;
                default:
                    std::cout << "PIECE NOT FOUND";
                    return;
            }
        } else {
            switch (piece) {
                case Piece::Pawn:
                    black_pawns ^= from_bit;
                    break;
                case Piece::Rook:
                    black_rooks ^= from_bit;
                    break;
                case Piece::Knight:
                    black_knights ^= from_bit;
                    break;
                case Piece::Bishop:
                    black_bishops ^= from_bit;
                    break;
                case Piece::Queen:
                    black_queens ^= from_bit;
                    break;
                case Piece::King:
                    black_kings ^= from_bit;
                    break;
                default:
                    std::cout << "PIECE NOT FOUND";
                    return;
            }
        }

        // Insert new piece

        // Update combined Bitboards
        empty_squares ^= from_bit;
        occupied_squares |= from_bit;

        // Update individual Bitboards
        if (moving_color == Color::White) {
            switch (piece) {
                case Piece::Pawn:
                    white_pawns |= to_bit;
                    break;
                case Piece::Rook:
                    white_rooks |= to_bit;
                    break;
                case Piece::Knight:
                    white_knights |= to_bit;
                    break;
                case Piece::Bishop:
                    white_bishops |= to_bit;
                    break;
                case Piece::Queen:
                    white_queens |= to_bit;
                    break;
                case Piece::King:
                    white_kings |= to_bit;
                    break;
                default:
                    std::cout << "PIECE NOT FOUND";
                    return;
            }
        } else {
            switch (piece) {
                case Piece::Pawn:
                    black_pawns |= to_bit;
                    break;
                case Piece::Rook:
                    black_rooks |= to_bit;
                    break;
                case Piece::Knight:
                    black_knights |= to_bit;
                    break;
                case Piece::Bishop:
                    black_bishops |= to_bit;
                    break;
                case Piece::Queen:
                    black_queens |= to_bit;
                    break;
                case Piece::King:
                    black_kings |= to_bit;
                    break;
                default:
                    std::cout << "PIECE NOT FOUND";
                    return;
            }
        }
    }

    std::vector<Move> generate_pawn_moves() {
        return generate_pseudo_pawn_moves();
        // clean them up afterwards
    }

    std::vector<Move> generate_pseudo_pawn_moves() {
        // TODO: En Passent
        // TODO: Make this shit pretty
        std::vector<Move> res = {};
        if (side_to_move == Color::White) {
            std::cout << "Pawns\n";

            uint64_t pushed_pawns = white_pawns << 8;

            uint64_t pushable_pawns = (empty_squares & pushed_pawns) >> 8;

            pushed_pawns = pushable_pawns << 8;

            while (pushed_pawns != 0ULL) {
                // Get the square index of the most significant bit
                // so of one piece
                int msb_index_from = fast_log_2(pushable_pawns);
                int msb_index_to = fast_log_2(pushed_pawns);

                // remove this bit from the bitboard
                pushed_pawns ^= 1ULL << msb_index_to;

                // same for the starting square
                pushable_pawns ^= 1ULL << msb_index_from;

                Move m(msb_index_from, msb_index_to);
                // Check for promotions
                if (msb_index_to < 56) {
                    res.push_back(m);
                } else {
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

            uint64_t white_pawns_starting_mask = 0x000000000000FF00ULL;


            // Take into account that double pawn pushes do not jump over pieces on the third row
            uint64_t third_row = 0x0000000000FF0000ULL;
            uint64_t third_row_mask = occupied_squares & third_row;
            uint64_t double_pushed_pawns = ((white_pawns_starting_mask & white_pawns) << 16);
            // substract blocked pawns
            double_pushed_pawns &= ~(third_row_mask << 8);

            uint64_t double_pushable_pawns = (empty_squares & double_pushed_pawns) >> 16;

            double_pushed_pawns = double_pushable_pawns << 16;

            // Get the moves out of that, just like for once square
            while (double_pushed_pawns != 0ULL) {
                // basicly iterating over the pieces
                int msb_index_from = fast_log_2(double_pushable_pawns);
                int msb_index_to = fast_log_2(double_pushed_pawns);

                // remove this bit from the bitboard
                double_pushed_pawns ^= 1ULL << msb_index_to;

                // same for the starting square
                double_pushable_pawns ^= 1ULL << msb_index_from;

                Move m(msb_index_from, msb_index_to);
                // No promotions with double push possible
                res.push_back(m);
            }

            // Captures
            uint64_t pawns_to_check = white_pawns;
            while (pawns_to_check > 0ULL) {
                int index = fast_log_2(pawns_to_check);
                uint64_t current_pawn = 1ULL << index;
                pawns_to_check ^= current_pawn;
                auto outside_pawn = index % 8;
                if (outside_pawn > 0 && outside_pawn < 7) {
                    // not outside
                    int attacked_index_left = index + 7;
                    int attacked_index_right = index + 9;

                    if (piece_table[attacked_index_left] != Piece::Empty &&
                    color_table[attacked_index_left] == Color::Black) {
                        Move m(index, attacked_index_left);

                        // Check for promotions
                        if (attacked_index_left < 56) {
                            res.push_back(m);
                        } else {
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

                    if (piece_table[attacked_index_right] != Piece::Empty &&
                    color_table[attacked_index_right] == Color::Black) {
                        Move m(index, attacked_index_right);
                        // Check for promotions
                        if (attacked_index_right < 56) {
                            res.push_back(m);
                        } else {
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

                } else {
                    // outside
                    if (outside_pawn == 0) {
                        // left side of the board
                        int attacked_index_right = index + 9;

                        if (piece_table[attacked_index_right] != Piece::Empty &&
                        color_table[attacked_index_right] == Color::Black) {
                            Move m(index, attacked_index_right);
                            // Check for promotions
                            if (attacked_index_right < 56) {
                                res.push_back(m);
                            } else {
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
                    } else {
                        // right side of the board
                        int attacked_index_left = index + 7;
                        if (piece_table[attacked_index_left] != Piece::Empty &&
                        color_table[attacked_index_left] == Color::Black) {
                            Move m(index, attacked_index_left);
                            // Check for promotions
                            if (attacked_index_left < 56) {
                                res.push_back(m);
                            } else {
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
                }
            }
        } else {
            // check for black pawn moves

            std::cout << "Pawns\n";

            uint64_t pushed_pawns = black_pawns >> 8;

            uint64_t pushable_pawns = (empty_squares & pushed_pawns) << 8;

            pushed_pawns = pushable_pawns >> 8;

            while (pushed_pawns != 0ULL) {
                // Get the square index of the most significant bit
                // so of one piece
                int msb_index_from = fast_log_2(pushable_pawns);
                int msb_index_to = fast_log_2(pushed_pawns);

                // remove this bit from the bitboard
                pushed_pawns ^= 1ULL << msb_index_to;

                // same for the starting square
                pushable_pawns ^= 1ULL << msb_index_from;

                Move m(msb_index_from, msb_index_to);
                // Check for promotions
                if (msb_index_to > 7) {
                    res.push_back(m);
                } else {
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

            uint64_t black_pawns_starting_mask = 0x00FF000000000000ULL;


            // Take into account that double pawn pushes do not jump over pieces on the third row
            uint64_t sixth_row = 0x0000FF0000000000ULL;
            uint64_t sixth_row_mask = occupied_squares & sixth_row;
            uint64_t double_pushed_pawns = ((black_pawns_starting_mask & black_pawns) >> 16);
            // substract blocked pawns
            double_pushed_pawns &= ~(sixth_row_mask >> 8);

            uint64_t double_pushable_pawns = (empty_squares & double_pushed_pawns) << 16;

            double_pushed_pawns = double_pushable_pawns >> 16;

            // Get the moves out of that, just like for once square
            while (double_pushed_pawns != 0ULL) {
                // basicly iterating over the pieces
                int msb_index_from = fast_log_2(double_pushable_pawns);
                int msb_index_to = fast_log_2(double_pushed_pawns);

                // remove this bit from the bitboard
                double_pushed_pawns ^= 1ULL << msb_index_to;

                // same for the starting square
                double_pushable_pawns ^= 1ULL << msb_index_from;

                Move m(msb_index_from, msb_index_to);
                // No promotions with double push possible
                res.push_back(m);
            }

            // Captures
            uint64_t pawns_to_check = black_pawns;
            while (pawns_to_check > 0ULL) {
                int index = fast_log_2(pawns_to_check);
                uint64_t current_pawn = 1ULL << index;
                pawns_to_check ^= current_pawn;
                auto outside_pawn = index % 8;
                if (outside_pawn > 0 && outside_pawn < 7) {
                    // not outside
                    int attacked_index_left = index - 9;
                    int attacked_index_right = index - 7;

                    if (piece_table[attacked_index_left] != Piece::Empty &&
                    color_table[attacked_index_left] == Color::White) {
                        Move m(index, attacked_index_left);

                        // Check for promotions
                        if (attacked_index_left > 7) {
                            res.push_back(m);
                        } else {
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

                    if (piece_table[attacked_index_right] != Piece::Empty &&
                    color_table[attacked_index_right] == Color::White) {
                        Move m(index, attacked_index_right);
                        // Check for promotions
                        if (attacked_index_right > 7) {
                            res.push_back(m);
                        } else {
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

                } else {
                    // outside
                    if (outside_pawn == 0) {
                        // left side of the board
                        int attacked_index_right = index - 7;

                        if (piece_table[attacked_index_right] != Piece::Empty &&
                        color_table[attacked_index_right] == Color::White) {
                            Move m(index, attacked_index_right);
                            // Check for promotions
                            if (attacked_index_right > 7) {
                                res.push_back(m);
                            } else {
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
                    } else {
                        // right side of the board
                        int attacked_index_left = index - 9;
                        if (piece_table[attacked_index_left] != Piece::Empty &&
                        color_table[attacked_index_left] == Color::White) {
                            Move m(index, attacked_index_left);
                            // Check for promotions
                            if (attacked_index_left > 7) {
                                res.push_back(m);
                            } else {
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



        std::vector<Move> res;

        if (side_to_move == Color::White) {
            uint64_t white_knights_copy = white_knights;

            while(white_knights_copy > 0ULL) {
                // iterate over all knights
                int current_knight_index = fast_log_2(white_knights);
                uint64_t current_mask = knight_moves[current_knight_index];
                while(current_mask > 0ULL) {
                    // iterate over all possible moves of the current knight
                    int current_attack_index = fast_log_2(current_mask);
                    if (color_table[current_attack_index] == Color::White) {
                        // can not capture white pieces
                    } else if (color_table[current_attack_index] == Color::Black) {
                        // capture
                        Piece p = piece_table[current_attack_index];
                        switch (p) {
                            case Piece::Pawn:
                                black_pawns ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Rook:
                                black_rooks ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Knight:
                                black_knights ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Bishop:
                                black_bishops ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Queen:
                                black_queens ^= 1ULL << current_attack_index;
                                break;
                            case Piece::King:
                                black_kings ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Empty:
                                std::cout << "PANIC, EMPTY PIECE CAPTURED\n";
                                break;
                            default:
                                std::cout << "PANIC, invalid piece in 8x8\n";
                        }
                        Move m(current_knight_index, current_attack_index);
                        res.push_back(m);
                    } else {
                        // empty square
                        Move m(current_knight_index, current_attack_index);
                        res.push_back(m);
                    }
                    current_mask ^= 1ULL << current_attack_index;
                }
                white_knights_copy ^= 1ULL << current_knight_index;
            }
        } else {

            uint64_t black_knights_copy = black_knights;

            while(black_knights_copy > 0ULL) {
                // iterate over all knights
                int current_knight_index = fast_log_2(black_knights);
                uint64_t current_mask = knight_moves[current_knight_index];
                while(current_mask > 0ULL) {
                    // iterate over all possible moves of the current knight
                    int current_attack_index = fast_log_2(current_mask);
                    if (color_table[current_attack_index] == Color::Black) {
                        // can not capture white pieces
                        // TODO: make this nicer
                    } else if (color_table[current_attack_index] == Color::White) {
                        // capture
                        Piece p = piece_table[current_attack_index];
                        switch (p) {
                            case Piece::Pawn:
                                white_pawns ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Rook:
                                white_rooks ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Knight:
                                white_knights ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Bishop:
                                white_bishops ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Queen:
                                white_queens ^= 1ULL << current_attack_index;
                                break;
                            case Piece::King:
                                white_kings ^= 1ULL << current_attack_index;
                                break;
                            case Piece::Empty:
                                std::cout << "PANIC, EMPTY PIECE CAPTURED\n";
                                break;
                            default:
                                std::cout << "PANIC, invalid piece in 8x8\n";
                        }
                        Move m(current_knight_index, current_attack_index);
                        res.push_back(m);
                    } else {
                        // empty square
                        Move m(current_knight_index, current_attack_index);
                        res.push_back(m);
                    }
                    current_mask ^= 1ULL << current_attack_index;
                }
                black_knights_copy ^= 1ULL << current_knight_index;
            }
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
            } else if (color_table[current_index] != side_to_move) {
                // capture
                Move m(index, current_index);
                res.push_back(m);
            }
            moves ^= 1ULL << current_index;
        }

        return res;
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

    p.set_piece(Piece::Pawn, 'a', 2, Color::White);
    p.set_piece(Piece::Pawn, 'b', 2, Color::White);
    p.set_piece(Piece::Pawn, 'c', 2, Color::White);
    p.set_piece(Piece::Pawn, 'd', 2, Color::White);
    p.set_piece(Piece::Pawn, 'e', 2, Color::White);
    p.set_piece(Piece::Pawn, 'f', 2, Color::White);
    p.set_piece(Piece::Pawn, 'g', 2, Color::White);
    p.set_piece(Piece::Pawn, 'h', 2, Color::White);

    p.set_piece(Piece::Rook, 'a', 1, Color::White);
    p.set_piece(Piece::Rook, 'h', 1, Color::White);

    p.set_piece(Piece::Knight, 'b', 1, Color::White);
    p.set_piece(Piece::Knight, 'g', 1, Color::White);

    p.set_piece(Piece::Bishop, 'c', 1, Color::White);
    p.set_piece(Piece::Bishop, 'f', 1, Color::White);

    p.set_piece(Piece::Queen, 'd', 1, Color::White);

    p.set_piece(Piece::King, 'e', 1, Color::White);


    p.set_piece(Piece::Pawn, 'a', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'b', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'c', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'd', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'e', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'f', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'g', 7, Color::Black);
    p.set_piece(Piece::Pawn, 'h', 7, Color::Black);

    p.set_piece(Piece::Rook, 'a', 8, Color::Black);
    p.set_piece(Piece::Rook, 'h', 8, Color::Black);

    p.set_piece(Piece::Knight, 'b', 8, Color::Black);
    p.set_piece(Piece::Knight, 'g', 8, Color::Black);

    p.set_piece(Piece::Bishop, 'c', 8, Color::Black);
    p.set_piece(Piece::Bishop, 'f', 8, Color::Black);

    p.set_piece(Piece::Queen, 'd', 8, Color::Black);

    p.set_piece(Piece::King, 'e', 8, Color::Black);
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
        p.move_piece(m);
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

int main() {
    Position p;

    p.set_piece(Piece::King, 'a', 8, Color::White);
    p.set_piece(Piece::Rook, 'b', 8, Color::White);

//    p.side_to_move = Color::Black;

    auto first_moves = p.generate_king_moves();


    std::cout << "Number of Moves:" << first_moves.size() << std::endl;

    for (auto it : first_moves) {
        std::cout << "\n";
        print_coords_from_index(it.from);
        std::cout << " ";
        print_coords_from_index(it.to);
        std::cout << " ";
        if (it.promotion != Piece::Empty)
            std::cout << to_string(it.promotion);
    }
    std::cout << "\n";



/*
    int index = 14;
    uint64_t bb = 1ULL << index;
    uint64_t mask = 0x0000000000E0E0E0ULL;


    print_bitboard(bb);
    std::cout << "\n";
    print_bitboard(mask);

    std::cout << index << "\n";
*/


    /*
    p.set_piece(Piece::Pawn, 'a', 2, Color::White);
    p.set_piece(Piece::Pawn, 'b', 2, Color::White);
    p.set_piece(Piece::Pawn, 'c', 2, Color::White);
    p.set_piece(Piece::Pawn, 'a', 3, Color::White);
    p.set_piece(Piece::Pawn, 'b', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'c', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'h', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'h', 7, Color::Black);

    p.side_to_move = Color::Black;

    print_full_board(p);
    auto first_moves = p.generate_pawn_moves();

    std::cout << "Number of Moves:" << first_moves.size() << std::endl;
    for (auto it : first_moves) {
        std::cout << "\n";
        print_coords_from_index(it.from);
        std::cout << " ";
        print_coords_from_index(it.to);
        std::cout << " ";
        if (it.promotion != Piece::Empty)
            std::cout << to_string(it.promotion);
    }
    std::cout << "\n";

    */


    return 0;
}
