#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Game configuration constants
 */

const int BOARD_SIZE = 8;

namespace GameConfig {
    const int MAX_TURNS = 40;
    const int NUM_SHIPS_SIZE4 = 1;  // Number of 4-square ships
    const int NUM_SHIPS_SIZE3 = 2;  // Number of 3-square ships
    const int NUM_SHIPS_SIZE2 = 2;  // Number of 2-square ships

    // Board state constants
    const int EMPTY = 0;
    const int SHIP = 1;
    const int MISS = 2;
    const int HIT = 3;
}

#endif // CONSTANTS_H