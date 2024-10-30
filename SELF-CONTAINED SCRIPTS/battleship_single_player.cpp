#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <chrono>

using std::cout, std::cin, std::string, std::endl;
using namespace std::this_thread;
using namespace std::chrono;

const int BOARD_SIZE = 8;
enum MessageType { ERROR, ROW, COLUMN, HIT, MISS, SINK, WIN1, WIN2, LOSE1, LOSE2, RETRY };

class Ship {
public:
    int squares[5][3]; // Allows for ships up to 5 squares long
    // squares[][0]=x, squares[][1]=y, squares[][2]="not-yet-hit"

    Ship(int size, int boarddata[BOARD_SIZE][BOARD_SIZE]) {
        if (size < 1 || size > 5) {
            cout << "Invalid ship size\n";
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
                    if (boarddata[x + i][y] != 0) {
                        valid = false;
                        break;
                    }
                }
            }
            else { // Vertical
                if (y + size > BOARD_SIZE) continue;
                for (int i = 0; i < size; i++) {
                    if (boarddata[x][y + i] != 0) {
                        valid = false;
                        break;
                    }
                }
            }
        } while (!valid);

        for (int i = 0; i < 5; i++) {
            if (i < size) {
                squares[i][0] = (orientation == 0) ? x + i : x;
                squares[i][1] = (orientation == 0) ? y : y + i;
                squares[i][2] = 1;
                boarddata[squares[i][0]][squares[i][1]] = 1;
            }
            else {
                squares[i][0] = -1;
                squares[i][1] = -1;
                squares[i][2] = 0;
            }
        }
    }

    Ship() {
        for (int i = 0; i < 5; i++) {
            squares[i][0] = -1;
            squares[i][1] = -1;
            squares[i][2] = 0;
        }
    }

    bool isSunk() const {
        for (int i = 0; i < 5; i++) {
            if (squares[i][2] == 1) {
                return false;
            }
        }
        return true;
    }
};

void initBoard(int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    if (BOARD_SIZE < 4 || BOARD_SIZE>9) {
        cout << "Invalid board size\n";
        return;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            boarddata[i][j] = 0;
        }
    }
}

void drawBoard(const int boarddata[BOARD_SIZE][BOARD_SIZE]) {

    string boardsquare[4];
    boardsquare[0] = "         |";
    boardsquare[1] = "         |";
    boardsquare[2] = "         |";
    boardsquare[3] = "---------#";

    int slength = boardsquare[0].length();
    int sheight = end(boardsquare) - begin(boardsquare);
    int rowlength = boardsquare[0].length() * (BOARD_SIZE + 1) + 1;

    string board = "";
    for (int line = 0; line < BOARD_SIZE + 1; line++) {
        for (int i = 0; i < sheight; i++) {
            for (int col = 0; col < BOARD_SIZE + 1; col++) {
                board.append(boardsquare[i]);
            }
            board.append("\n");
        }
    }
    for (int col = 0; col < BOARD_SIZE; col++) {
        int editidx = ((slength / 2) - 1 + rowlength * (sheight - 2) + slength * (col + 1));
        board[editidx] = col + 49;
    }
    for (int row = 0; row < BOARD_SIZE; row++) {
        int editidx = (slength - 4 + rowlength * (3 * (sheight / 2) - 1 + sheight * row));
        board[editidx] = row + 49; // change to row+65 for A,B,C...
    }


    // Hits and misses marked on the board:
    int ctridx = (sheight * rowlength + slength) * 1.5 - (rowlength + 1);
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (boarddata[row][col] == 2) {
                board[ctridx] = '.'; // Miss
            }
            else if (boarddata[row][col] == 3) {
                board[ctridx] = 'X'; // Hit
            }
            ctridx += slength;
        }
        ctridx += (sheight - 1) * rowlength + slength + 1;
    }

    cout << board << endl;

    /*
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << i << " ";
            for (int j = 0; j < BOARD_SIZE; j++) {
                switch(boarddata[i][j]) {
                    case 0: cout << ". "; break; // Empty
                    case 1: cout << "S "; break; // Ship
                    case 2: cout << "X "; break; // Hit
                    default: cout << "? "; // Error
                }
            }
            cout << endl;
        }
    */

}

