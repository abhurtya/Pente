#include "Player.h"
#include <iostream>

Player::Player() : points(0), captures(0) {

}

/**********************************************************************
Function Name: strategy
Purpose: Determine the optimal move based on the board state and player symbol.
Parameters:
            board, a reference to the current Board
            symbol, a character ('W' or 'B').
Return Value: A pair of integers for the row and column of the chosen move.
Algorithm:
            1) If first move and symbol is 'W', return firstMoveStrategy
            2) For a second move with 'W' at center, return secondMoveStrategy
            3) Get best move using findBestStrategy. Return if valid
            4) If not, get move using defaultStrategy. Return if valid
            5) If all else fails, return a move using randomStrategy
Assistance Received: None
**************************************************************** */

std::pair<int, int> Player:: strategy(const Board& board, char symbol) const{
	
    //strategy for first move second move
    if (board.checkFirstMoveSecondMove(symbol) == 0 && symbol == 'W') {
        std::cout << "used strategy firstmove\n";
        return firstMoveStrategy();
    }else if (board.checkFirstMoveSecondMove(symbol) == 1 && symbol == 'W' && board.getCell(9,9) =='W') {
        std::cout << "used strategy second move\n";
        return secondMoveStrategy(board);
    }

    std::pair<int, int> bestMove = findBestStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        return bestMove;
    }
    
    bestMove = defaultStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        return bestMove;
    }

    return randomStrategy(board);
    
}

/**********************************************************************
Function Name: findBestStrategy
Purpose: Determine the optimal strategy among various strategies
Parameters:
            board, a reference to the current Board
            symbol, a character ('W' or 'B')
Return Value: A pair of integers for the row and column of the chosen move, or (-1, -1) if no 
Algorithm:
            1) Seek  5 in a row winning move.
            2) If none, block opponent's win
            3) If none, defend against captures
            4) If none, capture opponent's stones
            5) If none, form a chain
            6) If none, block opponent's chain
            7) If none, default to backup strategy
Assistance Received: None
*****************************************************************/
std::pair<int, int> Player::findBestStrategy(const Board& board, char symbol) const {
    std::pair<int, int> bestMove;

    bestMove = checkWinStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Winning Move" << std::endl;
        return bestMove;
    }

    bestMove = blockWinStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Block Opponent's Win" << std::endl;
        return bestMove;
    }

    bestMove = blockCaptureStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Defend Against Capture" << std::endl;
        return bestMove;
    }

    bestMove = checkCaptureStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Capture Opponent's Stones" << std::endl;
        return bestMove;
    }

    bestMove = snakeStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Form a Chain of Stones" << std::endl;
        return bestMove;
    }

    bestMove = blockSnakeStrategy(board, symbol);
    if (isMoveOk(bestMove)) {
        std::cout << "Strategy: Block Opponent's Chain" << std::endl;
        return bestMove;
    }

    std::cout << "Strategy: (Default strategy)" << std::endl;
    return bestMove; // Default to random strategy
}

/* *********************************************************************
Function Name: isMoveOk
Purpose: To determine if the proposed move is valid
Parameters:
            move, a pair of integers representing the row and column
Return Value: Returns true if the move is valid, and false otherwise
Algorithm:
            1) Check if the first value of the move pair is not -1 (indicative of an invalid move). If not, return true
            2) Otherwise, return false
Assistance Received: None
********************************************************************* */

bool Player::isMoveOk(const std::pair<int, int>& move) const {
    return move.first != -1;
}

/*****************************************************
Function Name: firstMoveStrategy
Purpose: Give the first move for the player
Return Value: A pair of integers representing the center position (J10)
Algorithm:
            1) Return the center position of the board i.e. (9, 9)
Assistance Received: None
***********************************************************/

std::pair<int, int> Player::firstMoveStrategy() const {
    return { 9, 9 };  // J10 
}

/**********************************************************************
Function Name: secondMoveStrategy
Purpose: Determine the second move for the player
Parameters:
    board, a constant reference to the Board class 
Return Value: A pair of integers representing move
Algorithm:
            1) Define the 4 possible directions for the second move
            2) Randomly pick a direction
            3) Validate the move using the chosen direction, if valid return the position
            4) If all directions are invalid (unlikely for a second move), return {-1, -1}
Assistance Received: None
**********************************************************************/

std::pair<int, int> Player::secondMoveStrategy(const Board& board) const {

    //just using 4 up, down , left and right for now
    std::array<std::pair<int, int>, 4> directions = { {{-4, 0}, {4, 0}, {0, -4}, {0, 4}} };
    
    //will randomly place stones in either dir.
    for (int i = 0; i < 4; i++) {
        int index = rand() % 4; 
        int x = 9 + directions[index].first;
        int y = 9 + directions[index].second;

        if (board.isValidMove(x, y)) {
            return { x, y };
        }
    }

    return { -1, -1 }; // should never reach here bc it's second move
}

