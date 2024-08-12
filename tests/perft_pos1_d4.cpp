#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 197281;
  reference.number_of_captures = 1576;
  reference.number_of_en_passent = 0;
  reference.number_of_castles = 0;
  reference.number_of_promotions = 0;
  reference.number_of_checks = 469;
  reference.number_of_checkmates = 8;

  // Starting Position
  Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  Perft myp(p);

  PerftResults res = myp.run(4);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