void drawTitle() {
    string title[6] = { "","","","","","" };
    title[0] = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    title[1] = "  ______________________________________________________________________________________  ";
    title[2] = " /     ______  _______ _______ _______        _______ _______ _     _ _____  _____      \\ ";
    title[3] = "///    |_____] |_____|    |       |    |      |______ |______ |_____|   |   |_____]    \\\\\\";
    title[4] = "\\\\\\    |_____] |     |    |       |    |_____ |______ ______| |     | __|__ |          ///";
    title[5] = " \\______________________________________________________________________________________/ ";
    cout << title[0] << "\n" << title[1] << "\n" << title[2] << "\n" << title[3] << "\n" << title[4] << "\n" << title[5] << "\n\n";
}

void drawMessage(MessageType message) {
    string emptyheader = "\n\n\n\n\n";
    string startheader = "\n                ___  ____ ____ ____ ____    ____ _  _ ___ ____ ____\n                |__] |__/ |___ [__  [__     |___ |\\ |  |  |___ |__/\n                |    |  \\ |___ ___] ___]    |___ | \\|  |  |___ |  \\ \n\n";
    string rowheader = "\n      ____ ____ _    ____ ____ ___    ____    ____ ____ _ _ _     _      __  \n      [__  |___ |    |___ |     |     |__|    |__/ |  | | | |    /|  __ |__| \n      ___] |___ |___ |___ |___  |     |  |    |  \\ |__| |_|_|    _|_    |__|\n\n";
    string colheader = "\n      ____ _  _ ___     ____    ____ ____ _    _  _ _  _ _  _     _      __  \n      |__| |\\ | |  \\    |__|    |    |  | |    |  | |\\/| |\\ |    /|  __ |__|  \n      |  | | \\| |__/    |  |    |___ |__| |___ |__| |  | | \\|    _|_    |__|\n\n";
    string hitheader = "\n                       _ ___ . ____    ____    _  _ _ ___\n                       |  |  ' [__     |__|    |__| |  |  \n                       |  |    ___]    |  |    |  | |  |  \n\n";
    string missheader = "\n                 _   _ ____ _  _    _  _ _ ____ ____ ____ ___  \n                  \\_/  |  | |  |    |\\/| | [__  [__  |___ |  \\ \n                   |   |__| |__|    |  | | ___] ___] |___ |__/ \n\n";
    string sinkheader = "\n      _   _ ____ _  _    ____ _  _ _  _ _  _    ____    ____ _  _ _ ___  \n       \\_/  |  | |  |    [__  |  | |\\ | |_/     |__|    [__  |__| | |__] \n        |   |__| |__|    ___] |__| | \\| | \\_    |  |    ___] |  | | |    \n\n";
    string winheader1 = "\n      ____ _    _       ____ _  _ _ ___  ____    ____ ____ _  _ _  _ ___  \n      |__| |    |       [__  |__| | |__] [__     |___ |  | |  | |\\ | |  \\ \n      |  | |___ |___    ___] |  | | |    ___]    |    |__| |__| | \\| |__/ \n\n";
    string winheader2 = "\n                         _   _ ____ _  _    _ _ _ _ _  _ \n                         \\_/  |  | |  |    | | | | |\\ | \n                          |   |__| |__|    |_|_| | | \\| \n\n";
    string loseheader1 = "\n      ____ ____ _  _    ____ _  _ ___    ____ ____    ___ _  _ ____ _  _ ____ \n      |__/ |__| |\\ |    |  | |  |  |     |  | |___     |  |  | |__/ |\\ | [__  \n      |  \\ |  | | \\|    |__| |__|  |     |__| |        |  |__| |  \\ | \\| ___] \n\n";
    string loseheader2 = "\n                     _   _ ____ _  _    _    ____ ____ ____ \n                      \\_/  |  | |  |    |    |  | [__  |___ \n                       |   |__| |__|    |___ |__| ___] |___ \n\n";

    switch (message) {
    case ERROR: cout << "\n\nInvalid input.\nPlease try again.\n\n"; break;
    case RETRY: cout << "\n\nYou already guessed that square.\nPlease try again.\n\n"; break;
    case ROW:       cout << rowheader; break;
    case COLUMN:    cout << colheader; break;
    case HIT:       cout << hitheader; break;
    case MISS:      cout << missheader; break;
    case SINK:      cout << sinkheader; break;
    case WIN1:      cout << winheader1; break;
    case WIN2:      cout << winheader2; break;
    case LOSE1:     cout << loseheader1; break;
    }
}

