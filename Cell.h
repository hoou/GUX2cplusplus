#ifndef GUX2CPLUSPLUS_CELL_H
#define GUX2CPLUSPLUS_CELL_H

#include <string>

typedef enum {
    Empty,
    O,
    X
} CellValue;

typedef enum {
    Horizontal,
    Vertical,
    Diagonal,
    OppositeDiagonal,
    Unknown
} WinningCellSequenceDirection;

class Cell {
private:
    unsigned long row;
    unsigned long col;
    CellValue value = Empty;
    WinningCellSequenceDirection winningCellSequenceDirection = Unknown;

public:
    Cell(unsigned long row, unsigned long col);

    Cell(unsigned long row, unsigned long col, CellValue value);

    CellValue getValue() const;

    std::string getStringValue() const;

    static std::string convertValueToString(CellValue value);

    void setValue(CellValue value);

    unsigned long getRow() const;

    unsigned long getCol() const;

    WinningCellSequenceDirection getWinningCellSequenceDirection() const;

    void setWinningCellSequenceDirection(WinningCellSequenceDirection winningCellSequenceDirection);
};


#endif //GUX2CPLUSPLUS_CELL_H
