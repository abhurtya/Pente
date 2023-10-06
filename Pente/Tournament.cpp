#include "Tournament.h"
#include <iostream>

/* *********************************************************************
Function Name: Tournament (constructor)
Purpose: Constructor to initialize the Tournament class with human and computer players
Parameters:
    human, a pointer to the Player object representing the human player
    computer, a pointer to the Player object representing the computer player
Assistance Received: None
**************************************/
Tournament::Tournament(Player* human, Player* computer)
    : m_human(human), m_computer(computer), m_totalHumanPoints(0), m_totalComputerPoints(0) {}

/* *********************************************************************
Function Name: ~Tournament (destructor)
Purpose: Destructor for the Tournament class
Assistance Received: None
******************************************/
Tournament::~Tournament() {
    
}

/* *********************************************************************
Function Name: startGame
Purpose: To start and manage the flow of the tournament until the user decides to end
Algorithm:
    1) Continuously play rounds until the user decides not to continue.
    2) Announce the overall tournament winner
Assistance Received: None
*************************************************/
void Tournament::startGame() {
   
    do {
        playRound(m_human, m_computer);
    } while (askUserPlay());
    announceTournamentWinner();
}

/* *********************************************************************
Function Name: resumeGame
Purpose: To resume the tournament from a saved state
Parameters:
    loadedBoard, a reference to the Board object representing the saved game state
    human, a pointer to the Player object representing the human player
    computer, a pointer to the Player object representing the computer player
    nextPlayerName, a string representing the name of the next player to play
    nextPlayerSymbol, a character representing the symbol of the next player to play
Algorithm:
    1) Set the overall scores and display them.
    2) Resume the current round and then continuously play new rounds until the user decides not to continue.
    3) Announce the overall tournament winner.
Assistance Received: None
***************************************/
void Tournament::resumeGame(Board& loadedBoard, Player* human, Player* computer, std::string nextPlayerName, char nextPlayerSymbol) {
    m_totalHumanPoints = human->getPoints();
    m_totalComputerPoints = computer->getPoints();
    std::cout << "Total tournament Score:\t Human " << m_totalHumanPoints << " - " << m_totalComputerPoints << " Computer\n";
    std::cout << "Next Player: " << nextPlayerName << "\t Playing Symbol: " << nextPlayerSymbol << std::endl;

    playRound(human, computer, &loadedBoard, nextPlayerName, nextPlayerSymbol);
    while (askUserPlay()) {
        playRound(m_human, m_computer);
    }
    announceTournamentWinner();
}

/* *********************************************************************
Function Name: askUserPlay
Purpose: To ask the user if they wish to play another round
Return Value: true if the user wants to play again, false otherwise
Algorithm:
    1) Prompt the user to continue playing or not.  Return user decision
Assistance Received: None
*************************************************/

bool Tournament::askUserPlay() const {
    char continuePlaying;

    while (true)
    {
        std::cout << "Do you wanna play another round? (y/n): ";
        std::cin >> continuePlaying;
        if (continuePlaying == 'y' || continuePlaying == 'Y' || continuePlaying == 'n' || continuePlaying == 'N') {
            break;  // Exit loop if input valid
        }
        else {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no" << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
        }
    }
    return (continuePlaying == 'y' || continuePlaying == 'Y');
}

/* *********************************************************************
Function Name: announceTournamentWinner
Purpose: To display the winner of the tournament based on total points
Algorithm:
    1) Compare the total points of human and computer.
    2) Announce the winner or declare a draw if points are equal
Assistance Received: None
***************************************************/

void Tournament::announceTournamentWinner() const {
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

/* *********************************************************************
Function Name: playRound
Purpose: To play a single round in the tournament, either start or resume
Parameters:
    human, a pointer to the Player object representing the human player
    computer, a pointer to the Player object representing the computer player
    loadedBoard, a pointer to the Board object representing the saved game state (null if not resuming)
    nextPlayerName, a string representing the name of the next player to play (used if resuming)
    nextPlayerSymbol, a character representing the symbol of the next player to play (used if resuming)
Algorithm:
    1) Reset players' points for a new round (remove after debugging).
    2) Depending on whether we're resuming or not, initiate a round.
    3) Calculate and display the scores after the round.
Assistance Received: None
********************************************************************* */

void Tournament::playRound(Player* human, Player* computer, Board* loadedBoard, std::string nextPlayerName, char nextPlayerSymbol) {

    //this part is unnecessary, b/c we reset points and captures for new round , delete after Debugging
 /*   human->setPoints(0);
    computer->setPoints(0);*/
 
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

    std::cout << "Points this round: \tHuman " << roundPoints.first << " - " << roundPoints.second << " Computer\n";
    std::cout << "Total tournament Score:\t Human " << m_totalHumanPoints << " - " << m_totalComputerPoints << " Computer\n";
}
