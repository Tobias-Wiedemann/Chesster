#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 9467;
  reference.number_of_captures = 1021;
  reference.number_of_en_passent = 4;
  reference.number_of_castles = 0;
  reference.number_of_promotions = 120;
  reference.number_of_checks = 38;
  reference.number_of_checkmates = 22;

  // Perft Position 4
  Position p(
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  Perft myp(p);

  PerftResults res = myp.run(3);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
