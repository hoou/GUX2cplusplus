#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "Gui.h"
#include "utils.h"

Gui::Gui(int argc, char **argv) : argc(argc), argv(argv) {
    gtkApplication = gtk_application_new("cz.vutbr.fit.stud.xmikit01.piskvorky", G_APPLICATION_FLAGS_NONE);

    // When g_application_run() called, function activate() will be called also

    g_signal_connect(gtkApplication, "activate", G_CALLBACK(activateApplicationCB), this);
}

void Gui::initGui() {
    createMainWindow();
    createLayoutContainer();

    showHomeScreen();
}

int Gui::run() {
    return g_application_run(G_APPLICATION(gtkApplication), argc, argv);
}

void Gui::quit() {
    g_application_quit(G_APPLICATION(gtkApplication));
}

void Gui::startNewGame() {
    delete gameLogic;
    gameLogic = new GameLogic(gridSizeScaleValue, numberOfCellsInRowToWin, player1color, player2color);

    showGameScreen();
}

void Gui::createMainWindow() {
    mainWindow = gtk_application_window_new(gtkApplication);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Piskvorky");

    g_signal_connect(mainWindow, "destroy", G_CALLBACK(destroyMainWindowCB), this);
}

void Gui::createLayoutContainer() {
    layoutContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_bottom(layoutContainer, 20);
    gtk_container_add(GTK_CONTAINER(mainWindow), layoutContainer);
}

