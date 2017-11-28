//
// Created by hoou on 27.11.17.
//

#ifndef GUX2CPLUSPLUS_PLAYER_H
#define GUX2CPLUSPLUS_PLAYER_H

#include <string>
#include "Cell.h"

class Player {
private:
    std::string name;
    CellValue symbol;

public:
    Player(std::string name, CellValue symbol);

    // Getters

    const std::string &getName() const;

    CellValue getSymbol() const;
};


#endif //GUX2CPLUSPLUS_PLAYER_H
