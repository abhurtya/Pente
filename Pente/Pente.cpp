// Pente.cpp :  'main' pProgram execution, entry and exit  here

#include <iostream>
#include "Tournament.h"
#include "Human.h"
#include "Computer.h"
#include "FileReader.h"

void displayIntro() {
    std::cout << "=====================================\n";
    std::cout << "||                                 ||\n";
    std::cout << "||          Let's Play             ||\n";
    std::cout << "||                                 ||\n";
    std::cout << "||             PENTE               ||\n";
    std::cout << "||                                 ||\n";
    std::cout << "||            MENU OPTION          ||\n";
    std::cout << "||          1) Start new game      ||\n";
    std::cout << "||          2) Load  game          ||\n";
    std::cout << "||                                 ||\n";
    std::cout << "=====================================\n";
}


int main()
{

    //std::cout << "Welcome to the game!\n";
    //  // Implement sleep to Wait for 1 second
    //std::cout << "Get ready...\n";
    //// Implement sleep to Wait for 1 second
    displayIntro();
    std::cout << "Get ready...\n";

    int option;
    std::cout << "Enter your option: ";
    std::cin >> option;

    Player* human = new Human();
    Player* computer = new Computer();
    Tournament t(human, computer);
    /*t.startGame();*/

    if (option == 1) {
        t.startGame();
    }
    else if (option == 2) {
        FileReader reader;
        Board loadedBoard;
        std::string nextPlayer;
        char nextPlayerSymbol;
        if (reader.loadGame(loadedBoard, human, computer, nextPlayer, nextPlayerSymbol)) {
            std::cout << "Game loaded successfully!\n\n";
            t.resumeGame(loadedBoard, human, computer, nextPlayer, nextPlayerSymbol);
        }
        else {
            std::cout << "Failed to load the game!\n";
        }
    }


    delete human;
    delete computer;


    return 0;
}