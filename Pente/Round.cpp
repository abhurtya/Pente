
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


char Round::determineFirstPlayer() {
    char firstPlayerSymbol;
    // Initialized as true for first round, in the future it will be false for all instances
    static bool isFirstRound = true;
    bool isTossRequired = isFirstRound || (m_humanPlayer->getPoints() == m_computerPlayer->getPoints());

    if (isTossRequired) {

        if (isFirstRound) {
            std::cout << "It's the first round. ";
        }
        else {
            std::cout << "Both huamn and computer have same points.\n ";
        }
        int call;
        std::cout << "Toss a coin! (Enter 1 for heads or 2 for tails):: ";
        std::cin >> call;

        std::srand(std::time(0));
        int tossResult = (std::rand() % 2) + 1;
        std::cout << "The coin toss result is: " << (tossResult == 1 ? "Heads" : "Tails") << std::endl;

        firstPlayerSymbol = (tossResult == call) ? 'H' : 'C';
        std::cout << ((firstPlayerSymbol == 'H') ? "You will play first." : "Computer will play first.") << std::endl;

        isFirstRound = false;  // Reset for future rounds
    }
    else {
        firstPlayerSymbol = ((m_humanPlayer->getPoints()) > (m_computerPlayer->getPoints())) ? 'H' : 'C';
        std::cout << ((firstPlayerSymbol == 'H') ? "You have more points. You will play first." : "Computer has more points. Computer will play first.") << std::endl;
    }

    return firstPlayerSymbol;
}



void Round::takeTurn(Player* currentPlayer, char symbol) {

    currentPlayer->play(m_board,symbol);
    std::pair<int, int> location = currentPlayer->getLocation();
    int x = location.first;
    int y = location.second;

    m_board.setCell(x, y, symbol);
    m_board.displayBoard();

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
            //mark endRound member flag true
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

    //to store co-ordinates of caputures , if any
    std::pair<int, int> capture1, capture2;

    //  Checking in different possible directions
    std::array<std::pair<int, int>, 8> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {-1, -1}, {-1, 1}} };

    for (int i = 0; i < directions.size(); ++i) {
        int dx = directions[i].first;
        int dy = directions[i].second;

        if (checkForCaptureDirection(x, y, dx, dy, symbol, capture1, capture2)) {
            capture = true;

            currentPlayer->addCaptures();
            currentPlayer->addPoints(1);

            std::cout << currentPlayer->getPlayerType() << " with symbol " << symbol << " captured a pair of"
                << opponentSymbol << " stones at : " << char('A' + capture1.second) << (capture1.first + 1)
                << " and " << char('A' + capture2.second) << (capture2.first + 1) << "!\n";

        }
    }
    return capture;
}

bool Round::checkForCaptureDirection(int x, int y, int dx, int dy, char symbol, std::pair<int, int>& capture1, std::pair<int, int>& capture2) {

    // early return if the index is out of bounds
    for (int i = 1; i <= 3; ++i) {
        int newX = x + i * dx;
        int newY = y + i * dy;
        if (newX < 0 || newX >= 19 || newY < 0 || newY >= 19) {
            return false;
        }
    }

    char first = m_board.getCell(x + dx, y + dy);
    char second = m_board.getCell(x + 2 * dx, y + 2 * dy);
    char third = m_board.getCell(x + 3 * dx, y + 3 * dy);

    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    if (first == opponentSymbol && second == opponentSymbol && third == symbol) {

        // Capture the pair
        m_board.setCell(x + dx, y + dy, '*');
        m_board.setCell(x + 2 * dx, y + 2 * dy, '*');

        //updating them so that parent function can access them
        capture1 = std::make_pair(x + dx, y + dy);
        capture2 = std::make_pair(x + 2 * dx, y + 2 * dy);
        return true;

    }
    return false;
}

void Round::updateScore(int points, Player* currentPlayer) {



}

//currentPlayer parameter =>passed by reference  so that it can be modified.
void Round::playGame(Player*& currentPlayer, char& currentSymbol) {
    do {
        takeTurn(currentPlayer, currentSymbol);

        if (checkForCapture(currentSymbol, currentPlayer)) {
            //updateScore();
        }

        if (checkForWin(currentSymbol, currentPlayer)) {
            //updateScore();
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

std::pair<int, int> Round::play() {

    /*board.displayBoard();*/

    char firstPlayerSymbol = determineFirstPlayer();


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