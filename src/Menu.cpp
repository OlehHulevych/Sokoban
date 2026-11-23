#include "Menu.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

Menu::Menu(const std::string &listFile) {
    std::ifstream file(listFile);
    if (!file) {
        throw std::runtime_error("Cannot open level list: " + listFile);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            levels_.push_back(line);
        }
    }

    if (levels_.empty()) {
        throw std::runtime_error("No levels in list: " + listFile);
    }
}

std::size_t Menu::size() const {
    return levels_.size();
}

const std::string &Menu::path(int index) const {
    return levels_.at(static_cast<std::size_t>(index));
}

std::string Menu::name(int index) const {
    const std::string &p = path(index);
    std::size_t pos = p.find_last_of("/\\");
    return pos == std::string::npos ? p : p.substr(pos + 1);
}

int Menu::choose(const Score &scores) const {
    std::cout << "\033[2J\033[H";
    std::cout << "SOKOBAN\r\n\r\n";
    for (std::size_t i = 0; i < levels_.size(); ++i) {
        std::string levelName = name(static_cast<int>(i));
        int best = scores.best(levelName);
        std::cout << ' ' << i + 1 << ") " << levelName << "   best: ";
        if (best < 0) {
            std::cout << '-';
        } else {
            std::cout << best;
        }
        std::cout << "\r\n";
    }
    std::cout << "\r\n q) quit\r\n" << std::flush;

    char c;
    while (std::cin >> c) {
        if (c == 'q') {
            return -1;
        }
        if (c >= '1' && c <= '9') {
            int index = c - '1';
            if (static_cast<std::size_t>(index) < levels_.size()) {
                return index;
            }
        }
    }
    return -1;
}
