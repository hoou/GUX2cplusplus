//
// Created by hoou on 25.11.17.
//

#ifndef GUX2CPLUSPLUS_APPLICATION_H
#define GUX2CPLUSPLUS_APPLICATION_H


#include "Gui.h"
#include "GameLogic.h"

class Application {
private:
    Gui *gui;
    GameLogic *gameLogic;

public:
    Application(int argc, char *argv[], unsigned int gridSize, unsigned int numberOfCellsInRowToWin);

    ~Application();

    int run();
};


#endif //GUX2CPLUSPLUS_APPLICATION_H