void Gui::createHomeScreen() {
    GtkWidget *box, *image, *optionsBox, *colorOptionsBox, *gridSizeOptionsBox, *numberOfCellsInRowToWinOptionsBox;
    GtkWidget *scale, *label, *button, *buttonBox, *vbox, *hbox;
    GSList *group;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(box, true);
    gtk_widget_set_margin_start(box, 30);
    gtk_widget_set_margin_end(box, 30);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_hexpand(box, true);

    // IMAGE
    image = gtk_image_new_from_file("piskvorky.png");

    // OPTIONS
    optionsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_widget_set_halign(optionsBox, GTK_ALIGN_CENTER);

    // GRID SIZE
    gridSizeOptionsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(optionsBox), gridSizeOptionsBox, false, false, 0);

    label = gtk_label_new("Grid size");
    gtk_box_pack_start(GTK_BOX(gridSizeOptionsBox), label, false, false, 0);

    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, MIN_GRID_SIZE, MAX_GRID_SIZE, 1);
    gtk_widget_set_size_request(scale, 80, 5);
    gtk_range_set_value(GTK_RANGE(scale), DEFAULT_GRID_SIZE);
    g_signal_connect(scale, "value-changed", G_CALLBACK(gridSizeScaleValueChangedCB), this);
    gridSizeScaleValue = DEFAULT_GRID_SIZE;
    gtk_box_pack_start(GTK_BOX(gridSizeOptionsBox), scale, true, true, 0);

    // NUMBER OF CELLS IN ROW TO WIN
    numberOfCellsInRowToWinOptionsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(optionsBox), numberOfCellsInRowToWinOptionsBox, false, false, 0);

    label = gtk_label_new("Symbols to win");
    gtk_box_pack_start(GTK_BOX(numberOfCellsInRowToWinOptionsBox), label, false, false, 0);

    button = gtk_radio_button_new_with_label(nullptr, "3");
    g_signal_connect(button, "toggled", G_CALLBACK(numberOfCellsInRowToWinButtonToggledCB), this);
    gtk_box_pack_start(GTK_BOX(numberOfCellsInRowToWinOptionsBox), button, true, true, 0);

    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (button));
    button = gtk_radio_button_new_with_label(group, "4");
    g_signal_connect(button, "toggled", G_CALLBACK(numberOfCellsInRowToWinButtonToggledCB), this);
    gtk_box_pack_start(GTK_BOX(numberOfCellsInRowToWinOptionsBox), button, true, true, 0);

    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON (button));
    button = gtk_radio_button_new_with_label(group, "5");
    g_signal_connect(button, "toggled", G_CALLBACK(numberOfCellsInRowToWinButtonToggledCB), this);
    gtk_box_pack_start(GTK_BOX(numberOfCellsInRowToWinOptionsBox), button, true, true, 0);

    numberOfCellsInRowToWin = DEFAULT_NUMBER_OF_CELLS_IN_ROW_TO_WIN;

    // COLORS
    colorOptionsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_widget_set_halign(colorOptionsBox, GTK_ALIGN_CENTER);
    GdkRGBA color{};

    // PLAYER 1 COLORS
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label = gtk_label_new("Player 1 color");
    gdk_rgba_parse(&color, PLAYER1_DEFAULT_COLOR);
    button = gtk_color_button_new_with_rgba(&color);
    player1color = PLAYER1_DEFAULT_COLOR;
    g_signal_connect(button, "color-set", G_CALLBACK(colorButtonColorSetCB), &player1color);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    player1ColorChooseDrawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(player1ColorChooseDrawingArea, 30, 30);
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);
    g_signal_connect(player1ColorChooseDrawingArea,"draw", G_CALLBACK(player1ColorChooseDrawingAreaDrawCB), this);
    gtk_box_pack_start(GTK_BOX(hbox), player1ColorChooseDrawingArea, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(colorOptionsBox), vbox, false, false, 0);

    // PLAYER 2 COLORS
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label = gtk_label_new("Player 2 color");
    gdk_rgba_parse(&color, PLAYER2_DEFAULT_COLOR);
    button = gtk_color_button_new_with_rgba(&color);
    player2color = PLAYER2_DEFAULT_COLOR;
    g_signal_connect(button, "color-set", G_CALLBACK(colorButtonColorSetCB), &player2color);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    player2ColorChooseDrawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(player2ColorChooseDrawingArea, 30, 30);
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);
    g_signal_connect(player2ColorChooseDrawingArea,"draw", G_CALLBACK(player2ColorChooseDrawingAreaDrawCB), this);
    gtk_box_pack_start(GTK_BOX(hbox), player2ColorChooseDrawingArea, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(colorOptionsBox), vbox, false, false, 0);

    // START BUTTON
    button = gtk_button_new_with_label("START");
    g_signal_connect(button, "clicked", G_CALLBACK(startButtonClickedCB), this);
    buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(buttonBox), button);

    gtk_box_pack_start(GTK_BOX(box), image, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), optionsBox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), colorOptionsBox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonBox, false, false, 0);

    homeScreen = box;
}

void Gui::createMenuBar() {
    GtkWidget *gameMenu, *helpMenu, *gameMenuItem, *helpMenuItem, *newGameMenuItem, *restartGameMenuItem;
    GtkWidget *quitMenuItem, *menuBar, *aboutMenuItem;
    GtkWidget *label, *box;

    menuBar = gtk_menu_bar_new();

    gameMenu = gtk_menu_new();
    helpMenu = gtk_menu_new();
    gameMenuItem = gtk_menu_item_new_with_label("Game");
    helpMenuItem = gtk_menu_item_new_with_label("Help");
    newGameMenuItem = gtk_menu_item_new_with_label("New game");
    restartGameMenuItem = gtk_menu_item_new_with_label("Restart game");
    quitMenuItem = gtk_menu_item_new_with_label("Quit");
    aboutMenuItem = gtk_menu_item_new_with_label("About");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(gameMenuItem), gameMenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMenuItem), helpMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(gameMenu), newGameMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(gameMenu), restartGameMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(gameMenu), quitMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), aboutMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), gameMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), helpMenuItem);
    g_signal_connect(newGameMenuItem, "activate", G_CALLBACK(newGameMenuItemActivateCB), this);
    g_signal_connect(restartGameMenuItem, "activate", G_CALLBACK(restartGameMenuItemActivateCB), this);
    g_signal_connect(quitMenuItem, "activate", G_CALLBACK(quitMenuItemActivateCB), this);
    g_signal_connect(aboutMenuItem, "activate", G_CALLBACK(aboutMenuItemActivateCB), this);

    menuBarBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Create labels for game status as the part of the menu bar
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    gtk_widget_set_margin_end(box, 10);

    label = gtk_label_new("on the move: ");
    onTheMoveDrawingAreaDraw = gtk_drawing_area_new();
    gtk_widget_set_size_request(onTheMoveDrawingAreaDraw, 30, 30);
    g_signal_connect(onTheMoveDrawingAreaDraw, "draw", G_CALLBACK(onTheMoveDrawingAreaDrawCB), this);
    gtk_box_pack_start(GTK_BOX(box), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), onTheMoveDrawingAreaDraw, false, false, 0);

    gtk_box_pack_start(GTK_BOX(menuBarBox), menuBar, false, false, 0);
    gtk_box_pack_start(GTK_BOX(menuBarBox), box, true, true, 0);
}

