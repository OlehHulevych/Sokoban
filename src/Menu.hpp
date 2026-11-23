#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include "Score.hpp"

class Menu {
public:
    explicit Menu(const std::string &listFile);

    int choose(const Score &scores) const;

    std::size_t size() const;
    const std::string &path(int index) const;
    std::string name(int index) const;

private:
    std::vector<std::string> levels_;
};