//
// Created by hoou on 25.11.17.
//

#ifndef GUX2CPLUSPLUS_GUI_H
#define GUX2CPLUSPLUS_GUI_H

#include <gtk/gtk.h>
#include "GameLogic.h"

#define MIN_BUTTON_SIZE 30

class Gui {
private:
    int argc;
    char **argv;
    GameLogic *gameLogic;

    GtkApplication *gtkApplication;
    GtkWidget *mainWindow;
    GtkWidget *playGrid;

    void createMainWindow();

    void createPlayGrid();

    void initGui();

    void redrawButton(GtkWidget *widget);

    void getButtonIndices(GtkWidget *widget, unsigned long *row, unsigned long *col);

    // Callbacks
    static void activateApplicationCB(GtkApplication *app, gpointer data);

    static void destroyMainWindowCB(GtkWidget *widget, gpointer data);

    static void playGridButtonClickedCB(GtkWidget *widget, gpointer data);

    static gboolean drawWinStroke(GtkWidget *widget, cairo_t *cr, gpointer data);

public:

    Gui(int argc, char **argv, GameLogic *gameLogic);

    int run();
};


#endif //GUX2CPLUSPLUS_GUI_H
