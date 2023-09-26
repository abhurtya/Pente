#include "computer.h"

#include <iostream>

Computer::Computer() : location(-1, -1) {
}

Computer::~Computer() {
}

void Computer::play(Board& board, char symbol) {

    std::pair<int, int> bestMove = strategy(board, symbol);
    
    char column = 'A' + bestMove.second;  
    int row = bestMove.first+1;  

    std::cout << "Computer chose position: " << column << row << std::endl;
 
    //validation done by the strategy
    setLocation(bestMove.first, bestMove.second);

}

void Computer::setLocation(int x, int y) {
    location = std::make_pair(x, y);
}

std::pair<int, int> Computer::getLocation() const {
    return location;
}