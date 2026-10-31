//
// Created by Oleh on 10/29/2026.
//

#ifndef SOKOBAN_SCORE_H
#define SOKOBAN_SCORE_H
#include <map>
#include <string>


class Score {
public:
    explicit Score(const std::string &filePath);
    int best(const std::string &level);
    bool update(const std::string &level, int moves);
    void save() const;
private:
    void load();
    std::map<std::string,int> scores_;
    std::string filePath_;
};


#endif //SOKOBAN_SCORE_H
