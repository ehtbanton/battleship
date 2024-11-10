#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_map>
#include <ctime>
#include <numeric>

using std::cout;
using std::cin;
using std::string;
using std::endl;

const int BOARD_SIZE = 8;

void initBoard(int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            boarddata[i][j] = 0;
        }
    }
}

void drawBoard(const int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            switch (boarddata[i][j]) {
            case 0: std::cout << ". "; break; // Empty
            case 1: std::cout << "S "; break; // Ship
            case 2: std::cout << "O "; break; // Miss
            case 3: std::cout << "X "; break; // Hit
            default: std::cout << "? "; // Error
            }
        }
        std::cout << std::endl;
    }
}

std::string boardToString(const int boarddata[BOARD_SIZE][BOARD_SIZE]) {
    std::string state;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            state += std::to_string(boarddata[i][j]);
        }
    }
    return state;
}

class Ship {
public:
    int squares[5][3]; // Allows for ships up to 5 squares long
    // squares[][0]=x, squares[][1]=y, squares[][2]="not-yet-hit"

    Ship(int size, int boarddata[BOARD_SIZE][BOARD_SIZE]) {
        if (size < 1 || size > 5) {
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

class BattleshipDQLAgent {
public:
    BattleshipDQLAgent(int board_size = 8, int num_ships = 5, double discount_factor = 0.99, double learning_rate = 0.2, double epsilon = 1, double epsilon_decay = 0.9999, double min_epsilon = 0.1)
        : board_size(board_size), num_ships(num_ships), discount_factor(discount_factor), learning_rate(learning_rate), epsilon(epsilon), epsilon_decay(epsilon_decay), min_epsilon(min_epsilon), gen(std::random_device{}()), dis(0.0, 1.0) {
        //loadQTable();
    }

    int getAction(const std::string& state) {
        if (dis(gen) < epsilon) {
            return std::uniform_int_distribution<>{0, board_size* board_size - 1}(gen);
        }
        else {
            if (q_table.find(state) == q_table.end()) {
                q_table[state] = std::vector<double>(board_size * board_size, 0.0);
            }
            return std::distance(q_table[state].begin(), std::max_element(q_table[state].begin(), q_table[state].end()));
        }
    }

    void updateQTable(const std::string& state, int action, double reward, const std::string& next_state) {
        if (q_table.find(state) == q_table.end()) {
            q_table[state] = std::vector<double>(board_size * board_size, 0.0);
        }
        if (q_table.find(next_state) == q_table.end()) {
            q_table[next_state] = std::vector<double>(board_size * board_size, 0.0);
        }
        double current_q = q_table[state][action];
        double max_future_q = *std::max_element(q_table[next_state].begin(), q_table[next_state].end());
        double new_q = (1 - learning_rate) * current_q + learning_rate * (reward + discount_factor * max_future_q);
        q_table[state][action] = new_q;
    }

    void train(int num_episodes, const std::vector<int>& ship_sizes) {
        int turns[1000];
        for (int episode = 0; episode < num_episodes; episode++) {
            int boarddata[BOARD_SIZE][BOARD_SIZE] = { 0 };
            std::vector<Ship> ships = placeShips(ship_sizes, boarddata);
            std::string state = boardToString(boarddata);
            int turn = 0;
            while (turn < 64) {
                int action = getAction(state);
                double reward;
                std::string next_state;
                bool done;
                std::tie(reward, next_state, done) = getRewardAndNextState(turn, state, action, ships, boarddata);
                updateQTable(state, action, reward, next_state);
                state = next_state;

                if (done) break;
                turn++;
            }
            turns[episode % 1000] = turn + 1;
            epsilon *= epsilon_decay;
            epsilon = std::max(epsilon, min_epsilon);
            if (episode % 1000 == 0) {
                //saveQTable();
                int turn_average = std::accumulate(turns, turns + 1000, 0) / 1000;
                std::cout << "\nEpisode " << episode << ". Average turns: " << turn_average << "\nEpsilon: " << epsilon << std::endl;
                drawBoard(boarddata);
            }
        }
    }

    std::vector<Ship> placeShips(const std::vector<int>& ship_sizes, int boarddata[BOARD_SIZE][BOARD_SIZE]) {
        std::vector<Ship> ships;
        for (int size : ship_sizes) {
            ships.emplace_back(size, boarddata);
        }
        return ships;
    }

    std::tuple<double, std::string, bool> getRewardAndNextState(int& turn, const std::string& state, int action, std::vector<Ship>& ships, int boarddata[BOARD_SIZE][BOARD_SIZE]) {
        int row = action / BOARD_SIZE;
        int col = action % BOARD_SIZE;
        double reward = 0;
        bool hit = false;
        bool done = false;

        switch (boarddata[row][col]) {
        case 0: // Empty
            boarddata[row][col] = 2;
            reward = -1;
            break;
        case 1: // Ship
            boarddata[row][col] = 3;
            reward = 1;
            hit = true;
            for (auto& ship : ships) {
                for (int j = 0; j < 5; j++) {
                    if (ship.squares[j][0] == row && ship.squares[j][1] == col) {
                        ship.squares[j][2] = 0;
                        if (ship.isSunk()) {
                            reward = 2;
                        }
                    }
                }
            }
            break;
        case 2: // Already guessed (miss)
            reward = -1;
            turn--;
            break;
        case 3: // Already guessed (hit)
            reward = -1;
            turn--;
            break;
        }

        done = std::all_of(ships.begin(), ships.end(), [](const Ship& ship) { return ship.isSunk(); });
        std::string next_state = boardToString(boarddata);
        return std::make_tuple(reward, next_state, done);
    }

private:
    int board_size;
    int num_ships;
    double discount_factor;
    double learning_rate;
    double epsilon;
    double epsilon_decay;
    double min_epsilon;
    std::unordered_map<std::string, std::vector<double>> q_table;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    void loadQTable() {
        std::ifstream file("q_table.txt");
        if (file.is_open()) {
            std::string state;
            std::string line;
            while (std::getline(file, state)) {
                if (std::getline(file, line)) {
                    std::istringstream iss(line);
                    q_table[state] = std::vector<double>(board_size * board_size);
                    for (int i = 0; i < board_size * board_size; i++) {
                        if (!(iss >> q_table[state][i])) {
                            // Handle error: not enough values in the line
                            break;
                        }
                    }
                }
            }
            file.close();
        }
        else {
            std::cout << "No existing Q-table found. Starting with an empty table." << std::endl;
        }
    }

    void saveQTable() {
        std::ofstream file("q_table.txt");
        for (const auto& entry : q_table) {
            file << entry.first << "\n";
            for (const auto& value : entry.second) {
                file << value << " ";
            }
            file << "\n";
        }
        file.close();
    }
};

int main() {
    system("cls");
    cout << "Starting..." << endl;

    const int num_ship4 = 1;
    const int num_ship3 = 2;
    const int num_ship2 = 2;
    std::vector<int> ship_sizes(num_ship4, 4);
    ship_sizes.insert(ship_sizes.end(), num_ship3, 3);
    ship_sizes.insert(ship_sizes.end(), num_ship2, 2);
    int num_ships = ship_sizes.size();

    cout << "Creating agent..." << endl;
    BattleshipDQLAgent agent(BOARD_SIZE, num_ships);

    cout << "Training the agent..." << endl;
    agent.train(30000, ship_sizes);

    cout << "\nTraining complete. Press Enter to start the game...";
    cin.ignore();
    cin.get();

    // Initialize game board and ships
    int boarddata[BOARD_SIZE][BOARD_SIZE];
    initBoard(boarddata);
    std::vector<Ship> game_ships = agent.placeShips(ship_sizes, boarddata);

    std::string state = boardToString(boarddata);
    int turn = 0;
    const int max_turns = 40;

    cout << "Starting the game!" << endl;
    while (turn < max_turns) {
        int action = agent.getAction(state);
        int row = action / BOARD_SIZE;
        int col = action % BOARD_SIZE;

        double reward;
        std::string next_state;
        bool done;
        std::tie(reward, next_state, done) = agent.getRewardAndNextState(turn, state, action, game_ships, boarddata);
        state = next_state;

        turn++;

        // Display game state
        cout << "Turn: " << turn << endl;
        drawBoard(boarddata);

        if (done) {
            cout << "The agent won in " << turn << " turns!" << endl;
            break;
        }

        cout << "Press Enter to continue...";
        cin.get();
    }

    if (turn == max_turns) {
        cout << "The agent couldn't sink all ships within " << max_turns << " turns." << endl;
    }

    drawBoard(boarddata);

    return 0;
}