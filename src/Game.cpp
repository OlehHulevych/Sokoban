#include "Game.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

Game::Game(const Level &level) :level_(level),initial_(level){}

void Game::run() {
    std::thread in(&Game::inputLoop, this);
    std::thread comp(&Game::computeLoop, this);
    std::thread out(&Game::outputLoop, this);
    in.join();
    comp.join();
    out.join();
}

void Game::inputLoop() {
    char c;
    while (std::cin >> c) {
        std::unique_lock<std::mutex> lock(mutex_);
        handleCommand(c);
        if (level_.isSolved()) {
            running_ = false;
        }
        dirty_ = true;
        bool stop = !running_;
        lock.unlock();
        cvar_.notify_all();
        if (stop) {
            return;
        }
    }
    std::unique_lock<std::mutex> lock(mutex_);
    running_ = false;
    dirty_ = true;
    lock.unlock();
    cvar_.notify_all();
}

void Game::computeLoop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (running_) {
        if (cvar_.wait_for(lock,std::chrono::seconds(1), [this]{return !running_;})) {
            break;
        }
        ++elapsed_;
        dirty_ = true;
        cvar_.notify_all();
    }
}

void Game::outputLoop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (true) {
        cvar_.wait(lock, [this]{ return dirty_ || !running_; });
        dirty_ = false;
        std::cout << "\033[2J\033[H";
        renderer_.draw(level_);
        std::cout<< "Moves: "<< moves_<<" Time: "<< elapsed_ << "s\r\n";
        if (level_.isSolved()) {
            std::cout<< "Solved! \r\n";
        }
        std::cout<< std::flush;
        if (!running_) {
            break;
        }

    }
}

bool Game::tryMove(Position dir) {
    Position next = level_.getPlayer() + dir;

    if (level_.isWall(next)) {
        return false;
    }

    if (level_.hasBox(next)) {
        Position beyond = next + dir;
        if (level_.isWall(beyond) || level_.hasBox(beyond)) {
            return false;
        }
        level_.moveBox(next, beyond);
    }

    level_.setPlayer(next);
    ++moves_;
    return true;
}



void Game::handleCommand(char c) {
    switch (c) {
        case 'w': tryMove({-1, 0}); break;
        case 's': tryMove({1, 0});  break;
        case 'a': tryMove({0, -1}); break;
        case 'd': tryMove({0, 1});  break;
        case 'r': restart();        break;
        case 'q': running_ = false; break;
        default:                    break;
    }
}

void Game::restart() {
    level_ = initial_;
    moves_ = 0;
}

const Level &Game::getLevel() const {
    return level_;
}

int Game::getMoves() const {
    return moves_;
}