int main() {

    system("cls");
    int boarddata[BOARD_SIZE][BOARD_SIZE];
    initBoard(boarddata);

    const int num_ship4 = 1;
    const int num_ship3 = 2;
    const int num_ship2 = 2;
    int num_ships = num_ship4 + num_ship3 + num_ship2;

    Ship ships[num_ships];

    int ship_index = 0;
    for (int i = 0; i < num_ship4; i++) ships[ship_index++] = Ship(4, boarddata);
    for (int i = 0; i < num_ship3; i++) ships[ship_index++] = Ship(3, boarddata);
    for (int i = 0; i < num_ship2; i++) ships[ship_index++] = Ship(2, boarddata);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int row, col;
    bool won_game = 0;


    for (int turn = 0; turn < 40; turn++) {


        if (won_game) {
            drawTitle();
            drawMessage(WIN1);
            drawBoard(boarddata);
            cin.ignore();
            cin.get();
            drawTitle();
            drawMessage(WIN2);
            drawBoard(boarddata);
            break;
        }


        drawTitle();
        drawMessage(ROW);
        drawBoard(boarddata);
        do {
            cin >> row;
        } while (row < 1 || row > BOARD_SIZE);

        drawTitle();
        drawMessage(COLUMN);
        drawBoard(boarddata);
        do {
            cin >> col;
        } while (col < 1 || col > BOARD_SIZE);

        cin.ignore();
        switch (boarddata[row - 1][col - 1]) {
        case 0: // Empty
            boarddata[row - 1][col - 1] = 2;
            drawTitle();
            drawMessage(MISS);
            drawBoard(boarddata);
            break;
        case 1: // Ship
            boarddata[row - 1][col - 1] = 3;
            drawTitle();
            drawMessage(HIT);
            drawBoard(boarddata);
            for (int i = 0; i < num_ship4 + num_ship3 + num_ship2; i++) {
                for (int j = 0; j < 5; j++) {
                    if (ships[i].squares[j][0] == row - 1 && ships[i].squares[j][1] == col - 1) {
                        ships[i].squares[j][2] = 0;
                        if (ships[i].isSunk()) {
                            cin.get();
                            drawTitle();
                            drawMessage(SINK);
                            drawBoard(boarddata);

                            won_game = 1; //temporary
                            for (int ship = 0; ship < num_ships; ship++) {
                                if (!ships[ship].isSunk()) {
                                    won_game = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2: // Already guessed
            drawTitle();
            drawBoard(boarddata);
            drawMessage(RETRY);
            turn--;
            break;
        default: // Error
            drawTitle();
            drawBoard(boarddata);
            drawMessage(ERROR);
            turn--;
            break;
        }
        cin.get();
    }


    if (!won_game) {
        drawTitle();
        drawMessage(LOSE1);
        drawBoard(boarddata);
        cin.ignore();
        cin.get();
        drawTitle();
        drawMessage(LOSE2);
        drawBoard(boarddata);
    }



    return 0;
}