#include "Ship.h"

Ship::Ship(int size, int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    if (size < 1 || size > MAX_SHIP_SIZE) {
        std::cout << "Invalid ship size\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - size);

    int orientation = dis(gen) % 2;
    bool valid = false;
    int x, y;

    do {
        x = dis(gen);
        y = dis(gen);
        valid = true;

        if (orientation == 0) { // Horizontal
            if (x + size > BOARD_SIZE) continue;
            for (int i = 0; i < size; i++) {
                if (boarddata[x + i][y] != GameConfig::EMPTY) {
                    valid = false;
                    break;
                }
            }
        }
        else { // Vertical
            if (y + size > BOARD_SIZE) continue;
            for (int i = 0; i < size; i++) {
                if (boarddata[x][y + i] != GameConfig::EMPTY) {
                    valid = false;
                    break;
                }
            }
        }
    } while (!valid);

    for (int i = 0; i < MAX_SHIP_SIZE; i++) {
        if (i < size) {
            squares[i][0] = (orientation == 0) ? x + i : x;
            squares[i][1] = (orientation == 0) ? y : y + i;
            squares[i][2] = 1;
            boarddata[squares[i][0]][squares[i][1]] = GameConfig::SHIP;
        }
        else {
            squares[i][0] = -1;
            squares[i][1] = -1;
            squares[i][2] = 0;
        }
    }
}

Ship::Ship() {
    for (int i = 0; i < MAX_SHIP_SIZE; i++) {
        squares[i][0] = -1;
        squares[i][1] = -1;
        squares[i][2] = 0;
    }
}

bool Ship::isSunk() const {
    for (int i = 0; i < MAX_SHIP_SIZE; i++) {
        if (squares[i][2] == 1) {
            return false;
        }
    }
    return true;
}