//Game.cpp
#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
    Board::initBoard(boarddata);
    numShips = GameConfig::NUM_SHIPS_SIZE4 + GameConfig::NUM_SHIPS_SIZE3 + GameConfig::NUM_SHIPS_SIZE2;
    ships = new Ship[numShips];

    int shipIndex = 0;
    for (int i = 0; i < GameConfig::NUM_SHIPS_SIZE4; i++) {
        ships[shipIndex++] = Ship(4, boarddata);
    }
    for (int i = 0; i < GameConfig::NUM_SHIPS_SIZE3; i++) {
        ships[shipIndex++] = Ship(3, boarddata);
    }
    for (int i = 0; i < GameConfig::NUM_SHIPS_SIZE2; i++) {
        ships[shipIndex++] = Ship(2, boarddata);
    }

    wonGame = false;
}

Game::~Game() {
    delete[] ships;  // Delete the dynamically allocated array of ships
}

void Game::run() {
    const int MAX_TURNS = 40;
    int row, col;

    for (int turn = 0; turn < MAX_TURNS; turn++) {
        if (wonGame) {
            Board::drawTitle();
            GameMessages::drawMessage(WIN1);
            Board::drawBoard(boarddata);
            cin.ignore();
            cin.get();
            Board::drawTitle();
            GameMessages::drawMessage(WIN2);
            Board::drawBoard(boarddata);
            break;
        }

        // Get row input
        Board::drawTitle();
        GameMessages::drawMessage(ROW);
        Board::drawBoard(boarddata);
        do {
            cin >> row;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                row = 0;
            }
        } while (row < 1 || row > BOARD_SIZE);

        // Get column input
        Board::drawTitle();
        GameMessages::drawMessage(COLUMN);
        Board::drawBoard(boarddata);
        do {
            cin >> col;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                col = 0;
            }
        } while (col < 1 || col > BOARD_SIZE);

        cin.ignore();

        // Process the guess and handle the result
        bool validTurn = processGuess(row - 1, col - 1);
        if (!validTurn) {
            turn--;  // Don't count invalid turns
        }

        cin.get();
    }

    // Handle game loss if player didn't win
    if (!wonGame) {
        Board::drawTitle();
        GameMessages::drawMessage(LOSE1);
        Board::drawBoard(boarddata);
        cin.ignore();
        cin.get();
        Board::drawTitle();
        GameMessages::drawMessage(LOSE2);
        Board::drawBoard(boarddata);
    }
}

bool Game::processGuess(int row, int col) {
    switch (boarddata[row][col]) {
    case 0: // Empty square - Miss
        boarddata[row][col] = 2;
        Board::drawTitle();
        GameMessages::drawMessage(MISS);
        Board::drawBoard(boarddata);
        return true;

    case 1: // Ship hit
        boarddata[row][col] = 3;
        Board::drawTitle();
        GameMessages::drawMessage(HIT);
        Board::drawBoard(boarddata);

        // Check which ship was hit and update its status
        for (int i = 0; i < numShips; i++) {
            for (int j = 0; j < 5; j++) {
                if (ships[i].squares[j][0] == row && ships[i].squares[j][1] == col) {
                    ships[i].squares[j][2] = 0;

                    // Check if ship was sunk
                    if (ships[i].isSunk()) {
                        cin.get();
                        Board::drawTitle();
                        GameMessages::drawMessage(SINK);
                        Board::drawBoard(boarddata);

                        // Check if all ships are sunk (game won)
                        wonGame = true;
                        for (int ship = 0; ship < numShips; ship++) {
                            if (!ships[ship].isSunk()) {
                                wonGame = false;
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
        return true;

    case 2: // Already guessed - Miss
    case 3: // Already guessed - Hit
        Board::drawTitle();
        Board::drawBoard(boarddata);
        GameMessages::drawMessage(RETRY);
        return false;

    default: // Error case
        Board::drawTitle();
        Board::drawBoard(boarddata);
        GameMessages::drawMessage(ERROR);
        return false;
    }
}

bool Game::isGameWon() const {
    return wonGame;
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Optional debugging method to reveal ship positions
void Game::revealShips() {
    for (int i = 0; i < numShips; i++) {
        cout << "Ship " << i + 1 << " positions:\n";
        for (int j = 0; j < 5; j++) {
            if (ships[i].squares[j][0] != -1) {
                cout << "  Square " << j + 1 << ": ("
                    << ships[i].squares[j][0] + 1 << ", "
                    << ships[i].squares[j][1] + 1 << ")\n";
            }
        }
        cout << "\n";
    }
}

// Added helper method to get the number of remaining ships
int Game::getRemainingShips() const {
    int remaining = 0;
    for (int i = 0; i < numShips; i++) {
        if (!ships[i].isSunk()) {
            remaining++;
        }
    }
    return remaining;
}

// Added method to get the current board state at a position
int Game::getBoardState(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return boarddata[row][col];
    }
    return -1;  // Invalid position
}