#include <iostream>
#include <string>

void uciloop() {
    std::string input;
    while(true) {
        std::getline(std::cin, input);
        if (input == "uci") {
            std::cout << "id name DummyChessEngine\n";
            std::cout << "id author MyName\n";
            std::cout << "uciok\n";
        } else if (input == "isready") {
            std::cout << "readyok\n";
        } else if (input == "quit") {
            break;
        }
    }
}

int main() {
    uciloop();
    return 0;
}
