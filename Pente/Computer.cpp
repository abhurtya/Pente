#include "computer.h"

#include <iostream>
#include <cstdlib> // For random move
#include <ctime> // For seeding random number generator

void Computer::play(Board& board) {
    std::srand(std::time(nullptr));
    int x, y;

    do {
         x = std::rand() % 19;
         y = std::rand() % 19;
    } while (!board.isValidMove(x, y));

    char column = 'A' + y;  
    int row = x+1;  

    std::cout << "Computer chose position: " << column << row << std::endl;
 
    setLocation(x, y);

}

void Computer::setLocation(int x, int y) {
    location = std::make_pair(x, y);
}

std::pair<int, int> Computer::getLocation() const {
    return location;
}