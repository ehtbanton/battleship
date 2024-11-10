//GameAI.cpp
#include "GameAI.h"
#include <chrono>
#include <thread>

GameAI::GameAI() : Game(), playerTurn(true) {}

void GameAI::run() {
    const int MAX_TURNS = GameConfig::MAX_TURNS * 2;  // Double turns for both players
    const int DELAY_MS = 1000; // 1 second delay

    for (int turn = 0; turn < MAX_TURNS; turn++) {
        if (isGameWon()) {
            handleGameEnd(playerTurn);
            break;
        }

        // Add delay before computer's move
        if (!playerTurn) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
        }

        if (playerTurn) {
            handlePlayerTurn();
        }
        else {
            handleAITurn();
        }
        playerTurn = !playerTurn;  // Switch turns
    }

    if (!isGameWon()) {
        handleGameEnd(false);  // Game ended in a draw or timeout
    }
}

void GameAI::handlePlayerTurn() {
    // Existing player turn logic from Game class
    int row, col;
    // Get row input
    Board::drawTitle();
    GameMessages::drawMessage(ROW);
    Board::drawBoard(boarddata);
    do {
        std::cin >> row;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            row = 0;
        }
    } while (row < 1 || row > BOARD_SIZE);
    // Get column input
    Board::drawTitle();
    GameMessages::drawMessage(COLUMN);
    Board::drawBoard(boarddata);
    do {
        std::cin >> col;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            col = 0;
        }
    } while (col < 1 || col > BOARD_SIZE);
    processGuess(row - 1, col - 1);
}

void GameAI::handleAITurn() {
    int ships_before = getRemainingShips();
    std::pair<int, int> move = ai.makeMove();
    int row = move.first;
    int col = move.second;

    // Process AI's move
    int result = getBoardState(row, col);
    bool isHit = (result == GameConfig::SHIP);
    bool isSunk = false;

    if (isHit) {
        // Check if the hit resulted in a sink
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

    // Don't wait for user input anymore
    // std::cin.get(); // Removed this line
}

void GameAI::handleGameEnd(bool playerWon) {
    Board::drawTitle();
    if (playerWon) {
        GameMessages::drawMessage(WIN1);
    }
    else {
        GameMessages::drawMessage(LOSE1);
    }
    Board::drawBoard(boarddata);
    std::cin.get(); // Keep this to pause at the end of the game
}