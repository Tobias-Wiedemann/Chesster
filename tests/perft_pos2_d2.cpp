#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 2039;
  reference.number_of_captures = 351;
  reference.number_of_en_passent = 1;
  reference.number_of_castles = 91;
  reference.number_of_promotions = 0;
  reference.number_of_checks = 3;
  reference.number_of_checkmates = 0;

  // Perft Position 2
  Position p(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

  Perft myp(p);

  PerftResults res = myp.run(2);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
