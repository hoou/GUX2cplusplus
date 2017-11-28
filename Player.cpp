//
// Created by hoou on 27.11.17.
//

#include "Player.h"

#include <utility>

Player::Player(std::string name, CellValue symbol, const std::string &color) : name(std::move(name)),
                                                                               symbol(symbol),
                                                                               color(color) {}

// Getters

const std::string &Player::getName() const {
    return name;
}

CellValue Player::getSymbol() const {
    return symbol;
}

const std::string &Player::getColor() const {
    return color;
}

void Player::setColor(const std::string &color) {
    Player::color = color;
}

