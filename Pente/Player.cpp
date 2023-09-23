#include "Player.h"

std::pair<int, int> Player:: strategy(const Board& board, char symbol) {
	/*std::pair<int, int> bestMove = std::make_pair(-1, -1);

	return bestMove;*/


    int x, y;
    do {
        x = std::rand() % 19;
        y = std::rand() % 19;
    } while (!board.isValidMove(x, y));
    return std::make_pair(x, y);
}
