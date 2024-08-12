#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 43238;
  reference.number_of_captures = 3348;
  reference.number_of_en_passent = 123;
  reference.number_of_castles = 2;
  reference.number_of_promotions = 0;
  reference.number_of_checks = 1680;
  reference.number_of_checkmates = 17;

  // Perft Position 3
  Position p("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

  Perft myp(p);

  PerftResults res = myp.run(4);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
