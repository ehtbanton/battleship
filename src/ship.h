#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <random>
#include "Constants.h"

const int MAX_SHIP_SIZE = 5;

class Ship {
public:
    // Stores ship coordinates and hit status
    // squares[][0] = x coordinate
    // squares[][1] = y coordinate
    // squares[][2] = hit status (1 = not hit, 0 = hit)
    int squares[MAX_SHIP_SIZE][3];

    /**
     * @brief Default constructor - creates an empty ship
     */
    Ship();

    /**
     * @brief Creates a ship of specified size and places it on the board
     * @param size Length of the ship (1-5)
     * @param boarddata Reference to the game board
     */
    Ship(int size, int boarddata[BOARD_SIZE][BOARD_SIZE]);

    /**
     * @brief Checks if the ship has been completely destroyed
     * @return true if all squares have been hit, false otherwise
     */
    bool isSunk() const;
};

#endif // SHIP_H