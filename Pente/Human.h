#pragma once

#include "Player.h"

class Human : public Player {
public:
    Human(); 
    virtual ~Human(); 

    void play(Board& board, char symbol) override;
    std::pair<int, int> getLocation() const override;
    void setLocation(int x, int y);
    std::string getPlayerType() const override { return "Human"; }
private:
    std::pair<int, int> location;
};
