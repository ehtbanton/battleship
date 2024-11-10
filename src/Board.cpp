//Board.cpp
#include "Board.h"
#include "Constants.h"
#include <iostream>

void Board::initBoard(int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    if (BOARD_SIZE < 4 || BOARD_SIZE > 9) {
        std::cout << "Invalid board size\n";
        return;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            boarddata[i][j] = GameConfig::EMPTY;
        }
    }
}

void Board::drawBoard(const int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    std::string boardsquare[4];
    boardsquare[0] = "         |";
    boardsquare[1] = "         |";
    boardsquare[2] = "         |";
    boardsquare[3] = "---------#";

    int slength = boardsquare[0].length();
    int sheight = 4;  // size of boardsquare array
    int rowlength = boardsquare[0].length() * (BOARD_SIZE + 1) + 1;

    std::string board = "";
    for (int line = 0; line < BOARD_SIZE + 1; line++) {
        for (int i = 0; i < sheight; i++) {
            for (int col = 0; col < BOARD_SIZE + 1; col++) {
                board.append(boardsquare[i]);
            }
            board.append("\n");
        }
    }

    // Add column numbers
    for (int col = 0; col < BOARD_SIZE; col++) {
        int editidx = ((slength / 2) - 1 + rowlength * (sheight - 2) + slength * (col + 1));
        board[editidx] = col + 49;
    }

    // Add row numbers
    for (int row = 0; row < BOARD_SIZE; row++) {
        int editidx = (slength - 4 + rowlength * (3 * (sheight / 2) - 1 + sheight * row));
        board[editidx] = row + 49;
    }

    // Add hits and misses
    int ctridx = (sheight * rowlength + slength) * 1.5 - (rowlength + 1);
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (boarddata[row][col] == GameConfig::MISS) {
                board[ctridx] = '.';
            }
            else if (boarddata[row][col] == GameConfig::HIT) {
                board[ctridx] = 'X';
            }
            ctridx += slength;
        }
        ctridx += (sheight - 1) * rowlength + slength + 1;
    }

    std::cout << board << std::endl;
}

void Board::drawTitle() {
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    std::cout << "  ______________________________________________________________________________________  \n";
    std::cout << " /     ______  _______ _______ _______        _______ _______ _     _ _____  _____      \\ \n";
    std::cout << "///    |_____] |_____|    |       |    |      |______ |______ |_____|   |   |_____]    \\\\\\\n";
    std::cout << "\\\\\\    |_____] |     |    |       |    |_____ |______ ______| |     | __|__ |          ///\n";
    std::cout << " \\______________________________________________________________________________________/ \n\n";
}