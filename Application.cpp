//
// Created by hoou on 25.11.17.
//

#include "Application.h"

Application::Application(int argc, char *argv[]) {
    gui = new Gui(argc, argv);
}

Application::~Application() {
    delete gui;
//    delete gameLogic;
}

int Application::run() {
    int status = 0;

    status = gui->run();

    return status;
}
