#include "Human.h"
#include <iostream>
#include <string> 

/**********************************************************************
Function Name: Human (constructor)
Purpose: Initialize the human player object with a default location (-1, -1)
Parameters: None
Return Value: None
Assistance Received: None
*******************************************************************/
Human::Human() : location(-1, -1) {
}


Human::~Human() {
}

/**********************************************************************
Function Name: play
Purpose: Allow the human player to make a move on the board, with an option to get move suggestions
Parameters:
            board, a reference to the current board object
            symbol, a character representing the human player's symbol ('W' or 'B').
Return Value: None.
Algorithm:
            1) Continuously prompt the user for a move unles move valid
            2) If "help" is entered, suggest a move based on strategy
            3) Validate the entered move
            4) If move is valid, update the board  Otherwise, notify the user and repeat.
Assistance Received: None
**************************************************************/
void Human::play(Board& board, char symbol) {
    std::string input;
    int x, y;
    char column;
    int row;

    while (true) {
        std::cout << "Enter your move or type 'help': ";
        std::cin >> input;
        std::cout << std::endl;

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
