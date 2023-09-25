#include "Human.h"
#include <iostream>
#include <string> 

void Human::play(Board& board, char symbol) {
    std::string input;
    int x, y;
    char column;
    int row;

    while (true) {
        std::cout << "Enter stone move(eg: E5) or tye 'help': ";
        std::cin >> input;

        // Providing help to the user
        if (input == "help") {
            std::pair<int, int> suggestedMove = strategy(board, symbol);
            column = 'A' + suggestedMove.second;
            row = suggestedMove.first + 1;
            std::cout << "Suggested move: " << column << row << std::endl <<std::endl;
            continue;  // Continue to allow user to decide after viewing suggestion
        }

        if (input.length() < 2) {
            std::cout << "Invalid input. enter coordinates in correct format (e.g: E5)" << std::endl;
            continue;
        }

        column = std::toupper(input[0]);  
        try {
            row = std::stoi(input.substr(1));
        }
        catch (const std::exception&) {
            std::cout << "Invalid input. Enter coordinates in the correct format (e.g: E5)." << std::endl;
            continue;
        }


        if (column < 'A' || column > 'S' || row < 1 || row > 19) {
            std::cout << "Invalid input. Enter coordinates in the correct format (eg: E5)." << std::endl;
            continue;
        }

        // Convert to internal representation
        y = column - 'A';
        x =  row-1;

        // Check if the move is valid
        if (!board.isValidMove(x, y, symbol)) {
            std::cout << "OOPS! Invalid move choose another spot." << std::endl;
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