void Gui::createPlayGrid() {
    GtkWidget *cell;

    playGrid = gtk_grid_new();

    for (unsigned int row = 0; row < gameLogic->getGridSize(); row++) {
        for (unsigned int col = 0; col < gameLogic->getGridSize(); col++) {
            cell = gtk_drawing_area_new();

            setBackgroundGradientToWidget(cell, "#f4f5f5", "#dfdddd");

            g_signal_connect(cell, "draw", G_CALLBACK(playGridCellDrawCB), this);
            g_signal_connect(cell, "button-press-event", G_CALLBACK(playGridCellButtonPressEventCB), this);

            gtk_widget_set_events(cell, gtk_widget_get_events(cell) | GDK_BUTTON_PRESS_MASK);

            gtk_grid_attach(GTK_GRID(playGrid), cell, row, col, 1, 1);
        }
    }
}

void Gui::setMinSizeOfMainWindow(int size) {
    GdkGeometry hints{};
    hints.min_width = size;
    hints.min_height = size;
    hints.min_aspect = 1;
    hints.max_aspect = 1;
    gtk_window_set_geometry_hints(GTK_WINDOW(mainWindow), nullptr, &hints,
                                  (GdkWindowHints) (GDK_HINT_MIN_SIZE | GDK_HINT_ASPECT));
}

void Gui::removeAllChildrenFromLayoutContainer() {
    gtk_container_foreach(GTK_CONTAINER(layoutContainer), (GtkCallback) removeWidget, this);
}

void Gui::showHomeScreen() {
    removeAllChildrenFromLayoutContainer();

    createHomeScreen();
    gtk_box_pack_start(GTK_BOX(layoutContainer), homeScreen, false, false, 0);

    setMinSizeOfMainWindow(-1);
    gtk_window_resize(GTK_WINDOW(mainWindow), 200, 200);

    gtk_widget_show_all(mainWindow);
}

void Gui::showGameScreen() {
    GtkWidget *aspectFrame;
    unsigned int minSizeOfMainWindow;

    removeAllChildrenFromLayoutContainer();

    createMenuBar();
    createPlayGrid();

    aspectFrame = gtk_aspect_frame_new(nullptr, 0.5, 0.5, 1, false);
    gtk_container_add(GTK_CONTAINER(aspectFrame), playGrid);

    gtk_box_pack_start(GTK_BOX(layoutContainer), menuBarBox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(layoutContainer), aspectFrame, true, true, 0);

    // Set min size of window
    minSizeOfMainWindow = gameLogic->getGridSize() * PLAY_GRID_CELL_MIN_SIZE;
    if (minSizeOfMainWindow < 200)
        minSizeOfMainWindow = 200;

    setMinSizeOfMainWindow(minSizeOfMainWindow);

    gtk_grid_set_column_homogeneous(GTK_GRID(playGrid), true);
    gtk_grid_set_row_homogeneous(GTK_GRID(playGrid), true);
    gtk_grid_set_column_spacing(GTK_GRID(playGrid), 1);
    gtk_grid_set_row_spacing(GTK_GRID(playGrid), 1);

    gtk_widget_show_all(mainWindow);
}

