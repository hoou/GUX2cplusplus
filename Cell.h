#ifndef GUX2CPLUSPLUS_CELL_H
#define GUX2CPLUSPLUS_CELL_H

#include <string>

#define DEFAULT_CELL_COLOR "black"

typedef enum {
    Empty,
    O,
    X
} CellValue;

typedef enum {
    Horizontal,
    Vertical,
    Diagonal,
    OppositeDiagonal
} WinningCellSequenceDirection;

class Cell {
private:
    unsigned long row;
    unsigned long col;
    std::string color = DEFAULT_CELL_COLOR;
    CellValue value = Empty;

public:
    Cell(unsigned long row, unsigned long col);

    Cell(unsigned long row, unsigned long col, CellValue value);

    CellValue getValue() const;

    std::string getStringValue() const;

    static std::string convertValueToString(CellValue value);

    void setValue(CellValue value);

    unsigned long getRow() const;

    unsigned long getCol() const;

    const std::string &getColor() const;

    void setColor(const std::string &color);
};


#endif //GUX2CPLUSPLUS_CELL_H
