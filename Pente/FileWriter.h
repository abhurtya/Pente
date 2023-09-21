#pragma once

#include "Board.h"
#include "Player.h"


class FileWriter {
public:
	static bool saveGame(const Board* board, const Player* human, const Player* computer, const std::string nextPlayer, const std::string  nextPlayerSymbol);
};