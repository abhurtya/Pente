#include "FileReader.h"
#include <fstream>

bool FileReader::loadGame(Board& board, Player* human, Player* computer, std::string& nextPlayer, char& nextPlayerSymbol) {
    std::ifstream inFile("game_save.txt");
    if (!inFile.is_open()) {
        return false;
    }

    std::string line;
    getline(inFile, line); // Read the "Board:" line

    for (int i = 18; i >= 0; --i) {
        getline(inFile, line);
        for (int j = 0; j < 19; ++j) {
            board.setCell(i, j, line[j]);
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
