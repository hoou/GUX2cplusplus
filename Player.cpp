//
// Created by hoou on 27.11.17.
//

#include "Player.h"

#include <utility>

Player::Player(std::string name, CellValue symbol) : name(std::move(name)), symbol(symbol) {}

// Getters

const std::string &Player::getName() const {
    return name;
}

CellValue Player::getSymbol() const {
    return symbol;
}

