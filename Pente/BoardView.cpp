#include "boardView.h"

BoardView::BoardView(Board& board) : board(board) {}

void BoardView::show() {
    board.displayBoard();
}