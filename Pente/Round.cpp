
#include "Round.h"
#include "FileWriter.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

/* *********************************************************************
Function Name: Round (Constructor)
Purpose: To initialize a new round for the game with the provided human and computer players
Parameters:
            human, a pointer to a Player object representing the human player
            computer, a pointer to a Player object representing the computer player
Algorithm:
            1) Assign the human and computer players to member variables
            2) Reset the captures and points of both players to 0 for the new round
Assistance Received: None
********************************************************************* */

Round::Round(Player* human, Player* computer) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
    m_humanPlayer->setCaptures(0);
    m_computerPlayer->setCaptures(0);
    m_humanPlayer->setPoints(0);
    m_computerPlayer->setPoints(0);
}

/* *********************************************************************
Function Name: Round (Constructor with board initialization)
Purpose: To initialize a new round with  human and computer players, and from  previously saved board
Parameters:
            human, a pointer to a Player object representing the human player.
            computer, a pointer to a Player object representing the computer player
            loadedBoard, a constant reference to a Board object, representing the saved state of the board
Algorithm:
            1) Assign the human and computer players to member variables.
            2) Use the default copy assignment operator to copy the state of the loadedBoard into m_board. After this operation, both objects have the same state but are independent of each other.
Assistance Received: None
********************************************************************* */

Round::Round(Player* human, Player* computer, const Board& loadedBoard) {
    m_humanPlayer = human;
    m_computerPlayer = computer;
    m_board = loadedBoard;
}

/* *********************************************************************
Function Name: tossHumanComputer
Purpose: To perform a coin toss and determine who plays first: the human or the computer
Return Value: A character representing who plays first ('H' for human, 'C' for computer)
Algorithm:
            1) Get User Input (1/2) for heads or tails. generate random number 1/2 and do coin toss
            2) Display who won and gets the first turn
            3) Return the symbol of the player who plays first ('H' for human, 'C' for computer)
Assistance Received: None
********************************************************************* */

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

/* *********************************************************************
Function Name: displayCaptures
Purpose: To display the number of captures made by the human player and the computer
Parameters: None
Return Value: None
Algorithm:
            1) Fetch the number of captures made by the human   and computer players using the getCaptures method.
            2) Display the number of captures for both the human and the computer.
Assistance Received: None
********************************************************************* */

void Round::displayCaptures() const {
    std::cout << "Captures -\t Human: " << m_humanPlayer->getCaptures() << ",\t Computer: " << m_computerPlayer->getCaptures() << std::endl;
   
}

/* *********************************************************************
Function Name: takeTurn
Purpose: To handle the actions associated with a player's turn, including giving the option to save, quit, and execute the move.
Parameters:
    currentPlayer, A pointer to the Player object representing the current player (either human or computer).
    symbol, A character representing the symbol of the current player ('W' or 'B' for white or black respectively).
Return Value: None
Algorithm:
    1) Prompt the user with options to continue, save, or quit the game
    2) If the user chooses to save:
        a) Determine  next player
        b) Save the current game state using the FileWriter class
        c) On successful save, exit. Else, inform user
    3) If the user chooses to quit, exit the program
    5) Otherwise, execute player's move, update and display board and captures.
Assistance Received: None
********************************************************************* */

