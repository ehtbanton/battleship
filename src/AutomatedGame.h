//AutomatedGame.h
#pragma once
#ifndef AUTOMATED_GAME_H
#define AUTOMATED_GAME_H

#include "Game.h"
#include "ComputerPlayer.h"
#include <chrono>
#include <thread>

class AutomatedGame : public Game {
private:
    ComputerPlayer ai;
    static const int DELAY_MS = 1000; // 1 second delay between moves

    void handleAITurn();
    void handleGameEnd();

public:
    AutomatedGame() : Game() {}
    void run_auto();
};

#endif // AUTOMATED_GAME_H