#ifndef GUX2CPLUSPLUS_APPLICATION_H
#define GUX2CPLUSPLUS_APPLICATION_H


#include "Gui.h"
#include "GameLogic.h"

class Application {
private:
    Gui *gui;

public:
    Application(int argc, char *argv[]);

    ~Application();

    int run();
};


#endif //GUX2CPLUSPLUS_APPLICATION_H
