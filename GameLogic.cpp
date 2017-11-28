//

#include "GameLogic.h"
#include <iostream>

GameLogic::GameLogic() {
    initCells();
    initWinSymbolSequenceVector();
}

GameLogic::GameLogic(unsigned int gridSize, unsigned int numberOfCellsInRowToWin) : gridSize(gridSize),
                                                                                    numberOfCellsInRowToWin(
                                                                                            numberOfCellsInRowToWin) {
    initCells();
    initWinSymbolSequenceVector();
}

GameLogic::GameLogic(unsigned int gridSize, unsigned int numberOfCellsInRowToWin, std::string player1color,
                     std::string player2color) : gridSize(gridSize),
                                                 numberOfCellsInRowToWin(
                                                         numberOfCellsInRowToWin){

    players[0].setColor(player1color);
    players[1].setColor(player2color);

    initCells();
    initWinSymbolSequenceVector();
}

GameLogic::~GameLogic() {
    for (auto row = cells.begin(); row < cells.end(); row++) {
        for (auto col = (*row).begin(); col < (*row).end(); col++) {
            delete *col;
        }
    }
}

void GameLogic::initCells() {
    for (unsigned long row = 0; row < gridSize; row++) {
        std::vector<Cell *> rowVector;
        rowVector.reserve(gridSize);
        for (unsigned long col = 0; col < gridSize; col++) {
            rowVector.push_back(new Cell(row, col));
        }
        cells.push_back(rowVector);
    }
}

void GameLogic::initWinSymbolSequenceVector() {
    for (const auto &player : players) {
        std::__cxx11::string winSymbolSequence;
        for (int i = 0; i < numberOfCellsInRowToWin; i++) {
            winSymbolSequence.append(Cell::convertValueToString(player.getSymbol()));
        }

        winSymbolSequenceVector.push_back(winSymbolSequence);
    }
}

bool GameLogic::updateGrid(unsigned long row, unsigned long col) {
    if (!isGameRunning())
        return false;

    if (cells.at(row).at(col)->getValue() != Empty)
        return false;

    cells.at(row).at(col)->setValue(getActivePlayer()->getSymbol());

    if (doWeHaveWinner(row, col)) {
        setupWinningCells();
        stopGame();
    } else
        swapActivePlayer();

    return true;
}

void GameLogic::swapActivePlayer() {
    switch (activePlayerIndex) {
        case 0:
            activePlayerIndex = 1;
            break;
        case 1:
            activePlayerIndex = 0;
            break;
        default:
            break;
    }
}

bool GameLogic::doWeHaveWinner(unsigned long row, unsigned long col) {
    std::string cellValues;

    // Check row
    unsigned int offsetBegin = (unsigned int) std::max(0, int(col - numberOfCellsInRowToWin + 1));
    unsigned int offsetEnd = gridSize - 1 -
                             (unsigned int) std::min((unsigned int) col + numberOfCellsInRowToWin - 1,
                                                     gridSize - 1);

    for (auto it = cells.at(row).begin() + offsetBegin; it < cells.at(row).end() - offsetEnd; it++) {
        cellValues.append((*it)->getStringValue());
    }

    std::size_t found = cellValues.find(winSymbolSequenceVector.at(activePlayerIndex));
    if (found != std::string::npos) {
        firstWinningCellRow = row;
        firstWinningCellCol = found + offsetBegin;
        winningCellSequenceDirection = Horizontal;
        return true;
    }

    cellValues = "";

    // Check column
    offsetBegin = (unsigned int) std::max(0, int(row - numberOfCellsInRowToWin + 1));
    offsetEnd = gridSize - 1 -
                (unsigned int) std::min((unsigned int) row + numberOfCellsInRowToWin - 1, gridSize - 1);


    for (auto it = cells.begin() + offsetBegin; it < cells.end() - offsetEnd; it++) {
        cellValues.append((*it).at(col)->getStringValue());
    }

    found = cellValues.find(winSymbolSequenceVector.at(activePlayerIndex));
    if (found != std::string::npos) {
        firstWinningCellRow = found + offsetBegin;
        firstWinningCellCol = col;
        winningCellSequenceDirection = Vertical;
        return true;
    }

    cellValues = "";

    // Check diagonal

    // Say row index is smaller
    long smallerIndex = row;

    if (col < smallerIndex)
        smallerIndex = col;

    if (smallerIndex > 4)
        smallerIndex = 4;

    for (long i = -smallerIndex; i < (-smallerIndex + smallerIndex + numberOfCellsInRowToWin); i++) {
        if (row + i >= gridSize || col + i >= gridSize)
            break;

        cellValues.append(cells.at(row + i).at(col + i)->getStringValue());
    }

    found = cellValues.find(winSymbolSequenceVector.at(activePlayerIndex));
    if (found != std::string::npos) {
        firstWinningCellRow = row - smallerIndex + found;
        firstWinningCellCol = col - smallerIndex + found;
        winningCellSequenceDirection = Diagonal;
        return true;
    }

    cellValues = "";

    // Check opposite diagonal

    smallerIndex = row;
    long colFromEnd = gridSize - col - 1;

    if (colFromEnd < smallerIndex)
        smallerIndex = colFromEnd;

    if (smallerIndex > 4)
        smallerIndex = 4;

    for (long i = -smallerIndex; i < (-smallerIndex + smallerIndex + numberOfCellsInRowToWin); i++) {
        if (row + i >= gridSize || col - i >= gridSize)
            break;

        cellValues.append(cells.at(row + i).at(col - i)->getStringValue());
    }

    found = cellValues.find(winSymbolSequenceVector.at(activePlayerIndex));
    if (found != std::string::npos) {
        firstWinningCellRow = row - smallerIndex + found;
        firstWinningCellCol = col + smallerIndex - found;
        winningCellSequenceDirection = OppositeDiagonal;
        return true;
    }

    return false;
}

void GameLogic::stopGame() {
    gameRunning = false;
}

void GameLogic::setupWinningCells() {
    int *rowOffset = nullptr, *colOffset = nullptr;
    int zero = 0;
    int i;
    int negativei;

    switch (winningCellSequenceDirection) {
        case Horizontal:
            rowOffset = &zero;
            colOffset = &i;
            break;
        case Vertical:
            rowOffset = &i;
            colOffset = &zero;
            break;
        case Diagonal:
            rowOffset = &i;
            colOffset = &i;
            break;
        case OppositeDiagonal:
            rowOffset = &i;
            colOffset = &negativei;
            break;
        default:
            return;
    }

    for (i = 0; i < numberOfCellsInRowToWin; i++) {
        negativei = -i;
        Cell *cell = cells.at(firstWinningCellRow + *rowOffset).at(firstWinningCellCol + *colOffset);
        cell->setWinningCellSequenceDirection(winningCellSequenceDirection);
        winningCells.push_back(cell);
    }
}

// Getters

unsigned int GameLogic::getGridSize() const {
    return gridSize;
}

Cell *GameLogic::getCell(unsigned long row, unsigned long col) {
    return cells.at(row).at(col);
}

Player *GameLogic::getActivePlayer() {
    return &players[activePlayerIndex];
}

bool GameLogic::isGameRunning() const {
    return gameRunning;
}

Player *GameLogic::getWinner() {
    if (isGameRunning())
        return nullptr;
    else
        return getActivePlayer();
}

const std::vector<Cell *> &GameLogic::getWinningCells() const {
        return winningCells;
    }