void Gui::showGameOverWindow(Player *winner) {
    std::string message;

    if (winner) {
        message = winner->getName() + " wins! Congratulations!";
    } else {
        message = "Tie game!";
    }

    GtkWidget *gameOverDialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_NONE, "%s", message.c_str());

    gtk_dialog_add_button(GTK_DIALOG(gameOverDialog), "New game", GTK_RESPONSE_YES);
    gtk_dialog_add_button(GTK_DIALOG(gameOverDialog), "Restart game", GTK_RESPONSE_NO);
    gtk_dialog_add_button(GTK_DIALOG(gameOverDialog), "Close", GTK_RESPONSE_CLOSE);


    g_signal_connect(gameOverDialog, "response", G_CALLBACK(gameOverDialogResponseCB), this);

    gtk_widget_show_all(gameOverDialog);
}

void Gui::showAboutDialog() {
    const char *authors[] = {"Tibor Mikita <xmikit01@stud.fit.vutbr.cz>", nullptr};

    gtk_show_about_dialog(
            GTK_WINDOW(mainWindow),
            "authors", &authors,
            "comments", "This is basically simple tic-tac-toe game written for school project.",
            "copyright", "© 2017 Tibor Mikita",
            "logo-icon-name", NULL,
            "program-name", "Piskvorky",
            "version", "2.0",
            NULL
    );
}

void Gui::getPlayGridCellIndices(GtkWidget *widget, unsigned long *row, unsigned long *col) {
    gint left, top;

    gtk_container_child_get(GTK_CONTAINER(playGrid), widget,
                            "top-attach", &top,
                            "left-attach", &left,
                            NULL);
    *row = static_cast<unsigned long>(top);
    *col = static_cast<unsigned long>(left);
}

void Gui::removeWidget(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gtk_container_remove(GTK_CONTAINER(gui->layoutContainer), widget);
}

// CSS modifiers
void Gui::changeFontSizeOfWidget(GtkWidget *widget, unsigned int fontSize) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {font-size: " + to_string(fontSize) + "pt}").c_str(), -1,
                                    nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::changeFontColorOfWidget(GtkWidget *widget, const std::string &color) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {color: " + color + "}").c_str(), -1, nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::changeBorderRadiusOfWidget(GtkWidget *widget, unsigned int radius) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {border-radius: " + to_string(radius) + "pt; }").c_str(), -1,
                                    nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::changeBorderWidthOfWidget(GtkWidget *widget, unsigned int width) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {border-width: " + to_string(width) + "pt; }").c_str(), -1, nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::changeBorderColorOfWidget(GtkWidget *widget, const std::string &color) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {border-color: " + color + "; }").c_str(), -1, nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::changeBackgroundColorOfWidget(GtkWidget *widget, const std::string &color) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {background-color: " + color + "; }").c_str(), -1, nullptr);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void Gui::setBackgroundGradientToWidget(GtkWidget *widget, const std::string &color1, const std::string &color2) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(
            provider,
            ("* {background: linear-gradient(to bottom," + color1 + ", " + color2 + "); }").c_str(),
            -1,
            nullptr
    );
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void
Gui::setBackgroundGradientOnHoverToWidget(GtkWidget *widget, const std::string &color1, const std::string &color2) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(
            provider,
            ("*:active {background: linear-gradient(to bottom," + color1 + ", " + color2 + "); }").c_str(),
            -1,
            nullptr
    );
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// CALLBACKS

void Gui::destroyMainWindowCB(GtkWidget *widget, gpointer data) {
    g_application_quit(G_APPLICATION(((Gui *) data)->gtkApplication));
}

void Gui::activateApplicationCB(GtkApplication *app, gpointer data) {
    ((Gui *) data)->initGui();
    gtk_widget_show_all(((Gui *) data)->mainWindow);
}

