
#include "Round.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

Round::Round(Player* human, Player* computer) {
    humanPlayer = human;
    computerPlayer = computer;
    humanPoints = 0;
    computerPoints = 0;
    humanCaptures = 0;
    computerCaptures = 0;
}


char Round::determineFirstPlayer() {
    char firstPlayerSymbol;
    // Initialized as true for first round, in the future it will be false for all instances
    static bool isFirstRound = true;
    bool isTossRequired = isFirstRound || (humanPoints == computerPoints);

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
        firstPlayerSymbol = (humanPoints > computerPoints) ? 'H' : 'C';
        std::cout << ((firstPlayerSymbol == 'H') ? "You have more points. You will play first." : "Computer has more points. Computer will play first.") << std::endl;
    }

    return firstPlayerSymbol;
}



void Round::takeTurn(Player* currentPlayer, char symbol) {

    currentPlayer->play(board);
    std::pair<int, int> location = currentPlayer->getLocation();
    int x = location.first;
    int y = location.second;

    board.setCell(x, y, symbol);
    
    board.displayBoard();
}

bool Round::checkForEndOfRound() {
    
    return false;
}

bool Round::checkForWin(char symbol, Player* currentPlayer) {

    std::pair<int, int> lastMove = currentPlayer->getLocation();
    int x = lastMove.first;
    int y = lastMove.second;

    //1,0) for x-axis,( 0,1) for y-axis, (1,1) for x=y line, (1,-1) for x=-y line
    std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };

    for (int i = 0; i < directions.size(); ++i) {
        int dx = directions[i].first;
        int dy = directions[i].second;
        int count = 1; //counting the last move itself

        count += checkDirection(x, y, dx, dy, symbol);
        count += checkDirection(x, y, -dx, -dy, symbol);

        if (count >= 5) {
            return true;
        }
    }
                
           

    return false;
}

int Round::checkDirection(int x, int y, int dx, int dy, char symbol) {
    int count = 0;
    for (int i = 1; i < 5; ++i) {
        int newX = x + i * dx;
        int newY = y + i * dy;

        // Check if the index is out of bounds or isnot same stone
        if (newX < 0 || newX >= 19 || newY < 0 || newY >= 19 || board.getCell(newX, newY) != symbol) {
            break;
        }

        ++count;
    }
    return count;
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
            if (currentPlayer == humanPlayer) {
                ++humanCaptures;
                std::cout << "Nice, Human with symbol " << symbol << " captured a pair of stones at:" << char('A' + capture1.second) << (capture1.first + 1)
                    << " and " << char('A' + capture2.second) << (capture2.first + 1) << "!\n";

            }
            else {
                ++computerCaptures;
                std::cout << "Damn, Computer with symbol " << symbol << " captured a pair of your stones at:" << char('A' + capture1.second) << (capture1.first + 1)
                    << " and " << char('A' + capture2.second) << (capture2.first + 1) << "!\n";
            }

            
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

    char first = board.getCell(x + dx, y + dy);
    char second = board.getCell(x + 2 * dx, y + 2 * dy);
    char third = board.getCell(x + 3 * dx, y + 3 * dy);

    char opponentSymbol = (symbol == 'W') ? 'B' : 'W';
    if (first == opponentSymbol && second == opponentSymbol && third == symbol) {
        
            // Capture the pair
            board.setCell(x + dx, y + dy, '*');
            board.setCell(x + 2 * dx, y + 2 * dy, '*');

            //updating them so that parent function can access them
            capture1 = std::make_pair(x + dx, y + dy);
            capture2 = std::make_pair(x + 2 * dx, y + 2 * dy);
            return true;
        
    }
    return false;
}

void Round::updateScore() {
    
}

std::pair<int,int> Round::play() {

    /*board.displayBoard();*/

    char firstPlayerSymbol = determineFirstPlayer();


    char currentSymbol = 'W'; // The first player plays white stones.
    Player* currentPlayer;
    if (firstPlayerSymbol == 'H') {
        currentPlayer = humanPlayer;
    }
    else {
        currentPlayer = computerPlayer;
    }


    while (!checkForEndOfRound()) {
        takeTurn(currentPlayer, currentSymbol);

        if (checkForWin(currentSymbol, currentPlayer)) {
            std::cout << "Player " << currentSymbol << " wins the round!\n";
            updateScore();

            return std::make_pair(humanPoints, computerPoints);
        }

        if (checkForCapture(currentSymbol,currentPlayer)) {
            /*
            if (currentPlayer == humanPlayer) {
                ++humanCaptures;
            }
            else {
                ++computerCaptures;
            }*/
            updateScore();
        }

        // Swap current player and symbol

        if (currentPlayer == humanPlayer) {
            currentPlayer = computerPlayer;
            
        }
        else {
            currentPlayer = humanPlayer;
            
        }
        //currentPlayer = (currentPlayer == humanPlayer) ? computerPlayer : humanPlayer;
        currentSymbol = (currentSymbol == 'W') ? 'B' : 'W';
    }

    return std::make_pair(humanPoints, computerPoints);
}
