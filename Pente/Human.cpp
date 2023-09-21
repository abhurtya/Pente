#include "Human.h"
#include <iostream>
#include <string> 

void Human::play(Board& board) {
    std::string input;
    int x, y;
    char column;
    int row;

    while (true) {
        std::cout << "Enter  coordinates where you'd wanna place a stone (format eg: E5): ";
        std::cin >> input;

        if (input.length() < 2) {
            std::cout << "Invalid input. Please enter coordinates in correct format (e.g: E5)." << std::endl;
            continue;
        }

        column = std::toupper(input[0]);  
        row = std::stoi(input.substr(1));

        if (column < 'A' || column > 'S' || row < 1 || row > 19) {
            std::cout << "Invalid input. Please enter coordinates in the correct format (eg: E5)." << std::endl;
            continue;
        }

        // Convert to internal representation
        y = column - 'A';
        x =  row-1;

        // Check if the move is valid
        if (!board.isValidMove(x, y)) {
            std::cout << "OOPS! Invalid move. That spot is already taken or out of bounds. Please choose another spot." << std::endl;
            continue;
        }

        break;  
    }

    // Now we can set the location
    setLocation(x, y);
    std::cout << "You chose position: " << column << row << std::endl;

}


void Human::setLocation(int x, int y) {
    location = std::make_pair(x, y);
}

std::pair<int, int> Human::getLocation() const {
    return location;
}
