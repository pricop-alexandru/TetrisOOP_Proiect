//
// Created by Alex Pricop on 17-Nov-23.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

class Player {
    std::string playerName;
    unsigned int clearedLines;
public:
    explicit Player(std::string  name = "", unsigned int linesCleared = 0);
    ~Player();
    Player& operator=(const Player& other);
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    void displayInfo() const;


    [[maybe_unused]] void setName(const std::string& name);
    [[maybe_unused]] void setLinesCleared(unsigned int linesCleared);


    [[maybe_unused]] [[nodiscard]] std::string getName() const;
    [[maybe_unused]] [[nodiscard]] unsigned int getLinesCleared() const;


};
class [[maybe_unused]] HighScoreSystem{
    std::vector<Player> highScores;
public:
    HighScoreSystem();
    ~HighScoreSystem();  // Destructor
    [[maybe_unused]] void addPlayer(const Player& player);
    [[maybe_unused]] void displayHighScores() const;

};

