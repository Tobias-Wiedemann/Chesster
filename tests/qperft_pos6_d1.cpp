#include "board.h"
#include "perft.h"

int main() {

  // Starting Position
  Position p("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

  Perft myp(p);

  return 46 != myp.run_fast(1);
}
