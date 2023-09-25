#pragma once
#include "Board.h"
#include "Player.h"

class FileReader {
public:
    static bool loadGame(Board& board, Player* human, Player* computer, std::string& nextPlayer, char& nextPlayerSymbol);
};