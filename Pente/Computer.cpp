#include "computer.h"

#include <iostream>

/**********************************************************************
Function Name: Computer (constructor)
Purpose: Initialize the computer player object with a default location (-1, -1)
Parameters: None
Return Value: None
Assistance Received: None
*******************************************************************/
Computer::Computer() : location(-1, -1) {
}

Computer::~Computer() {
}

/**********************************************************************
Function Name: play
Purpose: Make a move for the computer player on the board based on the chosen strategy
Parameters:
            board, reference to the current board.
            symbol, a character representing the computer player's symbol (W or B).
Return Value: None
Algorithm:
            1) Use the strategy function to get the best move
            2) Convert the numerical coordinates to a human-readable representation
            3) Set the chosen position as the computer's location
            
Assistance Received: None
*********************************************************/
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