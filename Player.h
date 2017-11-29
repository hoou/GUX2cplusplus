#ifndef GUX2CPLUSPLUS_PLAYER_H
#define GUX2CPLUSPLUS_PLAYER_H

#include <string>
#include "Cell.h"

class Player {
private:
    std::string name;
    CellValue symbol;
    std::string color;

public:
    Player(std::string name, CellValue symbol, const std::string &color);

    // Getters

    const std::string &getName() const;

    CellValue getSymbol() const;

    const std::string &getColor() const;

    // Setters

    void setColor(const std::string &color);
};


#endif //GUX2CPLUSPLUS_PLAYER_H
