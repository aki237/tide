#include "Clipboard.hpp"

int main(int argc, char *argv[]) {
    Clipboard board;
    std::cout << board.getText() << std::endl;
    board.putText("Test clipboard string");
    std::cout << board.getText() << std::endl;
    board.clear();
    return 0;
}

