//ComputerPlayer.h
#pragma once
#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include <vector>
#include <utility>
#include <random>
#include "Constants.h"

class ComputerPlayer {
private:
    std::vector<std::pair<int, int>> potentialTargets;  // Stores coordinates to try after a hit
    std::vector<std::pair<int, int>> availableMoves;    // All remaining possible moves
    std::mt19937 rng;                                   // Random number generator

    // Last successful hit coordinates
    int lastHitRow;
    int lastHitCol;
    bool hasLastHit;

    // Helper methods
    void initializeAvailableMoves();
    void addAdjacentSquares(int row, int col);
    std::pair<int, int> selectRandomMove();
    bool isValidCoordinate(int row, int col) const;
    void removeMove(const std::pair<int, int>& move);

public:
    ComputerPlayer();
    std::pair<int, int> makeMove();
    void processResult(int row, int col, bool isHit, bool isSunk);
};

#endif // COMPUTER_PLAYER_H