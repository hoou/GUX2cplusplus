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
    GtkWidget *layoutGrid;
    GtkWidget *homeScreen;
    GtkWidget *playGrid;
    GtkWidget *menuBarBox;

    unsigned int gridSizeScaleValue = DEFAULT_GRID_SIZE;
    unsigned int numberOfCellsInRowToWin = DEFAULT_NUMBER_OF_CELLS_IN_ROW_TO_WIN;

    void createMainWindow();

    void createLayoutGrid();

    void createHomeScreen();

    void createMenuBar();

    void createPlayGrid();

    void showHomeScreen();

    void showPlayGrid();

    void initGui();

    void redrawButton(GtkWidget *widget);

    void getButtonIndices(GtkWidget *widget, unsigned long *row, unsigned long *col);

    void hideAllWidgetsInLayoutGrid();

    static void hideWidget(GtkWidget *widget, gpointer data);

    // Callbacks
    static void activateApplicationCB(GtkApplication *app, gpointer data);

    static void destroyMainWindowCB(GtkWidget *widget, gpointer data);

    static void playGridButtonClickedCB(GtkWidget *widget, gpointer data);

    static gboolean drawWinStroke(GtkWidget *widget, cairo_t *cr, gpointer data);

    static void startButtonClickedCB(GtkWidget *widget, gpointer data);

    static void gridSizeScaleValueChangedCB(GtkRange *range, gpointer data);

    static void numberOfCellsInRowToWinButtonToggledCB(GtkToggleButton *toggleButton, gpointer data);

    // Menu items callbacks

    static void newGameMenuItemActivateCB(GtkWidget *widget, gpointer data);

public:

    Gui(int argc, char **argv);

    int run();
};


#endif //GUX2CPLUSPLUS_GUI_H
