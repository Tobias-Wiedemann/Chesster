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

    std::list<Move> generate_pawn_moves() {
        return generate_pseudo_pawn_moves();
        // clean them up afterwards
    }

    std::list<Move> generate_pseudo_pawn_moves() {
        std::list<Move> res = {};
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
            // TODO
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

int main() {


    std::cout << fast_log_2(1) << "\n";

    Position p;
    /*
    p.set_piece(Piece::Pawn, 'e', 2, Color::White);
    p.set_piece(Piece::Pawn, 'c', 2, Color::White);
    p.set_piece(Piece::Pawn, 'd', 2, Color::White);
    p.set_piece(Piece::Pawn, 'd', 3, Color::Black);
*/
    p.set_piece(Piece::Pawn, 'a', 2, Color::White);
    p.set_piece(Piece::Pawn, 'b', 2, Color::White);
    p.set_piece(Piece::Pawn, 'c', 2, Color::White);
    p.set_piece(Piece::Pawn, 'a', 3, Color::White);
    p.set_piece(Piece::Pawn, 'b', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'c', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'h', 3, Color::Black);
    p.set_piece(Piece::Pawn, 'h', 7, Color::Black);

    p.side_to_move = Color::Black;

//    p.set_piece(Piece::Pawn, 'd', 3, Color::White);
//    p.set_piece(Piece::Pawn, 'c', 4, Color::White);
//    p.set_piece(Piece::Pawn, 'f', 7, Color::White);
//    p.set_piece(Piece::Pawn, 'g', 7, Color::White);

//    p.set_piece(Piece::Pawn, 'c', 5, Color::White);
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


    /*
    std::cout << "empty\n";
    print_bitboard(p.empty_squares);
    std::cout << "occupied\n";
    print_bitboard(p.occupied_squares);

p.move_piece(Move(get_index('c', 2), get_index('c', 3)));

    std::cout << "empty\n";
    print_bitboard(p.empty_squares);
    std::cout << "occupied\n";
    print_bitboard(p.occupied_squares);

    print_full_board(p);
*/

    /*
    uint64_t bb = 0xFFFFFFFFFFFFFFFFULL;
    print_bitboard(bb);
    std::cout << "MS1B\n";
    bb = ms1b(bb);
    print_bitboard(bb);
    std::cout << bit_to_index(bb) << "\n";
    std::cout << ld(bb) << "\n";
*/


    return 0;
}
