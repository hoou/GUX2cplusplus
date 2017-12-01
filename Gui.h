#ifndef GUX2CPLUSPLUS_GUI_H
#define GUX2CPLUSPLUS_GUI_H

#include <gtk/gtk.h>
#include "GameLogic.h"

#define PLAY_GRID_CELL_MIN_SIZE 30

class Gui {
private:
    int argc;
    char **argv;
    GameLogic *gameLogic;

    GtkApplication *gtkApplication;
    GtkWidget *mainWindow;
    GtkWidget *layoutContainer;
    GtkWidget *homeScreen;
    GtkWidget *playGrid;
    GtkWidget *menuBarBox;
    GtkWidget *activePlayerLabel;

    unsigned int gridSizeScaleValue = DEFAULT_GRID_SIZE;
    unsigned int numberOfCellsInRowToWin = DEFAULT_NUMBER_OF_CELLS_IN_ROW_TO_WIN;
    std::string player1color = PLAYER1_DEFAULT_COLOR;
    std::string player2color = PLAYER2_DEFAULT_COLOR;

    void createMainWindow();

    void createLayoutContainer();

    void createHomeScreen();

    void createMenuBar();

    void createPlayGrid();

    void showHomeScreen();

    void showGameScreen();

    void showGameOverWindow(Player *winner);

    void showAboutDialog();

    void initGui();

    void startNewGame();

    void getPlayGridCellIndices(GtkWidget *widget, unsigned long *row, unsigned long *col);

    void removeAllChildrenFromLayoutContainer();

    static void removeWidget(GtkWidget *widget, gpointer data);

    void updateActivePlayerLabel();

    // CSS modifiers
    void changeFontSizeOfWidget(GtkWidget *widget, unsigned int fontSize);

    void changeFontColorOfWidget(GtkWidget *widget, const std::string &color);

    void changeBorderRadiusOfWidget(GtkWidget *widget, unsigned int radius);

    void changeBorderWidthOfWidget(GtkWidget *widget, unsigned int width);

    void changeBorderColorOfWidget(GtkWidget *widget, const std::string &color);

    void changeBackgroundColorOfWidget(GtkWidget *widget, const std::string &color);

    void setBackgroundGradientToWidget(GtkWidget *widget, const std::string &color1, const std::string &color2);

    void setBackgroundGradientOnHoverToWidget(GtkWidget *widget, const std::string &color1, const std::string &color2);

    void setMinSizeOfMainWindow(int size);

    // Callbacks
    static void activateApplicationCB(GtkApplication *app, gpointer data);

    static void destroyMainWindowCB(GtkWidget *widget, gpointer data);

    static gboolean playGridCellButtonPressEventCB(GtkWidget *widget, GdkEvent *event, gpointer data);

    static gboolean playGridCellDrawCB(GtkWidget *widget, cairo_t *cr, gpointer data);

    static void startButtonClickedCB(GtkWidget *widget, gpointer data);

    static void gridSizeScaleValueChangedCB(GtkRange *range, gpointer data);

    static void numberOfCellsInRowToWinButtonToggledCB(GtkToggleButton *toggleButton, gpointer data);

    static void colorButtonColorSetCB(GtkColorButton *widget, gpointer data);

    static void gameOverDialogResponseCB(GtkDialog *dialog, gint responseId, gpointer data);

    // Menu items callbacks

    static void newGameMenuItemActivateCB(GtkWidget *widget, gpointer data);

    static void restartGameMenuItemActivateCB(GtkWidget *widget, gpointer data);

    static void quitMenuItemActivateCB(GtkWidget *widget, gpointer data);

    static void aboutMenuItemActivateCB(GtkWidget *widget, gpointer data);

    // Drawing
    void drawX(cairo_t *cr, unsigned int lineWidth, const std::string &color, unsigned int width, unsigned int height);

    void drawO(cairo_t *cr, unsigned int lineWidth, const std::string &color, unsigned int width, unsigned int height);

public:

    Gui(int argc, char **argv);

    int run();

    void quit();
};


#endif //GUX2CPLUSPLUS_GUI_H
