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
	//void resumeGame();

private:
	Player* human;
	Player* computer;
	int totalHumanPoints;
	int totalComputerPoints;
	int roundNum;

};