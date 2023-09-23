#include "Tournament.h"
#include <iostream>

Tournament::Tournament(Player* human, Player* computer)
    : human(human), computer(computer), totalHumanPoints(0), roundNum(0), totalComputerPoints(0) {}



void Tournament::startGame() {
    do {
        playRound(human, computer);
    } while (askUserPlay());
    announceTournamentWinner();
}

void Tournament::resumeGame(Board& loadedBoard, Player* human, Player* computer, std::string nextPlayerName, char nextPlayerSymbol) {
    do {
        playRound(human, computer, &loadedBoard, nextPlayerName, nextPlayerSymbol);
    } while (askUserPlay());
    announceTournamentWinner();
}
bool Tournament::askUserPlay() {
    char continuePlaying;
    std::cout << "Do you wanna play another round? (y/n): ";
    std::cin >> continuePlaying;
    return (continuePlaying == 'y' || continuePlaying == 'Y');
}

void Tournament::announceTournamentWinner() {
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


//playRound() handles the logic for both startingand resuming games
void Tournament::playRound(Player* human, Player* computer, Board* loadedBoard, std::string nextPlayerName, char nextPlayerSymbol) {
   
    std::pair<int, int> roundPoints;

    if (loadedBoard) {

        Round round(human, computer,*loadedBoard);
        Player* currentPlayer = (nextPlayerName == "Human") ? human : computer;
        roundPoints = round.resume(currentPlayer, nextPlayerSymbol);
    }
    else {
        Round round(human, computer);
        roundPoints = round.play();
    }

    totalHumanPoints += roundPoints.first;
    totalComputerPoints += roundPoints.second;

    std::cout << "Points this round: Human " << roundPoints.first << " - " << roundPoints.second << " Computer\n";
    std::cout << "Total Score: Human " << totalHumanPoints << " - " << totalComputerPoints << " Computer\n";
}