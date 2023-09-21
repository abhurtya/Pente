#pragma once

#include "board.h"

class BoardView {
public:
    BoardView(Board& board);
    void show();
private:
    Board& board;
};
