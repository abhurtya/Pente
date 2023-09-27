#include "FileWriter.h"
#include <fstream>
#include <iostream>

/**********************************************************************
Function Name: saveGame
Purpose: Save the current game state to a file, including board configuration and player details.
Parameters:
            board, a pointer to the current Board object
            human, a pointer to the human player object
            computer, a pointer to the computer player object.
            nextPlayer, a  string indicating the next player's turn ("Human" or "Computer")
            nextPlayerSymbol, a character representing the next player's symbol ('W' or 'B')
Return Value: Returns true if the game data was successfully written, false otherwise
Algorithm:
            1) Get filename from user
            2) Open file to write
            3) Write board to file
            4) Write  human, computer player stats in file
            5) Write next player and symbol
            6) Close the file
Assistance Received: None
******************************************************************************/


bool FileWriter::saveGame(const Board* board, const Player* human, const Player* computer, const std::string nextPlayer, const std::string nextPlayerSymbol) {

    
    std::string filename;
    std::cout << "Please enter FileName: \n";
    std::cin >> filename;

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Failed to open file for writing.\n";
        return false;
    }

    outFile << "Board:\n";
    for (int i = 18; i >=0; --i) {
        for (int j = 0; j < 19; ++j) {
            if (board->getCell(i, j) == '*') {
                outFile << 'O';
            }
            else {
                outFile << board->getCell(i, j);
            }
            
        }
        outFile << "\n";
    }

    outFile << "\nHuman:\n";
    outFile << "Captured pairs: " << human->getCaptures() << "\n";
    outFile << "Score: " << human->getPoints() << "\n";

    outFile << "\nComputer:\n";
    outFile << "Captured pairs: " << computer->getCaptures() << "\n";
    outFile << "Score: " << computer->getPoints() << "\n";

    outFile << "\nNext Player: " << nextPlayer << " - " << nextPlayerSymbol ;

    outFile.close();
    return true;
}