gboolean Gui::playGridCellButtonPressEventCB(GtkWidget *widget, GdkEvent *event, gpointer data) {
    unsigned long row, col;
    bool result;
    auto *gui = (Gui *) data;

    gui->getPlayGridCellIndices(widget, &row, &col);

    result = gui->gameLogic->updateGrid(row, col);

    if (result) {
        Player *winner = gui->gameLogic->getWinner();
        if (winner) {
            gui->showGameOverWindow(winner);
        } else if (gui->gameLogic->isTieGame()) {
            gui->showGameOverWindow(nullptr);
        }

        gtk_widget_set_sensitive(widget, false);

        gtk_widget_queue_draw(gui->onTheMoveDrawingAreaDraw);
    }

    return false;
}

gboolean Gui::playGridCellDrawCB(GtkWidget *widget, cairo_t *cr, gpointer data) {
    auto *gui = (Gui *) data;
    unsigned long row, col;

    guint width, height;
    GdkRGBA color{};
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(widget);

    width = static_cast<guint>(gtk_widget_get_allocated_width(widget));
    height = static_cast<guint>(gtk_widget_get_allocated_height(widget));

    gtk_render_background(context, cr, 0, 0, width, height);

    // Setup line width
    auto lineWidth = (unsigned int) (gtk_widget_get_allocated_width(widget) / 10.0);

    // Get cell information from game logic
    gui->getPlayGridCellIndices(widget, &row, &col);
    Cell *cell = gui->gameLogic->getCell(row, col);

    // If cell not empty, draw X or O
    switch (cell->getValue()) {
        case X:
            gui->drawX(cr, lineWidth, cell->getColor(), width, height);
            break;
        case O:
            gui->drawO(cr, lineWidth, cell->getColor(), width, height);
            break;
        default:
            break;
    }

    // If game is over and there is winner (no tie game)
    if (!gui->gameLogic->isGameRunning() && !gui->gameLogic->isTieGame()) {
        // If cell is winning, draw win stroke
        if (gui->gameLogic->isCellWinning(cell)) {
            cairo_set_line_width(cr, lineWidth * 2);
            switch (gui->gameLogic->getWinningCellSequenceDirection()) {
                case Horizontal:
                    cairo_move_to(cr, 0, height / 2.0);
                    cairo_line_to(cr, width, height / 2.0);
                    break;
                case Vertical:
                    cairo_move_to(cr, width / 2.0, 0);
                    cairo_line_to(cr, width / 2.0, height);
                    break;
                case Diagonal:
                    cairo_move_to(cr, 0, 0);
                    cairo_line_to(cr, width, height);
                    break;
                case OppositeDiagonal:
                    cairo_move_to(cr, width, 0);
                    cairo_line_to(cr, 0, height);
                    break;
                default:
                    break;
            }

            gdk_rgba_parse(&color, "black");
            gdk_cairo_set_source_rgba(cr, &color);

            cairo_stroke(cr);
        }
    }

    return FALSE;
}

void Gui::startButtonClickedCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->startNewGame();
}

void Gui::gridSizeScaleValueChangedCB(GtkRange *range, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->gridSizeScaleValue = static_cast<unsigned int>(gtk_range_get_value(range));
}

void Gui::numberOfCellsInRowToWinButtonToggledCB(GtkToggleButton *toggleButton, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    if (gtk_toggle_button_get_active(toggleButton)) {
        gui->numberOfCellsInRowToWin = static_cast<unsigned int>(atoi(gtk_button_get_label(GTK_BUTTON(toggleButton))));
    }
}

void Gui::colorButtonColorSetCB(GtkColorButton *widget, gpointer data) {
    auto *target = static_cast<std::string *>(data);
    GdkRGBA color{};
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &color);
    *target = gdk_rgba_to_string(&color);
}

