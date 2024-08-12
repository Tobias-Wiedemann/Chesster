#include "board.h"
#include "perft.h"
#include <iostream>

int main() {
  PerftResults reference;
  reference.number_of_nodes = 4085603;
  reference.number_of_captures = 757163;
  reference.number_of_en_passent = 1929;
  reference.number_of_castles = 128013;
  reference.number_of_promotions = 15172;
  reference.number_of_checks = 25523;
  reference.number_of_checkmates = 43;

  // Perft Position 2
  Position p(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

  Perft myp(p);

  PerftResults res = myp.run(4);

  std::cout << "Should be: \n";
  reference.print();

  std::cout << "Is actually: \n";
  res.print();

  return !(res == reference); // Zero exit code indicates success
}
