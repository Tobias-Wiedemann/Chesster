#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 191;
  reference.number_of_captures = 14;
  reference.number_of_en_passent = 0;
  reference.number_of_castles = 0;
  reference.number_of_promotions = 0;
  reference.number_of_checks = 10;
  reference.number_of_checkmates = 0;

  // Perft Position 3
  Position p("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

  Perft myp(p);

  PerftResults res = myp.run(2);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
