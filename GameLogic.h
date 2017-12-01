#ifndef GUX2CPLUSPLUS_GAMELOGIC_H
#define GUX2CPLUSPLUS_GAMELOGIC_H


#include <vector>
#include "Cell.h"
#include "Player.h"

#define NUMBER_OF_PLAYERS 2
#define MIN_GRID_SIZE 3
#define MAX_GRID_SIZE 24
#define DEFAULT_GRID_SIZE 9
#define DEFAULT_NUMBER_OF_CELLS_IN_ROW_TO_WIN 3
#define PLAYER1_DEFAULT_COLOR "red"
#define PLAYER2_DEFAULT_COLOR "blue"
#define PLAYER1_DEFAULT_NAME "Player 1"
#define PLAYER2_DEFAULT_NAME "Player 2"

class GameLogic {
private:
    unsigned int gridSize = DEFAULT_GRID_SIZE;
    unsigned int numberOfCellsInRowToWin = DEFAULT_NUMBER_OF_CELLS_IN_ROW_TO_WIN;
    std::vector<std::vector<Cell *>> cells;
    Player players[NUMBER_OF_PLAYERS]{{PLAYER1_DEFAULT_NAME, O, PLAYER1_DEFAULT_COLOR},
                                      {PLAYER2_DEFAULT_NAME, X, PLAYER2_DEFAULT_COLOR}};
    std::vector<std::string> winSymbolSequenceVector;
    unsigned short activePlayerIndex = 0;
    bool gameRunning = true;
    bool tieGame = false;
    unsigned long numberOfMoves = 0;
    Player *winner = nullptr;

    unsigned long firstWinningCellRow;
    unsigned long firstWinningCellCol;
    WinningCellSequenceDirection winningCellSequenceDirection;

    std::vector<Cell *> winningCells;

    void initCells();

    void initWinSymbolSequenceVector();

    void swapActivePlayer();

    bool doWeHaveWinner(unsigned long row, unsigned long col);

    void stopGame();

    void setupWinningCells();

public:

    GameLogic();

    GameLogic(unsigned int gridSize, unsigned int numberOfCellsInRowToWin);

    GameLogic(unsigned int gridSize, unsigned int numberOfCellsInRowToWin,
              std::string player1color, std::string player2color);

    virtual ~GameLogic();

    bool updateGrid(unsigned long row, unsigned long col);

    // Getters
    bool isGameRunning() const;

    unsigned int getGridSize() const;

    Cell *getCell(unsigned long row, unsigned long col);

    Player *getActivePlayer();

    Player *getWinner() const;

    const std::vector<Cell *> &getWinningCells() const;

    WinningCellSequenceDirection getWinningCellSequenceDirection() const;

    bool isCellWinning(Cell *cell);

    bool isTieGame() const;
};


#endif //GUX2CPLUSPLUS_GAMELOGIC_H
