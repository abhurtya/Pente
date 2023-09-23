#pragma once

#include <array>

class Board {

public:
	Board();
	void displayBoard() const;
	bool setCell(int x, int y, char symbol);
	char getCell(int x, int y) const;
	bool isCellEmpty(int x, int y) const;
	bool isValidMove(int x, int y) const;

	int checkFirstMoveSecondMove(char symbol) const;
	int countConsecutiveStones(int x, int y, int dx, int dy, char symbol) const;

private:

	// Create a 19x19 grid
	std::array<std::array<char, 19>, 19> grid = {' '};
};