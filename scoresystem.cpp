//
// Created by Alex Pricop on 17-Nov-23.
//
#include "scoresystem.hpp"

#include <utility>

Player::Player(std::string  name, unsigned int linesCleared)
        : playerName(std::move(name)), clearedLines(linesCleared) {}

Player::~Player() {
    std::cout << "Player object destroyed\n";
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        playerName = other.playerName;
        clearedLines = other.clearedLines;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player Name: " << player.playerName << ", Lines Cleared: " << player.clearedLines << "\n";
    return os;
}

void Player::displayInfo() const {
    std::cout << "Player Name: " << playerName << ", Lines Cleared: " << clearedLines << "\n"; //informatie player
}

void Player::setName(const std::string& name) {
    playerName = name;
}

void Player::setLinesCleared(unsigned int linesCleared) {
    clearedLines = linesCleared;
}

std::string Player::getName() const {
    return playerName;
}

unsigned int Player::getLinesCleared() const {
    return clearedLines;
}
HighScoreSystem::HighScoreSystem() = default;

HighScoreSystem::~HighScoreSystem() {
    std::cout << "HighScoreSystem object destroyed\n";
}

void HighScoreSystem::addPlayer(const Player& player) {
    highScores.push_back(player);//sortare
}

[[maybe_unused]] void HighScoreSystem::displayHighScores() const {
    for (const auto& player : highScores) {
        player.displayInfo();//afisare
    }
}//cod nefolosit inca