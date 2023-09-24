#include "Board.h"
#include <iostream>

Board::Board() {
	for (auto& row : grid) {
		for (char& cell : row) {
			cell = '*';
		}
	}
}

bool Board::isCellEmpty(int x, int y) const {
	return getCell(x, y) == '*';
}

bool Board::isValidMove(int x, int y) const {
	return x >= 0 && x < 19 && y >= 0 && y < 19 && isCellEmpty(x, y);
}

bool Board::setCell(int x, int y, char symbol) {
	
		grid[x][y] = symbol;

		return true;
	
}

char Board::getCell(int x, int y) const {
	if (x >= 0 && x < 19 && y >= 0 && y < 19) {
		return grid[x][y];
	}
	return ' '; // Return a blank if the index is out of range
}


// 0 if no moves made, 1 if first move made, -1 if moves >=2
int Board::checkFirstMoveSecondMove(char symbol) const {
	int count = 0;
	for (const auto& row : grid) {
		for (char cell : row) {
			if (cell == symbol) {
				count++;
				if (count > 1) return -1; // More than one move made, return early
			}
		}
	}
	return count; // Will return 0 if no moves, 1 if one move
}

int Board:: countConsecutiveStones(int x, int y, int dx, int dy, char symbol) const {
	int count = 0;
	for (int i = 1; i < 5; ++i) {
		int newX = x + i * dx;
		int newY = y + i * dy;

		if (newX < 0 || newX >= 19 || newY < 0 || newY >= 19 || getCell(newX, newY) != symbol) {
			break;
		}

		++count;
	}
	return count;
}

bool Board:: captureStones(int x, int y, int dx, int dy, char symbol)  {
	if (isCapturePossible(x, y, dx, dy, symbol)) {
		// Capture the pair
		setCell(x + dx, y + dy, '*');
		setCell(x + 2 * dx, y + 2 * dy, '*');

		//converting into human representation
		std::cout << "\n*******  Pairs Captured   ******  " << char('A' + (y + dy)) << (x + dx + 1) << " and " << char('A' + (y + 2 * dy)) << (x + 2 * dx + 1) << "!\n";

		return true;
	}
	return false;
}

bool Board:: isCapturePossible(int x, int y, int dx, int dy, char symbol) const {
	// early return if the index is out of bounds
	for (int i = 1; i <= 3; ++i) {
		int newX = x + i * dx;
		int newY = y + i * dy;
		if (newX < 0 || newX >= 19 || newY < 0 || newY >= 19) {
			return false;
		}
	}

	char first = getCell(x + dx, y + dy);
	char second = getCell(x + 2 * dx, y + 2 * dy);
	char third = getCell(x + 3 * dx, y + 3 * dy);

	char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
	return (first == opponentSymbol && second == opponentSymbol && third == symbol);
}

void Board::displayBoard() const {

	std::cout << "=============================" << std::endl;

	//first line of output will be ___ABC....
	std::cout << "||    "; 
	for (char c = 'A'; c <= 'S'; ++c) {
		std::cout << c;
	}
	std::cout << "  ||" << std::endl;


	for (int row = 19; row >= 1; --row) {
		std::cout << "|| ";
		if (row < 10) std::cout << ' ';
		std::cout << row<<' ';
		


		for (const char& col : grid[row-1]) {
			std::cout  <<col;

		}
		std::cout << "  ||" << std::endl;

	}
	std::cout << "=============================" << std::endl;
}
