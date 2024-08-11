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

    PerftResults reference;
    reference.number_of_nodes = 4085603;
    reference.number_of_captures = 757163;
    reference.number_of_en_passent = 1929;
    reference.number_of_castles = 128013;
    reference.number_of_promotions = 15172;
    reference.number_of_checks = 25523;
    reference.number_of_checkmates = 43;

    // Perft Position 2
    Position p("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    Perft myp(p);

    PerftResults res = myp.run(4);

    std::cout << "Should be: \n";
    reference.print();

    std::cout << "Is actually: \n";
    res.print();

    std::cout << "\n";

    return 0;
}
