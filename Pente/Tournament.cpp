#include "Tournament.h"
#include <iostream>

Tournament::Tournament(Player* human, Player* computer)
    : human(human), computer(computer), totalHumanPoints(0), roundNum(0), totalComputerPoints(0) {}


void Tournament::startGame() {

    char continuePlaying='y';
    do{
        //Board board;
        Round round( human, computer);

        // Play the round and store the winner
        std::pair<int, int> roundPoints = round.play();

        // Update total points
        totalHumanPoints += roundPoints.first;
        totalComputerPoints += roundPoints.second;

        std::cout << "Points this round: Human " << roundPoints.first << " - " << roundPoints.second << " Computer\n";

        std::cout << "Total Score: Human " << totalHumanPoints << " - " << totalComputerPoints << " Computer\n";

        std::cout << "Do you wanna play another round? (y/n): ";
        std::cin >> continuePlaying;

    } while (continuePlaying == 'y' || continuePlaying == 'Y');

    // Announce the tournament winner
    if (totalHumanPoints > totalComputerPoints) {
        std::cout << "Human wins the tournament!\n";
    }
    else if (totalComputerPoints > totalHumanPoints) {
        std::cout << "Computer wins the tournament!\n";
    }
    else {
        std::cout << "The tournament is a draw!\n";
    }

}
