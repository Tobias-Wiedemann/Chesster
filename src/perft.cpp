#include <chrono>
#include <iostream>

#include "board.h"
#include "utils.h"

uint64_t number_of_captures = 0;
uint64_t number_of_checks = 0;
uint64_t number_of_checkmates = 0;
uint64_t number_of_en_passent = 0;
uint64_t number_of_promotions = 0;
uint64_t number_of_castles = 0;

uint64_t perft(int depth, Position &p) {


    std::vector<Move> move_list = p.generate_moves();

    if (depth == 0) {
        if (p.is_check()) {
            number_of_checks++;
        }
        if (move_list.size() == 0) {
            number_of_checkmates++;
        }
        return 1ULL;
    }

    uint64_t nodes = 0ULL;

    for (int i = 0; i < move_list.size(); i++) {
        if (depth == 1) {
            number_of_captures += 
                move_list[i].type == Move_Type::Capture ||
                move_list[i].type == Move_Type::Capture_Promotion ||
                move_list[i].type == Move_Type::En_Passent
                ? 1 : 0;

            number_of_en_passent += 
                move_list[i].type == Move_Type::En_Passent ? 1 : 0;

            number_of_promotions += 
                move_list[i].type == Move_Type::Promotion ||
                move_list[i].type == Move_Type::Capture_Promotion ? 1 : 0;

            number_of_castles += 
                move_list[i].type == Move_Type::Short_Castle ||
                move_list[i].type == Move_Type::Long_Castle ? 1 : 0;
        }
        p.make_move(move_list[i]);
        nodes += perft(depth - 1, p);
        p.unmake_move();
    }

    return nodes;
}

void perft_up_to(int depth, Position p) {
    for (int i = 1; i <= depth; i++) {
        number_of_captures = 0;
        number_of_checks = 0;
        number_of_checkmates = 0;
        number_of_en_passent = 0;
        number_of_promotions = 0;
        number_of_castles = 0;

        auto beg = std::chrono::high_resolution_clock::now();
        uint64_t nodes = perft(i, p);
        auto end = std::chrono::high_resolution_clock::now();
 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
 

        std::cout << "\n";
        std::cout << "Perft on depth " << i << " took " << duration.count() << " microseconds\n";
        std::cout << "Nodes: " << nodes << "\n";
        std::cout << "captures: " << number_of_captures << "\n";
        std::cout << "promotions: " << number_of_promotions << "\n";
        std::cout << "en passents: " << number_of_en_passent << "\n";
        std::cout << "castles: " << number_of_castles << "\n";
        std::cout << "checks: " << number_of_checks << "\n";
        std::cout << "checkmates: " << number_of_checkmates << "\n";
        std::cout << "\n";
    }
}
