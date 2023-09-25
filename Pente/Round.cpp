
#include "Round.h"
#include "FileWriter.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

Round::Round(Player* human, Player* computer) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
    m_humanPlayer->setCaptures(0);
    m_computerPlayer->setCaptures(0);
    m_humanPlayer->setPoints(0);
    m_computerPlayer->setPoints(0);
}

Round::Round(Player* human, Player* computer, const Board& loadedBoard) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
    m_board = loadedBoard;
}

char Round::tossHumanComputer() const {
    char firstPlayerSymbol;
    
    int call;

    while(true){

        std::cout << "Toss a coin! ( 1 == heads, 2 == tails): ";
        std::cin >> call;

        if (call == 1 || call == 2) {
            break; // Exit the loop if the input is valid
        }
        else {
            std::cout << "Invalid choice. Enter 1 for heads, 2 for tails" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        }

    } 
    std::srand(std::time(0));
    int tossResult = (std::rand() % 2) + 1;
    std::cout << "Coin toss result: " << (tossResult == 1 ? "Heads" : "Tails") << std::endl;

    firstPlayerSymbol = (tossResult == call) ? 'H' : 'C';
    std::cout << ((firstPlayerSymbol == 'H') ? "You will play first." : "Computer will play first.") << std::endl << std::endl;

    return firstPlayerSymbol;
}

void Round::displayCaptures() const {
    std::cout << "Captures -\t Human: " << m_humanPlayer->getCaptures() << ",\t Computer: " << m_computerPlayer->getCaptures() << std::endl;
   
}

void Round::takeTurn(Player* currentPlayer, char symbol) {

    

    std::string userInput;
    std::cout << "\nEnter 'quit' to save & exit game, or press any other key to continue: ";
    std::cin >> userInput;

    if (userInput == "quit") {
        FileWriter writer;

        std::string nextPlayer = (currentPlayer->getPlayerType() == "Human") ? "Computer" : "Human";
        //symbol is current symbol, so must save opposite symbol
        std::string nextPlayerStone = (symbol == 'W') ? "Black" : "White";
        if (writer.saveGame(&m_board, m_humanPlayer, m_computerPlayer, nextPlayer, nextPlayerStone)) {
            std::cout << "Game saved successfully!" << std::endl;
            //mark endRound member flag truek0
            m_endRound = true;

        }
        else {
            std::cout << "Failed to save the game." << std::endl;
        }

    }

    currentPlayer->play(m_board, symbol);
    std::pair<int, int> location = currentPlayer->getLocation();
    int x = location.first;
    int y = location.second;

    m_board.setCell(x, y, symbol);
    m_board.displayBoard();
    displayCaptures();

    if (checkForCapture(symbol, currentPlayer)) {
        m_board.displayBoard();
        displayCaptures();
    }
}

bool Round::checkForEndOfRound() const {

    return m_endRound;
}

bool Round::checkForFiveInARow(char symbol, Player* currentPlayer) const {

    std::pair<int, int> lastMove = currentPlayer->getLocation();
    int x = lastMove.first;
    int y = lastMove.second;

    //1,0) for x-axis,( 0,1) for y-axis, (1,1) for x=y line, (1,-1) for x=-y line
    std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };

    for (int i = 0; i < directions.size(); ++i) {
        int dx = directions[i].first;
        int dy = directions[i].second;
        int count = 1; //counting the last move itself

        count += m_board.countConsecutiveStones(x, y, dx, dy, symbol);
        count += m_board.countConsecutiveStones(x, y, -dx, -dy, symbol);

        if (count >= 5) {
            std::cout << currentPlayer->getPlayerType() << " wins!--due to 5 in a row" << std::endl;

            // 5 points to the player with 5 in a row
            currentPlayer->addPoints(5);
           return true;
        }
    }
    return false;
}


