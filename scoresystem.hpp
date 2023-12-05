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
    explicit Player(const std::string& name = "", unsigned int linesCleared = 0);
    ~Player();
    Player& operator=(const Player& other);
    void displayInfo() const;


    void setName(const std::string& name);
    void setLinesCleared(unsigned int linesCleared);


    [[nodiscard]] std::string getName() const;
    [[nodiscard]] unsigned int getLinesCleared() const;


};
class HighScoreSystem{
    std::vector<Player> highScores;
public:
    HighScoreSystem();
    ~HighScoreSystem();  // Destructor
    void addPlayer(const Player& player);
    void displayHighScores() const;

};

