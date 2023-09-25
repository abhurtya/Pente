
#include "Round.h"
#include "FileWriter.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

Round::Round(Player* human, Player* computer) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
}

Round::Round(Player* human, Player* computer, const Board& loadedBoard) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
    m_board = loadedBoard;
}

char Round::tossHumanComputer() {
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



void Round::takeTurn(Player* currentPlayer, char symbol) {

    currentPlayer->play(m_board,symbol);
    std::pair<int, int> location = currentPlayer->getLocation();
    int x = location.first;
    int y = location.second;

    m_board.setCell(x, y, symbol);
    m_board.displayBoard();

    if (checkForCapture(symbol, currentPlayer)) {
        m_board.displayBoard();
    }

    std::string userInput;
    std::cout << "Enter 'quit' to save & exit game, or press any other key to continue: ";
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
}

bool Round::checkForEndOfRound() {

    return m_endRound;
}

bool Round::checkForWin(char symbol, Player* currentPlayer) {

    bool hasWon = false;

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
            hasWon = true;
        }
    }

    if (currentPlayer->getCaptures() >= 5) {
        std::cout << currentPlayer->getPlayerType() << " wins!--due to 5 captures of opponent stones" << std::endl;
        hasWon = true;
    }

    return hasWon;
}


bool Round::checkForCapture(char symbol, Player* currentPlayer) {

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
            currentPlayer->addPoints(1);

            std::cout << currentPlayer->getPlayerType() << " with symbol " << symbol << " captured a pair of "
                << opponentSymbol << " stones " << "!\n\n";
             
        }
    }
    return capture;
}

void Round::updateScore() {



}

//currentPlayer parameter =>passed by reference  so that it can be modified.
void Round::playGame(Player*& currentPlayer, char& currentSymbol) {

    m_board.displayBoard();

    do {
       
        takeTurn(currentPlayer, currentSymbol);
       
        if (checkForWin(currentSymbol, currentPlayer)) {
            m_board.displayBoard();
            updateScore();
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