#include "Player.h"

std::pair<int, int> Player:: strategy(const Board& board, char symbol) {
	
    //strategy for first move second move
    if (board.checkFirstMoveSecondMove(symbol) == 0 && symbol == 'W') {
        return firstMoveStrategy();
    }

    
    else if (board.checkFirstMoveSecondMove(symbol) == 1 && symbol == 'W' && board.getCell(9,9) =='W') {
        return secondMoveStrategy();
    }

    int x, y;
    do {
        x = std::rand() % 19;
        y = std::rand() % 19;
    } while (!board.isValidMove(x, y));
    return std::make_pair(x, y);
}


std::pair<int, int> Player::firstMoveStrategy() const {
    return { 9, 9 };  // J10 
}

std::pair<int, int> Player::secondMoveStrategy() const {

    return { 9, 12 };
}