bool Round::checkForFiveCaptures(Player* currentPlayer) const {
    if (currentPlayer->getCaptures() >= 5) {
        std::cout << currentPlayer->getPlayerType() << " wins!--due to 5 captures of opponent stones" << std::endl;
        return true;
    }
    return false;
}



bool Round::checkForCapture(char symbol, Player* currentPlayer)  {

    std::pair<int, int> lastMove = currentPlayer->getLocation();
    int x = lastMove.first;
    int y = lastMove.second;
    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    bool capture = false;

    //  Checking in different possible directions
    std::array<std::pair<int, int>, 8> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {-1, -1}, {-1, 1}} };

    for (int i = 0; i < directions.size(); ++i) {
        int dx = directions[i].first;
        int dy = directions[i].second;

        if (m_board.captureStones(x, y, dx, dy, symbol)) {
            capture = true;

            currentPlayer->addCaptures();
            

            std::cout << currentPlayer->getPlayerType() << " with symbol " << symbol << " captured a pair of "
                << opponentSymbol << " stones " << "!\n\n";
             
        }
    }
    return capture;
}

void Round::calculateScore(char symbol, Player* player) {
  
    // 5 points for  5  in a row
    // This is already handled in the checkForFiveInARow function, no need to do anything here

    //1 points per capture
    int capturePoints = player->getCaptures();
    player->addPoints(capturePoints);

    //1 points per 4 in a row

    int fourInARowPoints = 0;
    std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };

    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            for (int k = 0; k < directions.size(); ++k) {
                int dx = directions[k].first;
                int dy = directions[k].second;
                
                int countForward = m_board.countConsecutiveStones(i, j, dx, dy, symbol);
                int countBackward = m_board.countConsecutiveStones(i, j, -dx, -dy, symbol);
                int totalCount = 1 + countForward + countBackward; 

                if (totalCount == 4) {
                   //DO SOMETHING I DONT KNOW WHAT TO DO
                }
            }
        }
    }

    player->addPoints(fourInARowPoints);

}

//currentPlayer parameter =>passed by reference  so that it can be modified.
void Round::playGame(Player*& currentPlayer, char& currentSymbol) {

    m_board.displayBoard();
    

    do {
       
        takeTurn(currentPlayer, currentSymbol);
       
        if (checkForFiveInARow(currentSymbol, currentPlayer) || checkForFiveCaptures(currentPlayer)) {
            
            break;
        }
        // Swap current player and symbol
        
        if (currentPlayer == m_humanPlayer) {
            currentPlayer = m_computerPlayer;

        }
        else {
            currentPlayer = m_humanPlayer;

        }
        currentSymbol = (currentSymbol == 'W') ? 'B' : 'W';

    } while (!checkForEndOfRound());

    //we are here means, game has ended
    char opponentSymbol = (currentSymbol == 'W') ? 'B' : 'W';
    Player* opponentPlayer = (currentPlayer == m_humanPlayer) ? m_computerPlayer : m_humanPlayer;

    calculateScore(currentSymbol,currentPlayer);
    calculateScore(opponentSymbol, opponentPlayer);
}

std::pair<int, int> Round::play(char firstPlayerSymbol) {

    if (firstPlayerSymbol == ' ') {
        firstPlayerSymbol = tossHumanComputer();
    }
    char currentSymbol = 'W'; // The first player plays white stones.
    Player* currentPlayer;
    if (firstPlayerSymbol == 'H') {
        currentPlayer = m_humanPlayer;
    }
    else {
        currentPlayer = m_computerPlayer;
    }

    playGame(currentPlayer, currentSymbol);

    return std::make_pair(m_humanPlayer->getPoints(), m_computerPlayer->getPoints());
}


std::pair<int, int> Round::resume(Player* currentPlayer, char currentSymbol) {

    playGame(currentPlayer, currentSymbol);

    return std::make_pair(m_humanPlayer->getPoints(), m_computerPlayer->getPoints());
}