//
// Created by hoou on 25.11.17.
//

#include "Application.h"

Application::Application(int argc, char *argv[], unsigned int gridSize, unsigned int numberOfCellsInRowToWin) {
    gameLogic = new GameLogic(gridSize, numberOfCellsInRowToWin);
    gui = new Gui(argc, argv, gameLogic);
}

Application::~Application() {
    delete gui;
    delete gameLogic;
}

int Application::run() {
    int status = 0;

    status = gui->run();

    return status;
}
