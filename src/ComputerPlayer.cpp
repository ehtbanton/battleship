//ComputerPlayer.cpp
#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer() : 
    lastHitRow(-1), 
    lastHitCol(-1), 
    hasLastHit(false) {
    std::random_device rd;
    rng = std::mt19937(rd());
    initializeAvailableMoves();
}

void ComputerPlayer::initializeAvailableMoves() {
    availableMoves.clear();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            availableMoves.push_back(std::make_pair(i, j));
        }
    }
}

bool ComputerPlayer::isValidCoordinate(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

void ComputerPlayer::addAdjacentSquares(int row, int col) {
    const int dx[] = {-1, 1, 0, 0};  // Up, Down, Left, Right
    const int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int newRow = row + dx[i];
        int newCol = col + dy[i];
        
        if (isValidCoordinate(newRow, newCol)) {
            // Check if the move is still available
            auto it = std::find(availableMoves.begin(), availableMoves.end(), 
                              std::make_pair(newRow, newCol));
            if (it != availableMoves.end()) {
                potentialTargets.push_back(*it);
            }
        }
    }
}

void ComputerPlayer::removeMove(const std::pair<int, int>& move) {
    auto it = std::find(availableMoves.begin(), availableMoves.end(), move);
    if (it != availableMoves.end()) {
        availableMoves.erase(it);
    }
    
    it = std::find(potentialTargets.begin(), potentialTargets.end(), move);
    if (it != potentialTargets.end()) {
        potentialTargets.erase(it);
    }
}

std::pair<int, int> ComputerPlayer::selectRandomMove() {
    if (availableMoves.empty()) {
        return std::make_pair(-1, -1);  // No moves available
    }
    
    std::uniform_int_distribution<> dis(0, availableMoves.size() - 1);
    int index = dis(rng);
    return availableMoves[index];
}

std::pair<int, int> ComputerPlayer::makeMove() {
    std::pair<int, int> move;
    
    if (!potentialTargets.empty()) {
        // Target mode: choose from potential targets
        std::uniform_int_distribution<> dis(0, potentialTargets.size() - 1);
        int index = dis(rng);
        move = potentialTargets[index];
        potentialTargets.erase(potentialTargets.begin() + index);
    } else {
        // Hunt mode: choose randomly from available moves
        move = selectRandomMove();
    }
    
    removeMove(move);
    return move;
}

void ComputerPlayer::processResult(int row, int col, bool isHit, bool isSunk) {
    if (isHit) {
        if (isSunk) {
            // Clear potential targets if ship is sunk
            potentialTargets.clear();
            hasLastHit = false;
        } else {
            // Add adjacent squares to potential targets
            addAdjacentSquares(row, col);
            lastHitRow = row;
            lastHitCol = col;
            hasLastHit = true;
        }
    }
}