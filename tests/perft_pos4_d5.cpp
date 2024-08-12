#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 15833292;
  reference.number_of_captures = 2046173;
  reference.number_of_en_passent = 6512;
  reference.number_of_castles = 0;
  reference.number_of_promotions = 329464;
  reference.number_of_checks = 200568;
  reference.number_of_checkmates = 50562;

  // Perft Position 4
  Position p(
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  Perft myp(p);

  PerftResults res = myp.run(5);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
