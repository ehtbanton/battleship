//Game.h
#pragma once
#ifndef GAME_H
#define GAME_H

#include "Ship.h"
#include "Board.h"
#include "GameMessages.h"


// Main game class that manages the battleship game
class Game {
protected:
    int boarddata[BOARD_SIZE][BOARD_SIZE];  // Game board state
    Ship* ships;                            // Array of ships in the game
    int numShips;                           // Total number of ships
    bool wonGame;                           // Game win status

    // Clears the console screen
    void clearScreen();

public:
    // Default constructor - initializes game board and places ships
    Game();

     // Destructor - cleans up dynamically allocated ships
    ~Game();

    // Main game loop - handles player turns and game flow
    void run();

    // Processes a player's guess at the given coordinates
    bool processGuess(int row, int col);

    // Gets the current win status of the game
    bool isGameWon() const;

    // Debug method to show all ship positions
    void revealShips();

    // Gets the number of ships that haven't been sunk
    int getRemainingShips() const;

    // Gets the state of a board position
    // 0 = Empty
    // 1 = Ship
    // 2 = Miss
    // 3 = Hit
    int getBoardState(int row, int col) const;

};

#endif // GAME_H