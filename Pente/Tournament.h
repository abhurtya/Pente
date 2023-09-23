#pragma once

#include "Round.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"
#include "Board.h"

class Tournament {
public:
	Tournament(Player* human, Player* computer);
	void startGame();
	void resumeGame(Board& loadedBoard, Player* human, Player* computer, std::string nextPlayerName, char nextPlayerSymbol);

private:
	Player* human;
	Player* computer;
	int totalHumanPoints;
	int totalComputerPoints;
	int roundNum;

	void announceTournamentWinner();
	bool askUserPlay();
	void playRound(Player* human, Player* computer, Board* loadedBoard = nullptr, std::string nextPlayerName = "", char nextPlayerSymbol = ' ');

};