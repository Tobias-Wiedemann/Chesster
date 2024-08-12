#include <chrono>
#include <iostream>

#include "board.h"
#include "movegen.h"
#include "perft.h"
#include "utils.h"

Perft::Perft(Position pos) { p = pos; }

uint64_t Perft::run_fast(int depth) {
  MoveGenerator movegen(p);

  std::vector<Move> move_list = movegen.generate_moves();

  if (depth == 0) {
    return 1ULL;
  }

  uint64_t nodes = 0;

  for (auto m : move_list) {
    p.make_move(m);
    nodes += run_fast(depth - 1);
    p.unmake_move();
  }

  return nodes;
}

PerftResults Perft::run(int depth) {

  res = PerftResults();

  res.number_of_nodes = run_wrapped(depth);

  return res;
}

uint64_t Perft::run_wrapped(int depth) {
  MoveGenerator movegen(p);

  std::vector<Move> move_list = movegen.generate_moves();

  if (depth == 0) {
    if (p.is_check()) {
      res.number_of_checks++;
    }
    if (move_list.size() == 0) {
      res.number_of_checkmates++;
    }
    return 1ULL;
  }

  uint64_t nodes = 0ULL;

  for (int i = 0; i < move_list.size(); i++) {
    if (depth == 1) {
      res.number_of_captures +=
          move_list[i].type == Move_Type::Capture ||
                  move_list[i].type == Move_Type::Capture_Promotion ||
                  move_list[i].type == Move_Type::En_Passent
              ? 1
              : 0;

      res.number_of_en_passent +=
          move_list[i].type == Move_Type::En_Passent ? 1 : 0;

      res.number_of_promotions +=
          move_list[i].type == Move_Type::Promotion ||
                  move_list[i].type == Move_Type::Capture_Promotion
              ? 1
              : 0;

      res.number_of_castles +=
          move_list[i].type == Move_Type::Short_Castle ||
                  move_list[i].type == Move_Type::Long_Castle
              ? 1
              : 0;
    }
    p.make_move(move_list[i]);
    nodes += run_wrapped(depth - 1);
    p.unmake_move();
  }

  return nodes;
}

void Perft::run_up_to(int depth, Position p) {
  for (int i = 1; i <= depth; i++) {

    res = PerftResults();

    auto beg = std::chrono::high_resolution_clock::now();
    res.number_of_nodes = run_wrapped(i);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - beg);

    std::cout << "\n";
    std::cout << "Perft on depth " << i << " took " << duration.count()
              << " microseconds\n";
    std::cout << "Nodes: " << res.number_of_nodes << "\n";
    std::cout << "captures: " << res.number_of_captures << "\n";
    std::cout << "promotions: " << res.number_of_promotions << "\n";
    std::cout << "en passents: " << res.number_of_en_passent << "\n";
    std::cout << "castles: " << res.number_of_castles << "\n";
    std::cout << "checks: " << res.number_of_checks << "\n";
    std::cout << "checkmates: " << res.number_of_checkmates << "\n";
    std::cout << "\n";
  }
}
