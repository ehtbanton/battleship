//AutomatedGame.cpp
#include "AutomatedGame.h"

void AutomatedGame::run_auto() {
    const int MAX_TURNS = GameConfig::MAX_TURNS;

    for (int turn = 0; turn < MAX_TURNS; turn++) {
        if (isGameWon()) {
            handleGameEnd();
            break;
        }

        // Add delay before computer's move
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
        handleAITurn();
    }

    if (!isGameWon()) {
        handleGameEnd();  // Game ended in a timeout
    }
}

void AutomatedGame::handleAITurn() {
    int ships_before = getRemainingShips();
    std::pair<int, int> move = ai.makeMove();
    int row = move.first;
    int col = move.second;

    // Process AI's move
    int result = getBoardState(row, col);
    bool isHit = (result == GameConfig::SHIP);
    bool isSunk = false;

    if (isHit) {
        processGuess(row, col);
        isSunk = (getRemainingShips() < ships_before);
    }
    else {
        processGuess(row, col);
    }

    // Update AI's knowledge
    ai.processResult(row, col, isHit, isSunk);

    // Display AI's move
    Board::drawTitle();
    std::cout << "Computer chose: Row " << (row + 1) << ", Column " << (col + 1) << std::endl;
    Board::drawBoard(boarddata);
}

void AutomatedGame::handleGameEnd() {
    Board::drawTitle();
    if (isGameWon()) {
        GameMessages::drawMessage(WIN1);
    }
    else {
        GameMessages::drawMessage(LOSE1);
    }
    Board::drawBoard(boarddata);
    // No need to wait for input, but add a final delay
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS * 3));
}