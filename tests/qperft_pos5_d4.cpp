#include "board.h"
#include "perft.h"

int main() {

  // Starting Position
  Position p("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

  Perft myp(p);

  return 2103487 != myp.run_fast(4);
}
