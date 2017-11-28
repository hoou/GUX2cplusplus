//
// Created by hoou on 27.11.17.
//

#include "Cell.h"

Cell::Cell(unsigned long row, unsigned long col) : row(row), col(col) {}


Cell::Cell(unsigned long row, unsigned long col, CellValue value) : row(row), col(col), value(value) {}

CellValue Cell::getValue() const {
    return value;
}

void Cell::setValue(CellValue value) {
    Cell::value = value;
}

std::string Cell::getStringValue() const {
    switch (value) {
        case X:
            return "X";
        case O:
            return "O";
        case Empty:
            return " ";
        default:
            return "";
    }
}

std::string Cell::convertValueToString(CellValue value) {
    Cell tmpCell{0, 0, value};
    return tmpCell.getStringValue();
}

unsigned long Cell::getRow() const {
    return row;
}

unsigned long Cell::getCol() const {
    return col;
}

WinningCellSequenceDirection Cell::getWinningCellSequenceDirection() const {
    return winningCellSequenceDirection;
}

void Cell::setWinningCellSequenceDirection(WinningCellSequenceDirection winningCellSequenceDirection) {
    Cell::winningCellSequenceDirection = winningCellSequenceDirection;
}
