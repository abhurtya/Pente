
/*********************************************
* Name: Anish Bhurtyal						 *
* Project:  Pente Game						 *
* Class:  CMPS 366							 *
* Date:  09/27/2023                          *
**********************************************/

#include <iostream>
#include "Tournament.h"
#include "Human.h"
#include "Computer.h"
#include "FileReader.h"

void displayIntro() {
    std::cout << "_____________________________________\n";
    std::cout << "|                                   |\n";
    std::cout << "|          Let's Play               |\n";
    std::cout << "|                                   |\n";
    std::cout << "|             PENTE                 |\n";
    std::cout << "|                                   |\n";
    std::cout << "|            MENU OPTION            |\n";
    std::cout << "|          1) Start new game        |\n";
    std::cout << "|          2) Load  game            |\n";
    std::cout << "|                                   |\n";
    std::cout << "|___________________________________|\n";
    std::cout << "\nGet ready...\n";
}

int main()
{
    displayIntro();
    
    int option;
    while (true) {

        std::cout << "Enter your option: ";
        std::cin >> option;

        if (option == 1 || option == 2) {
            break; // Exit if input valid
        }
        else {
            std::cout << "Invalid choice. \nEnter 1 for new game, 2 for load game" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        }

    }
 
    Player* human = new Human();
    Player* computer = new Computer();
    Tournament t(human, computer);

    if (option == 1) {
        t.startGame();
    }
    else if (option == 2) {
        
        Board loadedBoard;
        std::string nextPlayer;
        char nextPlayerSymbol;
        if (FileReader::loadGame(loadedBoard, human, computer, nextPlayer, nextPlayerSymbol)) {
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