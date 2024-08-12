#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 264;
  reference.number_of_captures = 87;
  reference.number_of_en_passent = 0;
  reference.number_of_castles = 6;
  reference.number_of_promotions = 48;
  reference.number_of_checks = 10;
  reference.number_of_checkmates = 0;

  // Perft Position 4
  Position p(
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  Perft myp(p);

  PerftResults res = myp.run(2);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
