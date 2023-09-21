#pragma once

#include "Player.h"

class Computer : public Player {
public:
    void play(Board& board) override;
    std::pair<int, int> getLocation() const override;
    void setLocation(int x, int y);
    std::string getPlayerType() const override { return "Computer"; }

private:
    std::pair<int, int> location;
};