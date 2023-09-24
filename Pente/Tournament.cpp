#include "Tournament.h"
#include <iostream>

Tournament::Tournament(Player* human, Player* computer)
    : m_human(human), m_computer(computer), m_totalHumanPoints(0), m_roundNum(0), m_totalComputerPoints(0) {}

void Tournament::startGame() {
   
    do {
        playRound(m_human, m_computer);
    } while (askUserPlay());
    announceTournamentWinner();
}

void Tournament::resumeGame(Board& loadedBoard, Player* human, Player* computer, std::string nextPlayerName, char nextPlayerSymbol) {
    m_totalHumanPoints = human->getPoints();
    m_totalComputerPoints = computer->getPoints();

    playRound(human, computer, &loadedBoard, nextPlayerName, nextPlayerSymbol);
    while (askUserPlay()) {
        playRound(m_human, m_computer);
    }
    announceTournamentWinner();
}

bool Tournament::askUserPlay() {
    char continuePlaying;
    std::cout << "Do you wanna play another round? (y/n): ";
    std::cin >> continuePlaying;
    return (continuePlaying == 'y' || continuePlaying == 'Y');
}

void Tournament::announceTournamentWinner() {
    if (m_totalHumanPoints > m_totalComputerPoints) {
        std::cout << "Human wins the tournament!\n";
    }
    else if (m_totalComputerPoints > m_totalHumanPoints) {
        std::cout << "Computer wins the tournament!\n";
    }
    else {
        std::cout << "The tournament is a draw!\n";
    }
}

void Tournament::playRound(Player* human, Player* computer, Board* loadedBoard, std::string nextPlayerName, char nextPlayerSymbol) {

    human->setPoints(0);
    computer->setPoints(0);
 
    std::pair<int, int> roundPoints;

    if (loadedBoard) {
        //we're resuming a saved game, we continue from where we left
        Round round(human, computer, *loadedBoard);
        Player* currentPlayer = (nextPlayerName == "Human") ? human : computer;
        roundPoints = round.resume(currentPlayer, nextPlayerSymbol);
    }
    else {
        Round round(human, computer);
        if (m_totalHumanPoints == m_totalComputerPoints) {
            //by default, it will toss in round class
            roundPoints = round.play();
        }
        else {
            char firstPlayerSymbol;
            if (m_totalHumanPoints > m_totalComputerPoints) {
                std::cout << "Human points more , will play first\n";
                firstPlayerSymbol = 'H';
            }
            else {
                std::cout << "Computer points more ,will play first.\n";
                firstPlayerSymbol = 'C';
            }
            roundPoints = round.play(firstPlayerSymbol);
        }
    }

    m_totalHumanPoints += roundPoints.first;
    m_totalComputerPoints += roundPoints.second;

    std::cout << "Points this round: Human " << roundPoints.first << " - " << roundPoints.second << " Computer\n";
    std::cout << "Total Score: Human " << m_totalHumanPoints << " - " << m_totalComputerPoints << " Computer\n";
}
