#include <iostream>
#include "board.h"
#include "perft.h"

int main() {
    PerftResults reference;
    reference.number_of_nodes = 422333;
    reference.number_of_captures = 131393;
    reference.number_of_en_passent = 0;
    reference.number_of_castles = 7795;
    reference.number_of_promotions = 60032;
    reference.number_of_checks = 15492;
    reference.number_of_checkmates = 5;

    // Perft Position 4
    Position p("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    Perft myp(p);

    PerftResults res = myp.run(4);

    std::cout << "Should be: \n";
    reference.print();

    std::cout << "Is actually: \n";
    res.print();

    return !(res == reference); // Zero exit code indicates success
}
