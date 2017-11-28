//
// Created by hoou on 25.11.17.
//

#ifndef GUX2CPLUSPLUS_GAMELOGIC_H
#define GUX2CPLUSPLUS_GAMELOGIC_H


#include <vector>
#include "Cell.h"
#include "Player.h"

#define NUMBER_OF_PLAYERS 2

class GameLogic {
private:
    unsigned int gridSize;
    unsigned int numberOfCellsInRowToWin;
    std::vector<std::vector<Cell *>> cells;
    Player players[NUMBER_OF_PLAYERS]{{"Player1", X},
                                      {"Player2", O}};
    std::vector<std::string> winSymbolSequenceVector;
    unsigned short activePlayerIndex = 0;
    bool gameRunning = true;

    unsigned long firstWinningCellRow;
    unsigned long firstWinningCellCol;
    WinningCellSequenceDirection winningCellSequenceDirection;

    std::vector<Cell *> winningCells;

    void initCells();

    void swapActivePlayer();

    bool doWeHaveWinner(unsigned long row, unsigned long col);

    void stopGame();

    void setupWinningCells();

public:

    explicit GameLogic(unsigned int gridSize, unsigned int numberOfCellsInRowToWin);

    virtual ~GameLogic();

    bool updateGrid(unsigned long row, unsigned long col);

    // Getters
    bool isGameRunning() const;

    unsigned int getGridSize() const;

    Cell *getCell(unsigned long row, unsigned long col);

    Player *getActivePlayer();

    Player *getWinner();

    const std::vector<Cell *> &getWinningCells() const;
};


#endif //GUX2CPLUSPLUS_GAMELOGIC_H