/**********************************************************************
Function Name: randomStrategy
Purpose: To determine a move by randomly picking a position on the board
Parameters:
    board, a constant reference to the Board
Return Value: A pair of integers representing move
Algorithm:
            1) Continuously generate random x and y coordinates within the board size
            2) Check if the position is a valid move
            3) If valid, return the position. Otherwise, repeat
Assistance Received: None
**********************************************************************/


std::pair<int, int> Player::randomStrategy(const Board& board) const {
    int x, y;
    do {
        std::srand(std::time(0));
        x = std::rand() % 19;
        y = std::rand() % 19;
    } while (!board.isValidMove(x, y));
    return std::make_pair(x, y);
}

/**********************************************************************
Function Name: defaultStrategy
Purpose: Find an adjacent move without immediate capture risk
Parameters:
    board, a constant reference to the Current game board
    symbol, a char representing the current player's stone color ('W' or 'B')
Return Value: A pair of integers representing move or (-1, -1) if no suitable position is found
Algorithm:
            1) Iterate through each cell in the board and find own symbol
            2) Check adjecent spots to place stone
            3) Validate: Is it a legal move without immediate capture risk?
            4) If valid, return the position else return (-1, -1)
Assistance Received: None
*******************************************************************/

std::pair<int, int> Player::defaultStrategy(const Board& board,  char symbol) const {

    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            if (board.getCell(i, j) == symbol) {
                // Check adjacent cells for valid placement
                std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };
                for (const auto& direction : directions) {
                    int dx = direction.first;
                    int dy = direction.second;
                    int x = i + dx;
                    int y = j + dy;

                    // Ensure that placing stone here won't get in immediate capture
                    if (board.isValidMove(x, y) && !board.isCapturePossible(x, y, dx, dy, opponentSymbol) && !board.isCapturePossible(x, y, -dx, -dy, opponentSymbol)) {
                        return { x, y };
                    }
                }
            }
        }
    }
    return { -1,-1 }; 
}

/**********************************************************************
Function Name: checkWinStrategy
Purpose: Determine if there's an immediate winning due to 5 in a row
Parameters:
            board, a reference to the current Board object
            symbol, a character representing the player's symbol ('W' or 'B')
Return Value: Returns a pair of integers representing the row and column of the potential winning move or (-1, -1) if none
Algorithm:
            1) Iterate over all cells in the 19x19 board.
            2) If the current cell is empty, consider it for placing a stone
            3) For each of the directions (horizontal, vertical, both diagonals), calculate the total consecutive stones of the player's symbol
            4) If it creates  5 or more stones in a row, return that cell as the winning move.
            5) If none, return (-1, -1).
Assistance Received: None
**********************************************************************/

std::pair<int, int> Player::checkWinStrategy(const Board& board, char symbol) const {
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            // The current cell must be empty to place a stone
            if (board.getCell(i, j) == '*') {
                // Check all directions: x-axis, y-axis, x=y line, and x=-y line
                std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };
                for (const auto& direction : directions) {
                    int dx = direction.first;
                    int dy = direction.second;

                    // +1 considering the currently empty cell too
                    int totalConsecutiveStones = 1 + board.countConsecutiveStones(i, j, dx, dy, symbol) + board.countConsecutiveStones(i, j, -dx, -dy, symbol);
                    if (totalConsecutiveStones >= 5) {
                        return { i, j };
                    }
                }
            }
        }
    }
    // No winning move found
    return { -1, -1 };
}

/**********************************************************************
Function Name: blockWinStrategy
Purpose: To identify an immediate move to block the opponent's potential 5 in a row winning move 
Parameters:
            board, a reference to the current Board object
            symbol, a character representing the player's symbol ('W' or 'B')
Return Value: Returns a pair of integers representing the row and column of the move to block the opponent or (-1, -1) if none
Algorithm:
            1) Determine the opponent's symbol
            2) Call the checkWinStrategy function with the board and opponent's symbol to identify their potential winning move
            3) Return the result, effectively suggesting a move to block the opponent
Assistance Received: None
********************************************************************* */

std::pair<int, int> Player::blockWinStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return checkWinStrategy(board, opponentSymbol);
}


/**********************************************************************
Function Name: checkCaptureStrategy
Purpose: To find the best move on the board that allows the player to capture max number of opponent stones
Parameters:
            board, a reference to the current Board object.
            symbol, a character representing the player's symbol ('W' or 'B').
Return Value: Returns a pair of integers representing the row and column of the best capture move or (-1, -1) if none
Algorithm:
            1) Initialize the bestMove to (-1, -1) and maxCapturePossible to 0
            2) Loop through each cell on the board
                a) If the cell is empty, Loop through all 8 directions to check for potential captures
                b) if capture is possible, mark that as bestMove and continue the loop
                c) chekc If capture is possible in other call and if no of capture simultaneously is more than before, mark new location as best Move
            3) Return the bestMove.
Assistance Received: None
********************************************************************* */

