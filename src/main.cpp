#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "perft.h"
#include "utils.h"

Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// Position p("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
// Mockup functions
void handleSetOption(const std::string &optionName,
                     const std::string &optionValue) {
  // Implement setting options
}

void applyMove(const std::string &move) {
  // Convert the move string to your Move structure and apply it
  // For example, e2e4 -> Move(from=12, to=28)
}

void handlePosition(const std::string &positionData) {
  std::istringstream iss(positionData);
  std::string token;
  iss >> token; // position type

  bool startpos_flag = false;
  std::string fen = "";
  if (token == "startpos") {
    startpos_flag = true;
    iss >> token;
  } else {
    while (iss >> token && token != "moves") {
      fen += token + " ";
    }
  }
  if (startpos_flag) {
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }
  p = Position(fen);
  if (token == "moves") {
    while (iss >> token) {
      char file1 = token[0];
      char file2 = token[2];
      int rank1 = token[1] - '0';
      int rank2 = token[3] - '0';
      Move m(get_index(file1, rank1), get_index(file2, rank2));
      if (token.size() == 5) {
        switch (token[4]) {
        case 'q':
          m.promotion = Piece::Queen;
          break;
        case 'n':
          m.promotion = Piece::Knight;
          break;
        case 'r':
          m.promotion = Piece::Rook;
          break;
        case 'b':
          m.promotion = Piece::Bishop;
          break;
        default:
          break;
        }
      }
      p.make_move(m);
    }
  }
}

void handleGo(const std::string &goData) {
  // Implement move calculation logic
  std::vector<Move> moves = generate_moves(p);
  int best_index;
  int best_score = -1000;
  for (int i = 0; i < moves.size(); i++) {
    p.make_move(moves[i]);
    if (evaluate(p) > best_score) {
      best_score = evaluate(p);
      best_index = i;
    }
    p.unmake_move();
  }
  Move m = moves[best_index];
  std::cout << "bestmove " << get_coords_from_index(m.from)
            << get_coords_from_index(m.to);
  switch (m.promotion) {
  case Piece::Queen:
    std::cout << "q";
    break;
  case Piece::Knight:
    std::cout << "n";
    break;
  case Piece::Rook:
    std::cout << "r";
    break;
  case Piece::Bishop:
    std::cout << "b";
    break;
  default:
    break;
  }
  std::cout << "\n";
}

void uciloop() {
  std::string input;
  while (true) {
    std::getline(std::cin, input);
    if (input == "uci") {
      std::cout << "id name TakeItAll\n";
      std::cout << "id author MyName\n";
      std::cout << "uciok\n";
    } else if (input == "isready") {
      std::cout << "readyok\n";
    } else if (input.rfind("setoption", 0) == 0) {
      // Parse setoption command
      std::string optionName = "someOption"; // Extract from input
      std::string optionValue = "someValue"; // Extract from input
      handleSetOption(optionName, optionValue);
    } else if (input.rfind("position", 0) == 0) {
      // Parse position command
      std::string positionData = input.substr(9); // Extract position data
      handlePosition(positionData);
    } else if (input.rfind("go", 0) == 0) {
      // Parse go command
      std::string goData =
          input.size() > 2 ? input.substr(3) : ""; // Extract go parameters
      handleGo(goData);
    } else if (input == "quit") {
      break;
    } else {
      std::cerr << "Unknown command: " << input << std::endl;
    }
  }
}

int main() {
  uciloop();
  // auto moves = mg.generate_moves();
  // for (auto m : moves) {
  //   print_move_compact(m);
  //   p.make_move(m);
  //   std::cout << " " << evaluate(p) << "\n";
  //   p.unmake_move();
  // }
  return 0;
}
