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

int get_index(char file, int rank) {
    int index = file - 'a' + 8 * (rank - 1);
    assert(index >= 0);
    assert(index < 64);
    return index;
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

// Yields the most significant one bit
// assumes bb is greater than 0
uint64_t ms1b(uint64_t bb) {
    bb |= bb >> 32;
    bb |= bb >> 16;
    bb |= bb >>  8;
    bb |= bb >>  4;
    bb |= bb >>  2;
    bb |= bb >>  1;
    return (bb >> 1) + 1;
}

// Assumes bb is greater than 0
// maps 0 to 0
int bit_to_index(uint64_t bb) {
    int res = 0;
    while (bb > 1ULL) {
        bb >>= 1;
        res++;
    }
    return res;
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


    // 8x8 Board
    std::vector<Piece> piece_table;
    std::vector<Color> color_table;

    void set_piece(Piece piece, char file, int rank, Color col) {
        //int index = file - 'a' + 8 * (rank - 1);
        int index = get_index(file, rank);
        uint64_t bit = get_bitboard_bit(file, rank);
        color_table[index] = col;

        // Bitboard
        // Add new position
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
        // Bitboard Move
        // Delete old piece
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

    void generate_pawn_moves() {
        generate_pseudo_pawn_moves();
        // clean them up afterwards
    }

    std::list<Move> generate_pseudo_pawn_moves() {
        std::list<Move> res = {};
        if (side_to_move == Color::White) {
            std::cout << "Pawns\n";
            print_bitboard(white_pawns);


            uint64_t pushed_pawns = white_pawns << 8;
            std::cout << "Pushed_pawns\n";
            print_bitboard(pushed_pawns);

            //TODO
            uint64_t empty_squares = ~white_pawns;

            uint64_t pushable_pawns = (empty_squares & pushed_pawns) >> 8;
            std::cout << "Pushable_pawns\n";
            print_bitboard(pushable_pawns);


            pushed_pawns = pushable_pawns << 8;
            std::cout << "Pushed_pawns\n";
            print_bitboard(pushed_pawns);

        return res;
            uint64_t white_pawns_starting_mask = 0x000000000000FF00ULL;

            uint64_t double_push = (white_pawns_starting_mask & white_pawns) << 16;
            std::cout << "Double Push\n";
            print_bitboard(double_push);

            std::cout << "Single and Double Push\n";
            print_bitboard(pushed_pawns | double_push);

            uint64_t white_pawns_promotion_mask = 0xFF00000000000000ULL;
            std::cout << "Promotoion mask\n";
            print_bitboard(white_pawns_promotion_mask);
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
    /*
    Position p;
    p.set_piece(Piece::Pawn, 'e', 2, Color::White);
    p.set_piece(Piece::Pawn, 'd', 3, Color::White);
    p.set_piece(Piece::Pawn, 'c', 4, Color::White);
    p.set_piece(Piece::Pawn, 'f', 7, Color::White);
    p.set_piece(Piece::Pawn, 'g', 7, Color::White);

    p.set_piece(Piece::Pawn, 'c', 5, Color::White);
    print_full_board(p);
    p.generate_pawn_moves();
*/
    uint64_t bb = 0xFFFFFFFFFFFFFFFULL;
    print_bitboard(bb);
    std::cout << "MS1B\n";
    bb = ms1b(bb);
    print_bitboard(bb);
    std::cout << bit_to_index(bb) << "\n";


    return 0;
}
