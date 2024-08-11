#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "perft.h"
// #include "utils.h"
#include "movegen.h"

// Mockup functions
void handleSetOption(const std::string &optionName, const std::string &optionValue)
{
    // Implement setting options
}

void handlePosition(const std::string &positionData)
{
    // Implement setting up the board position
}

void handleGo(const std::string &goData)
{
    // Implement move calculation logic
    //    std::cout << "info currmove e2e4 currmovenumber 1\n";
    std::cout << "info depth 1 seldepth 0\ninfo score cp 63  depth 1 nodes 13 time 15 pv b2b4\n";
}

void uciloop()
{
    std::string input;
    while (true)
    {
        std::getline(std::cin, input);
        if (input == "uci")
        {
            std::cout << "id name DummyChessEngine\n";
            std::cout << "id author MyName\n";
            std::cout << "uciok\n";
        }
        else if (input == "isready")
        {
            std::cout << "readyok\n";
        }
        else if (input.rfind("setoption", 0) == 0)
        {
            // Parse setoption command
            std::string optionName = "someOption"; // Extract from input
            std::string optionValue = "someValue"; // Extract from input
            handleSetOption(optionName, optionValue);
        }
        else if (input.rfind("position", 0) == 0)
        {
            // Parse position command
            std::string positionData = input.substr(9); // Extract position data
            handlePosition(positionData);
        }
        else if (input.rfind("go", 0) == 0)
        {
            // Parse go command
            std::string goData = input.substr(3); // Extract go parameters
            handleGo(goData);
        }
        else if (input == "quit")
        {
            break;
        }
        else
        {
            std::cerr << "Unknown command: " << input << std::endl;
        }
    }
}

int main()
{

    // Starting Position
    Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // Perft Position 2
    Position p2("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    // Perft Position 3
    Position p3("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

    // Perft Position 4
    Position p4("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    Position test("4k2r/8/8/8/8/8/8/4K3 b KQkq - 0 1");

    Perft myp(p);

    auto res = myp.run(1);

    std::cout << "Nodes: " << res.number_of_nodes;

//    perft_up_to(3, p2);

    /*
        Position test("4k2r/8/8/8/8/8/8/4K3 b KQkq - 0 1");

        print_full_board(test);

        //Move m(60, 62, Move_Type::Short_Castle);
        Move m(63, 61);

        test.make_move(m);

        print_full_board(test);

        test.unmake_move();
        print_full_board(test);
    */

    std::cout << "\n";

    return 0;
}
