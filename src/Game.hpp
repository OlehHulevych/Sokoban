#pragma once

#include "Level.hpp"
#include "Renderer.hpp"
#include <mutex>
#include <condition_variable>

class Game {
public:
    explicit Game(const Level &level);
    void run();
    bool tryMove(Position dir);
    const Level &getLevel() const;
    int getMoves() const;
private:
    void inputLoop();
    void computeLoop();
    void outputLoop();
    std::mutex mutex_;
    std::condition_variable cvar_;
    bool dirty_=true;
    int elapsed_ = 0;
    void restart();
    void handleCommand(char c);
    Level level_;
    Level initial_;
    Renderer renderer_;
    int moves_ = 0;
    bool running_ = true;
};
