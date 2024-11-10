//Board.h
#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "Constants.h"

class Board {
public:
    /**
     * @brief Initializes the game board with empty spaces
     * @param boarddata The board array to initialize
     */
    static void initBoard(int boarddata[BOARD_SIZE][BOARD_SIZE]);

    /**
     * @brief Draws the current state of the board
     * @param boarddata The current board state to display
     */
    static void drawBoard(const int boarddata[BOARD_SIZE][BOARD_SIZE]);

    /**
     * @brief Draws the game title
     */
    static void drawTitle();
};

#endif // BOARD_H