#include "FileWriter.h"
#include <fstream>
#include <iostream>

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
