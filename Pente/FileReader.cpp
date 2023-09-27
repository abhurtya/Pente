#include "FileReader.h"
#include <fstream>
#include <iostream>


/**********************************************************************
Function Name: loadGame
Purpose: Load a saved game from a file, populating the board and player stats
Parameters:
            board, a reference to the Board object to be loaded
            human, a pointer to the human player object 
            computer, a pointer to the computer player object
            nextPlayer, a reference to a string to indicate the next player's turn ("Human" or "Computer")
            nextPlayerSymbol, a reference to a char to indicate the next player's symbol ('W' or 'B')
Return Value: Returns true if the game data was successfully loaded, false otherwise
Algorithm:
            1) Get filename from user
            2) Open file to read
            3) Populate board
            4) Set human, computer player stats
            5) Identify next player and symbol
            6) Close the file
Assistance Received: None
**********************************************************/

bool FileReader::loadGame(Board& board, Player* human, Player* computer, std::string& nextPlayer, char& nextPlayerSymbol) {

    std::string filename;
    std::cout << "Please enter FileName: \n";
    std::cin >> filename;
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "Failed to open file for reading.\n";
        return false;
    }

    std::string line;
    getline(inFile, line); // Read the "Board:" line

    for (int i = 18; i >= 0; --i) {
        getline(inFile, line);
        for (int j = 0; j < 19; ++j) {
            if (line[j] == 'O') {
                board.setCell(i, j, '*');
            }
            else {
                board.setCell(i, j, line[j]);
            }
            
        }
    }

    getline(inFile, line);
    getline(inFile, line); // Read the "Human:" line

    int humanCaptures, humanScore = 0;
    inFile >> line >> line >> humanCaptures;
    inFile >> line >> humanScore;
    human->setCaptures(humanCaptures);
    human->setPoints(humanScore);

    getline(inFile, line);
    getline(inFile, line);
    getline(inFile, line); // Read the "Computer:" line

    int computerCaptures, computerScore = 0;
    inFile >> line >> line >> computerCaptures;
    inFile >> line >> computerScore;
    computer->setCaptures(computerCaptures);
    computer->setPoints(computerScore);

    getline(inFile, line);
    getline(inFile, line);
    std::string nextPlayerStone;
    inFile >> line >>line >> nextPlayer >> line >> nextPlayerStone;
    nextPlayerSymbol = (nextPlayerStone == "Black") ? 'B' : 'W';

    inFile.close();
    return true;
}
