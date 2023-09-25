#include "FileReader.h"
#include <fstream>
#include <iostream>

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
