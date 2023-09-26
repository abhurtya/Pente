#include "Player.h"
#include <iostream>

Player::Player() : points(0), captures(0) {

}


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

    std::cout << "Strategy: Random Move (No specific strategy applied)" << std::endl;
    return bestMove; // Default to random strategy
}

bool Player::isMoveOk(const std::pair<int, int>& move) const {
    return move.first != -1;
}


std::pair<int, int> Player::firstMoveStrategy() const {
    return { 9, 9 };  // J10 
}

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

std::pair<int, int> Player::randomStrategy(const Board& board) const {
    int x, y;
    do {
        x = std::rand() % 19;
        y = std::rand() % 19;
    } while (!board.isValidMove(x, y));
    return std::make_pair(x, y);
}


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
    return { -1, -1 }; // No winning move found
}

std::pair<int, int> Player::blockWinStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return checkWinStrategy(board, opponentSymbol);
}

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

std::pair<int, int> Player::blockCaptureStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return checkCaptureStrategy(board, opponentSymbol);

}

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



std::pair<int, int>  Player::blockSnakeStrategy(const Board& board, char symbol) const {
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    return snakeStrategy(board, opponentSymbol);
}


