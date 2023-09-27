#include "Board.h"
#include <iostream>

/**********************************************************************
Function Name: Board constructor
Purpose: To initialize the game board with all cells as '*'
Parameters: None
Return Value: None
Assistance Received: None
********************************************************************* */
Board::Board() {
	for (auto& row : grid) {
		for (char& cell : row) {
			cell = '*';
		}
	}
}

// Destructor for the Board class
Board::~Board() {

}

/**********************************************************************
Function Name: isCellEmpty
Purpose: To check if a particular cell is empty or not
Parameters:
			x, an integer. The row number of the cell
			y, an integer. The column number of the cell
Return Value: A boolean value indicating whether the cell is empty or not
Assistance Received: None
********************************************************************* */

bool Board::isCellEmpty(int x, int y) const {
	return getCell(x, y) == '*';
}

/**********************************************************************
Function Name: isValidMove
Purpose: To validate the move based on position and player's symbol.
Parameters:
			x, an integer. The row number of the desired move
			y, an integer. The column number of the desired move.
			symbol, a character. The symbol of the player ('W' or 'B').
Return Value: A boolean value indicating whether the move is valid or not.
Algorithm:
			1) Check if the move is within the board boundaries.
			2) Ensure the cell is empty
			3) Check the specific moves for the 'W' player, the starting player rules
Assistance Received: None
********************************************************************* */
bool Board::isValidMove(int x, int y, char symbol) const {
	
	if (x < 0 || x >= 19 || y < 0 || y >= 19 ) {
		std::cout << "Invalid position " << std::endl;
		return false;
	}

	if (!isCellEmpty(x, y)) {
		std::cout << "Cell already occupied" << std::endl;
		return false;
	}

	if (symbol == 'W' && checkFirstMoveSecondMove('W') == 0) {
		if (!(x == 9 && y == 9)) {
			std::cout << "First white move must be at J10" << std::endl;
			return false;
		}
	}

	if (symbol == 'W' && checkFirstMoveSecondMove('W') == 1) {
		if (!(abs(x - 9) > 3 || abs(y - 9) > 3)) {
			std::cout << "Second white move must be 3 step away from J10" << std::endl;
			return false;
		}
	}

	return true;
	
}

/**********************************************************************
Function Name: setCell
Purpose: To set a symbol on the board at the given position.
Parameters:
			x, an integer. The row number of the cell.
			y, an integer. The column number of the cell
			symbol, a character. The symbol to place on the board.
Return Value: A boolean value indicating if the placement was successful
Assistance Received: None
*************************************************************************/
bool Board::setCell(int x, int y, char symbol) {

	if (x < 0 || x >= 19 || y < 0 || y >= 19) {
		std::cout << "Error: Invalid position for setCell." << std::endl;
		return false;
	}
	
	grid[x][y] = symbol;
	return true;
	
}

/*******************************************************************
Function Name: getCell
Purpose: Retrieve the content of a specific cell in the grid
Parameters:
			x, an integer for the row index.
			y, an integer for the column index.
Return Value: The content of the cell, a char value. Returns ' ' if out of range.
Assistance Received: None
**********************************************************************/
char Board::getCell(int x, int y) const {
	if (x >= 0 && x < 19 && y >= 0 && y < 19) {
		return grid[x][y];
	}
	return ' '; 
}

/**********************************************************************
Function Name: checkFirstMoveSecondMove
Purpose: To determine if it's the first, second, or subsequent move based on the symbol
Parameters:
			symbol, a character representing the player's symbol (W or B).
Return Value: 0 if no moves made, 1 if first move made, and -1 if moves >=2.
Algorithm:
			1) Iterate over each cell in the grid
			2) Count occurrences of the given symbol.
			3) Return the  value appropriately based on count
Assistance Received: None
*****************************************************************/

int Board::checkFirstMoveSecondMove(char symbol) const {
	int count = 0;
	for (const auto& row : grid) {
		for (char cell : row) {
			if (cell == symbol) {
				count++;
				// More than one move made, return early
				if (count > 1) return -1; 
			}
		}
	}
	return count; 
}

/**********************************************************************
Function Name: countConsecutiveStones
Purpose: Count consecutive stones in a given direction from a starting point
Parameters:
			x, y, starting cell coordinates
			dx, dy, direction to count in
			symbol, the stone's symbol to count
Return Value: No. of consecutive stones in the specified direction for 5 steps, an integer
Algorithm:
			1) From the starting cell, move in the specified direction.
			2) Count cells containing the given symbol.
			3) Stop counting if an empty or different symbol is found.
Assistance Received: None
****************************************************************/
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

/**********************************************************************
Function Name: captureStones
Purpose: Capture opponent's stones if they are flanked by the player's stones
Parameters:
			x, y, starting cell coordinates.
			dx, dy, direction to check.
			symbol, the player's symbol
Return Value: Whether any stones were captured, a boolean value
Algorithm:
			1) Check if capturing is possible using the isCapturePossible function.
			2) If capturing is possible, change the captured cells' symbols.
			3) Display a message indicating the captured cells.
Assistance Received: None
*************************************************************************************/

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

/**********************************************************************
Function Name: isCapturePossible
Purpose: Check if capturing opponent's stones is possible from a given starting point and direction
Parameters:
			x, y, starting cell coordinates.
			dx, dy, direction to check.
			symbol, the player's symbol
Return Value: Whether capturing is possible, a boolean value.
Algorithm:
			1) Check three cells in the specified direction.
			2) Return false if any of the cells are out of bounds.
			3) Return true if the first two cells contain the opponent's symbol and the third contains the player's symbol.
Assistance Received: None
********************************************************/

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

/***********************************************************
Function Name: displayBoard
Purpose: Display the current state of the game board to the console
Parameters: None
Return Value: None
Algorithm:
			1) Print the board's column labels
			2)  For each row in the board
				- Print the row number
				- Print the cell contents
Assistance Received: None
************************************************************/

void Board::displayBoard() const {

	std::cout << "________________________________________________" << std::endl;
	std::cout << "|                                              |" << std::endl;

	//first line of output will be ___ABC....
	std::cout << "|     "; 
	for (char c = 'A'; c <= 'S'; ++c) {
		std::cout << c <<" ";
	}
	std::cout << "   |" << std::endl;


	for (int row = 19; row >= 1; --row) {
		std::cout << "|  ";
		if (row < 10) std::cout << ' ';
		std::cout << row<<' ';
		


		for (const char& col : grid[row-1]) {
			std::cout  <<col <<" ";

		}
		std::cout << "   |" << std::endl;

	}
	
	std::cout << "|______________________________________________|" << std::endl;
}
