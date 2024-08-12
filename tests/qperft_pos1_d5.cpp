#include "board.h"
#include "perft.h"

int main() {

  // Starting Position
  Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  Perft myp(p);

  return 4865609 != myp.run_fast(5);
}
