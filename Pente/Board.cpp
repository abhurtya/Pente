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

void Board::displayBoard() {

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
