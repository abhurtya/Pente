#pragma once

#include "Board.h"
#include <string>

class Player {
public:
    virtual ~Player() {}

    // Pure virtual function
    virtual void play(Board& board, char symbol) = 0;
    virtual std::pair<int, int> getLocation() const = 0;
    virtual std::string getPlayerType() const = 0;

    int getPoints() const { return points; }
    int getCaptures() const { return captures; }

    void addPoints(int p) { points += p; }
    void addCaptures() { captures++; }
    void setPoints(int p) { points = p; }
    void setCaptures(int c) { captures = c; }

private:
    int points = 0;
    int captures = 0;

protected:
    virtual std::pair<int, int> strategy(const Board& board, char symbol);
    std::pair<int, int> firstMoveStrategy() const;
    std::pair<int, int> secondMoveStrategy() const;
    std::pair<int, int> checkWinStrategy(const Board& board, char symbol) const;
    std::pair<int, int> blockWinStrategy(const Board& board, char symbol) const;
    std::pair<int, int> checkCaptureStrategy(const Board& board, char symbol) const;
    std::pair<int, int> blockCaptureStrategy(const Board& board, char symbol) const;
    std::pair<int, int> snakeStrategy(const Board& board, char symbol) const;
    std::pair<int, int> blockSnakeStrategy(const Board& board, char symbol) const;

    


};