#pragma once

#include <array>

class Board {

public:
	Board();
	~Board();
	void displayBoard() const;
	bool setCell(int x, int y, char symbol);
	char getCell(int x, int y) const;
	bool isCellEmpty(int x, int y) const;
	bool isValidMove(int x, int y , char symbol = ' ') const;

	int checkFirstMoveSecondMove(char symbol) const;
	int countConsecutiveStones(int x, int y, int dx, int dy, char symbol) const;
	bool isCapturePossible(int x, int y, int dx, int dy, char symbol) const;
	bool captureStones(int x, int y, int dx, int dy, char symbol);


private:

	// Create a 19x19 grid
	std::array<std::array<char, 19>, 19> grid = {' '};
};