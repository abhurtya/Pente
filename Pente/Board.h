#pragma once

#include <array>

class Board {

public:
	Board();
	void displayBoard();
	bool setCell(int x, int y, char symbol);
	char getCell(int x, int y) const;
	bool isCellEmpty(int x, int y) const;
	bool isValidMove(int x, int y) const;

private:

	// Create a 19x19 grid
	std::array<std::array<char, 19>, 19> grid = {};
};