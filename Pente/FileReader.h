#pragma once
#include "Board.h"
#include "Player.h"

class FileReader {
public:
    bool loadGame(Board& board, Player* human, Player* computer, std::string& nextPlayer, char& nextPlayerSymbol);
};