std::pair<int, int> Player::checkCaptureStrategy(const Board& board, char symbol) const {
    std::pair<int, int> bestMove = { -1, -1 };
    std::array<std::pair<int, int>, 8> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {-1, -1}, {-1, 1}} };
    
    int maxCapturePossible = 0;
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            if (board.getCell(i, j) == '*') {

                //for all cells, check how many captrues possible
                int capturePossible = 0;
                for (const auto& direction : directions) {
                    int dx = direction.first;
                    int dy = direction.second;

                    if (board.isCapturePossible(i, j, dx, dy, symbol)) {
                        capturePossible++;
                    }
                }

                if (capturePossible > maxCapturePossible) {
                    maxCapturePossible = capturePossible;
                    bestMove = { i, j };
                }
            }
        }
    }
    return bestMove; // cant find capture then -1,-1

}

/**********************************************************************
Function Name: blockCaptureStrategy
Purpose: To determine the best move on the board that prevents the opponent from capturing the player's stones.
Parameters:
            board, a reference to the current Board object
            symbol, a character representing the player's symbol ('W' or 'B')
Return Value: Returns a pair of integers representing the row and column of the best blocking move or (-1, -1) if none
Algorithm:
            1) Determine the opponent's symbol based on the current player's symbol.
            2) Utilize the checkCaptureStrategy function with the opponent's symbol to identify the best blocking move
            3) Return the  move
Assistance Received: None
*********************************************************************/


std::pair<int, int> Player::blockCaptureStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return checkCaptureStrategy(board, opponentSymbol);

}

/**********************************************************************
Function Name: snakeStrategy
Purpose: To determine the best move on the board that extends the player's longest consecutive line of stones
Parameters:
            board, a reference to the current Board object
            symbol, a character representing the player's symbol ('W' or 'B')
Return Value: Returns a pair of integers representing the row and column of the best move to extend the line or (-1, -1) if none
Algorithm:
            1) Define possible directions for extending a line
            2) Prioritize making a line of length 4, then a line of length 3
            3) Check for target lenth 4, if not loop again for target length 3
                a) Traverse the entire board
                b) For each empty cell:
                    i) Check in all directions for existing consecutive stones
                    ii) If the count matches the target length:
                        - Ensure placing a stone doesn't result in immediate capture
                        - If safe, return the cell's position
            4) If none found that satisfies the criteria, return (-1, -1)
Assistance Received: None
********************************************************************* */

std::pair<int, int>  Player::snakeStrategy(const Board& board, char symbol) const {

    // code so that it will extend the longest line so far, 
    // for example if it has 3 consecutive stones already,
    //  then it will try to get it to 4. but if there are no 3 stones in 
    // the entire board,
    //then it will try to see if there are 2 consecutive stones there,
    //if it is it will try to make it 3 by extending with this strategy

    std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };

    // Prioritize making row of 4, if not make row of 3
    for (int targetLength = 4; targetLength >=3; --targetLength) {
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (board.getCell(i, j) == '*') {
                    for (const auto& direction : directions) {
                        int dx = direction.first;
                        int dy = direction.second;

                        int totalConsecutiveStones = 1 + board.countConsecutiveStones(i, j, dx, dy, symbol) + board.countConsecutiveStones(i, j, -dx, -dy, symbol);
                        if (totalConsecutiveStones == targetLength) {
                            // Ensure that placing stone here won't get in immediate capture
                            char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
                            if (!board.isCapturePossible(i, j, dx, dy, opponentSymbol) && !board.isCapturePossible(i, j, -dx, -dy, opponentSymbol)) {
                                return { i, j };
                            }
                        }
                    }
                }
            }
        }
    }
    return { -1, -1 };
}


/**********************************************************************
Function Name: blockSnakeStrategy
Purpose: To determine the best move on the board that blocks the opponent's longest consecutive line of stones
Parameters:
            board, a reference to the current Board object
            symbol, a character representing the player's symbol ('W' or 'B')
Return Value: Returns a pair of integers representing the row and column of the best move to block the opponent's line or (-1, -1) if none
Algorithm:
            1) Identify the opponent's symbol
            2) Call the snakeStrategy function with the opponent's symbol
            3) Return the result, which represents the best move to block the opponent's snake strategy
Assistance Received: None
********************************************************************* */

std::pair<int, int>  Player::blockSnakeStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return snakeStrategy(board, opponentSymbol);
}