void Round::takeTurn(Player* currentPlayer, char symbol) {

    std::string userInput;
    std::cout << "\nEnter 'quit' to exit, or 'save' to save game and exit.\nPress any key to continue: ";
    std::cin >> userInput;
    std::cout<<std::endl;

    if (userInput == "save") {
        
        std::string nextPlayer = (currentPlayer->getPlayerType() == "Human") ? "Computer" : "Human";
        //symbol is current symbol, so must save opposite symbol
        std::string nextPlayerStone = (symbol == 'W') ? "Black" : "White";
        if (FileWriter::saveGame(&m_board, m_humanPlayer, m_computerPlayer, nextPlayer, nextPlayerStone)) {
            std::cout << "Game saved successfully!" << std::endl;
            //mark endRound member flag true
            m_endRound = true;

            exit(0);

        }
        else {
            std::cout << "Failed to save the game." << std::endl;
        }

    }
    else if (userInput == "quit") {
        std::cout << "Exited without saving successfully" << std::endl;
        exit(0);
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

/* *********************************************************************
Function Name: checkForEndOfRound
Purpose: To check if the current round has ended based on the flag value.
Parameters: None
Return Value:
    true if the round has ended.false otherwise.
Algorithm:
    1) Return the value of m_endRound member variable.
Assistance Received: None
********************************************************************* */

bool Round::checkForEndOfRound() const {

    return m_endRound;
}

/* *********************************************************************
Function Name: checkForFiveInARow
Purpose: To check if the last move resulted in five consecutive stones of the same type in any direction
Parameters:
    symbol, a character ('W' or 'B').
    currentPlayer, A pointer to the Player object representing the current player (either human or computer)
Return Value: A boolean indicating if there is a five-in-a-row sequence for the current player,true if there's a five-in-a-row else false
Algorithm:
    1) Get the coordinates of the last move played by the current player
    2) For each direction (horizontal, vertical, and the two diagonals):
        a) Count the number of consecutive stones in that direction and the opposite direction.
        b) If the total count, including the last move, is 5 or more:
            i) Announce the win for the current player.
            ii) Award 5 points to the current player.
            iii) Return true.
    3) If no five-in-a-row sequence, return false.
Assistance Received: None
********************************************************************* */

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

/* *********************************************************************
Function Name: numFourInARow
Purpose: Count sequences of four consecutive stones of the same type on the board
Parameters:
    symbol, a character ('W' or 'B') representing the stone type
Return Value: The number of sequences of four consecutive stones of the specified type on the board
Algorithm:
    1) Initialize a count for sequences of four to zero.
    2) For each cell with the specified symbol on the board:
        a) Check all directions (horizontal, vertical, two diagonals) for sequences of four
        b) Ensure sequences are not part of a five-in-a-row
        c) Increment the count for valid sequences of four
    3) Return the count
Assistance Received: None
********************************************************************* */

int Round::numFourInARow(char symbol) {
    int fourInARow = 0;
    std::array<std::pair<int, int>, 4> directions = { {{1, 0}, {0, 1}, {1, 1}, {1, -1}} };

    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {

            if (m_board.getCell(i, j) != symbol) { continue; }

            for (int k = 0; k < directions.size(); ++k) {
                int dx = directions[k].first;
                int dy = directions[k].second;
                int count = 1; // counting the current cell itself
                count += m_board.countConsecutiveStones(i, j, dx, dy, symbol);

                if (count == 4) { 

                    //MAKING SURE IT IS NOT A PART OF FIVE IN A ROW
                    int nextX = i + 4 * dx;
                    int nextY = j + 4 * dy;
                    int prevX = i - dx;
                    int prevY = j - dy;
                    if ((nextX < 0 || nextX >= 19 || nextY < 0 || nextY >= 19 || m_board.getCell(nextX, nextY) != symbol) &&
                        (prevX < 0 || prevX >= 19 || prevY < 0 || prevY >= 19 || m_board.getCell(prevX, prevY) != symbol)) {
                        fourInARow++;
                    }
                    
                }
                
            }
        }
    }

   return fourInARow;
}

/* *********************************************************************
Function Name: checkForFiveCaptures
Purpose: Check if a player has captured at least five opponent stones
Parameters:
    currentPlayer, A pointer to the Player object representing the current player (either human or computer)
Return Value: True if the current player has made five or more captures, otherwise false
Algorithm:
    1) Fetch the number of captures made by the current player
    2) If captures are five or more:
        a) Announce the win for the current player due to captures.
        b) Return true
    3) Otherwise, return false
Assistance Received: None
********************************************************************* */

bool Round::checkForFiveCaptures(Player* currentPlayer) const {
    if (currentPlayer->getCaptures() >= 5) {
        std::cout << currentPlayer->getPlayerType() << " wins!--due to 5 captures of opponent stones" << std::endl;
        return true;
    }
    return false;
}

/* *********************************************************************
Function Name: checkForCapture
Purpose: Check if a stone played by the current player resulted in capturing any adjacent opponent stones
Parameters:
    symbol, a character ('W' or 'B') representing the stonetype
    currentPlayer, A pointer to the Player object representing the current player (either human or computer)
Return Value: True if the current player's stone led to a capture, otherwise false
Algorithm:
    1) Retrieve the coordinates of the last move played by the current player
    2) Determine the symbol of the opponent's stone
    3) For each possible direction around the stone:
        a) Check for any captures in that direction using the captureStones function of the board
        b) If a capture is detected:
            i) Increment the capture count of the current player
            ii) Announce the capture, specifying the type of stones that were captured
Assistance Received: None
********************************************************************* */

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

/* *********************************************************************
Function Name: calculateScore
Purpose: To compute and update the score for a player for captures and 4-in-a-row
Parameters:
    symbol, a character ('W' or 'B') representing the stone of the player
    player, a pointer to the Player object whose score needs to be calculated
Algorithm:
    1) Add the number of captures by the player to their score.
    2) Calculate the number of 4-in-a-row occurrences and add them to the player's score
Assistance Received: None
****************************************************/

void Round::calculateScore(char symbol, Player* player) {
  
    // 5 points for  5  in a row
    // This is already handled in the checkForFiveInARow function, no need to do anything here

    //1 points per capture
    int capturePoints = player->getCaptures();
    player->addPoints(capturePoints);

    //1 points per 4 in a row
    int fourInARowPoints = numFourInARow(symbol);
    player->addPoints(fourInARowPoints);

}

/* *********************************************************************
Function Name: playGame
Purpose: To simulate the gameplay between human and computer players
Parameters:
    currentPlayer, a pointer reference to the Player object, either human or computer
    currentSymbol, a reference to the character ('W' or 'B') representing the stone 
Algorithm:
    1) Display the game board.
    2) Loop until the end conditions of the round are met:
       a) Take a turn for the current player
       b) Check for winning conditions, if met, exit the loop
       c) Swap the current player and symbol.
    3) Calculate and update scores for both players
Assistance Received: None
*********************************************/
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

/* *********************************************************************
Function Name: play
Purpose: To start the game 
Parameters:
    firstPlayerSymbol, a character representing the first player H for human, C for computer
Return Value: A pair of integers representing scores of human and computer players respectively
Algorithm:
    1) Determine the starting player and their symbol based on firstPlayerSymbol
    2) Start the game simulation.
    3) Return the scores of human and computer players
Assistance Received: None
****************************************************/

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

/* *********************************************************************
Function Name: resume
Purpose: To continue the gameplay from the last saved state
Parameters:
    currentPlayer, a pointer to the Player object resuming the game (either human or computer)
    currentSymbol, a character ('W' or 'B') representing the stone of the resuming player
Return Value: A pair of integers representing scores of human and computer players respectively
Algorithm:
    1) Continue the game simulation using the provided player and symbol
    2) Return the scores of human and computer players.
Assistance Received: None
*******************************************************************/
std::pair<int, int> Round::resume(Player* currentPlayer, char currentSymbol) {

    playGame(currentPlayer, currentSymbol);

    return std::make_pair(m_humanPlayer->getPoints(), m_computerPlayer->getPoints());
}