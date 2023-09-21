
#pragma once

#include "Board.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"

#include <iostream>
#include <random>

class Round {
public:
    Round(Player* human, Player* computer);
    std::pair<int,int> play();

private:
    Board board;
    Player* humanPlayer;
    Player* computerPlayer;
    int humanPoints;
    int computerPoints;
    int humanCaptures;
    int computerCaptures;

    char determineFirstPlayer();
    void takeTurn(Player* currentPlayer, char symbol);
    bool checkForEndOfRound();

    bool checkForWin(char symbol, Player* currentPlayer);
    int checkDirection(int x, int y, int dx, int dy, char symbol);

    bool checkForCapture(char symbol, Player* currentPlayer);
    bool checkForCaptureDirection(int x, int y, int dx, int dy, char symbol, std::pair<int, int>& capture1, std::pair<int, int>& capture2);
    void updateScore();
};

