#include "Player.h"
#include <iostream>

std::pair<int, int> Player:: strategy(const Board& board, char symbol) {
	
    //strategy for first move second move
    if (board.checkFirstMoveSecondMove(symbol) == 0 && symbol == 'W') {
        std::cout << "used strategy firstmove\n";
        return firstMoveStrategy();
    }

    
    else if (board.checkFirstMoveSecondMove(symbol) == 1 && symbol == 'W' && board.getCell(9,9) =='W') {
        std::cout << "used strategy second move\n";
        return secondMoveStrategy(board);
    }

    std::pair<int, int> bestMove;
    bestMove = checkWinStrategy(board, symbol);
    if (bestMove.first != -1) {
        std::cout << "used strategy checkWin\n";
        return bestMove;
    }

    bestMove = blockWinStrategy(board, symbol);
    if (bestMove.first != -1) {
        std::cout << "used strategy blockWin\n";
        return bestMove;
    }

    bestMove = checkCaptureStrategy(board, symbol);
    if (bestMove.first != -1) {
        std::cout << "used strategy checkCapture\n";
        return bestMove;
    }

    bestMove = snakeStrategy(board, symbol);
    if (bestMove.first != -1) {
        std::cout << "used strategy snake\n";
        return bestMove;
    }

    bestMove = blockSnakeStrategy(board, symbol);
    if (bestMove.first != -1) {
        std::cout << "used strategy blockSnake\n";
        return bestMove;
    }


    bestMove = mujiStrategy(board);
    if (bestMove.first != -1) {
        std::cout << "used Muji strategy\n";
        return bestMove;
    }
}


std::pair<int, int> Player::firstMoveStrategy() const {
    return { 9, 9 };  // J10 
}

std::pair<int, int> Player::secondMoveStrategy(const Board& board) const {
    std::pair<int, int> move;
    
    do {
        move = mujiStrategy(board);
    } while (abs(move.first - 9) < 3 && abs(move.second - 9) < 3);
    return move;
}

std::pair<int, int> Player::mujiStrategy(const Board& board) const {
    int x, y;
    do {
        x = std::rand() % 19;
        y = std::rand() % 19;
    } while (!board.isValidMove(x, y));
    return std::make_pair(x, y);
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
    
    std::array<std::pair<int, int>, 8> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {-1, -1}, {-1, 1}} };

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            if (board.getCell(i, j) == '*') {
                for (const auto& direction : directions) {
                    int dx = direction.first;
                    int dy = direction.second;

                    if (board.isCapturePossible(i, j, dx, dy, symbol)) {
                        return { i, j };
                    }
                }
            }
        }
    }
    return { -1, -1 }; // cant find capture

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


