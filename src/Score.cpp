

#include "Score.hpp"
#include <fstream>

Score::Score(const std::string &filePath): filePath_(filePath) {
   load();
}

int Score::best(const std::string &level)  {
    auto it = scores_.find(level);
    if (it == scores_.end()) {
        return -1;
    }
    return it->second;

}

void Score::load() {
    std::string name;
    std::ifstream file(filePath_);
    int moves;
    while (file>>name >> moves) {
        scores_[name] = moves;
    }
}

void Score::save() const {
    std::ofstream file(filePath_);
    if (!file) {
        return;
    }
    for (const auto &entry:scores_) {
        file << entry.first << ' '<<entry.second << '\n';
    }
}

bool Score::update(const std::string &level, int moves) {
    auto it = scores_.find(level);
    if (it == scores_.end() || moves < it->second) {
        scores_[level] = moves;
        return true;
    }
    return false;
}


