#pragma once

#include "board.h"

class Player {
public:
    virtual ~Player() {}

    // Pure virtual function
    virtual void play(Board& board) = 0;  
    virtual std::pair<int, int> getLocation() const = 0; 
};