void Gui::gameOverDialogResponseCB(GtkDialog *dialog, gint responseId, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    switch (responseId) {
        case GTK_RESPONSE_YES:
            gtk_window_close(GTK_WINDOW(dialog));
            gui->showHomeScreen();
            break;
        case GTK_RESPONSE_NO:
            gtk_window_close(GTK_WINDOW(dialog));
            gui->startNewGame();
            break;
        case GTK_RESPONSE_CLOSE:
            gtk_window_close(GTK_WINDOW(dialog));
            break;
        default:
            break;
    }
}

gboolean Gui::player1ColorChooseDrawingAreaDrawCB(GtkWidget *widget, cairo_t *cr, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    guint width, height;
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(widget);

    width = static_cast<guint>(gtk_widget_get_allocated_width(widget));
    height = static_cast<guint>(gtk_widget_get_allocated_height(widget));
    auto lineWidth = (unsigned int) (gtk_widget_get_allocated_width(widget) / 10.0);

    gtk_render_background(context, cr, 0, 0, width, height);

    gui->drawO(cr, lineWidth, gui->player1color, width, height);

    return false;
}

gboolean Gui::player2ColorChooseDrawingAreaDrawCB(GtkWidget *widget, cairo_t *cr, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    guint width, height;
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(widget);

    width = static_cast<guint>(gtk_widget_get_allocated_width(widget));
    height = static_cast<guint>(gtk_widget_get_allocated_height(widget));
    auto lineWidth = (unsigned int) (gtk_widget_get_allocated_width(widget) / 10.0);

    gtk_render_background(context, cr, 0, 0, width, height);

    gui->drawX(cr, lineWidth, gui->player2color, width, height);

    return false;
}

gboolean Gui::onTheMoveDrawingAreaDrawCB(GtkWidget *widget, cairo_t *cr, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    guint width, height;
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(widget);

    width = static_cast<guint>(gtk_widget_get_allocated_width(widget));
    height = static_cast<guint>(gtk_widget_get_allocated_height(widget));
    auto lineWidth = (unsigned int) (gtk_widget_get_allocated_width(widget) / 10.0);

    gtk_render_background(context, cr, 0, 0, width, height);

    Player *activePlayer = gui->gameLogic->getActivePlayer();

    if (activePlayer->getSymbol() == X) {
        gui->drawX(cr, lineWidth, activePlayer->getColor(), width, height);
    } else if (activePlayer->getSymbol() == O) {
        gui->drawO(cr, lineWidth, activePlayer->getColor(), width, height);
    }

    return false;
}

// Menu items callbacks
void Gui::newGameMenuItemActivateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->showHomeScreen();
}

void Gui::restartGameMenuItemActivateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->startNewGame();
}

void Gui::quitMenuItemActivateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->quit();
}

void Gui::aboutMenuItemActivateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->showAboutDialog();
}

// Drawing

void
Gui::drawX(cairo_t *cr, unsigned int lineWidth, const std::string &color, unsigned int width, unsigned int height) {
    GdkRGBA rgba{};
    cairo_move_to(cr, 0 + lineWidth, 0 + lineWidth);
    cairo_line_to(cr, width - lineWidth, height - lineWidth);
    cairo_move_to(cr, width - lineWidth, 0 + lineWidth);
    cairo_line_to(cr, 0 + lineWidth, height - lineWidth);
    gdk_rgba_parse(&rgba, color.c_str());
    gdk_cairo_set_source_rgba(cr, &rgba);
    cairo_stroke(cr);
}

void
Gui::drawO(cairo_t *cr, unsigned int lineWidth, const std::string &color, unsigned int width, unsigned int height) {
    GdkRGBA rgba{};
    cairo_arc(cr, width / 2.0, height / 2.0, width / 2.0 - lineWidth, 0.0, 2 * M_PI);
    gdk_rgba_parse(&rgba, color.c_str());
    gdk_cairo_set_source_rgba(cr, &rgba);
    cairo_stroke(cr);
}
