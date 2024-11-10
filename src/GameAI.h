//GameAI.h
#pragma once
#ifndef GAME_AI_H
#define GAME_AI_H

#include "Game.h"
#include "ComputerPlayer.h"
#include <iostream>
#include <chrono>
#include <thread>

class GameAI : public Game {
private:
    ComputerPlayer ai;
    bool playerTurn;  // true for player turn, false for AI turn

    void handlePlayerTurn();
    void handleAITurn();
    void handleGameEnd(bool playerWon);

public:
    GameAI();
    void run();
};

#endif // GAME_AI_H