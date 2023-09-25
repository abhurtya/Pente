
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
    Round(Player* human, Player* computer, const Board& loadedBoard);
    std::pair<int, int> play(char firstPlayerSymbol = ' ');
    std::pair<int, int> resume(Player* currentPlayer, char currentSymbol);


private:
    Board m_board;
    Player* m_humanPlayer;
    Player* m_computerPlayer;
    bool m_endRound = false;
    
    char tossHumanComputer();
    void takeTurn(Player* currentPlayer, char symbol);
    bool checkForEndOfRound();

    bool checkForWin(char symbol, Player* currentPlayer);

    bool checkForCapture(char symbol, Player* currentPlayer);
    void updateScore();
    void playGame(Player*& currentPlayer, char& currentSymbol);
};
