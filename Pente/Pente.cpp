// Pente.cpp :  'main' pProgram execution, entry and exit  here

#include <iostream>
#include "Tournament.h"
#include "Human.h"
#include "Computer.h"
//#include "Board.h"
//#include "BoardView.h"

void displayIntro() {
    std::cout << "=====================================\n";
    std::cout << "||                                 ||\n";
    std::cout << "||          Let's Play             ||\n";
    std::cout << "||                                 ||\n";
    std::cout << "||             PENTE               ||\n";
    std::cout << "||                                 ||\n";
    std::cout << "=====================================\n";
}


int main()
{

    //std::cout << "Welcome to the game!\n";
    //  // Implement sleep to Wait for 1 second
    //std::cout << "Get ready...\n";
    //// Implement sleep to Wait for 1 second
    //displayIntro();
    //std::cout << "Get ready...\n";


    Player* human = new Human();
    Player* computer = new Computer();
    Tournament t(human, computer);
    t.startGame();

    delete human;
    delete computer;


     return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


