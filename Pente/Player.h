#pragma once

#include "Board.h"
#include <string>

class Player {
public:
    virtual ~Player() {}

    // Pure virtual function
    virtual void play(Board& board) = 0;  
    virtual std::pair<int, int> getLocation() const = 0; 
    virtual std::string getPlayerType() const = 0;

    int getPoints() const { return points; }
    int getCaptures() const { return captures; }

    void addPoints(int p) { points += p; }
    void addCaptures() { captures++; }

private:
    int points = 0;
    int captures = 0;
};