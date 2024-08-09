#pragma once

#include "utils.h"

struct Position
{
    Position();

    Position(std::string fen_position);

    bool white_kingside_castling_right = false;
    bool white_queenside_castling_right = false;
    bool black_kingside_castling_right = false;
    bool black_queenside_castling_right = false;
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

    bool is_check();

    bool position_is_legal();

    void set_piece(Piece piece, int index, Color col);

    void make_move(Move m);

    void unmake_move();

    std::vector<Move> generate_pawn_moves();

    std::vector<Move> generate_pseudo_pawn_moves();

    std::vector<Move> generate_knight_moves();

    std::vector<Move> generate_pseudo_knight_moves();

    std::vector<Move> generate_rook_moves();

    void rook_castling_right_helper(Move &m);

    std::vector<Move> generate_pseudo_rook_moves();

    std::vector<Move> generate_bishop_moves();

    std::vector<Move> generate_pseudo_bishop_moves();

    std::vector<Move> generate_queen_moves();

    std::vector<Move> generate_pseudo_queen_moves();

    std::vector<Move> generate_king_moves();

    std::vector<Move> generate_pseudo_king_moves();

    std::vector<Move> generate_moves();
};

void print_full_board(Position &p);
bool is_consistant(Position &p);
