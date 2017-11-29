#include <iostream>
#include "Gui.h"

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

void Gui::createMainWindow() {
    mainWindow = gtk_application_window_new(gtkApplication);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Piskvorky");

    GdkGeometry hints{};
    hints.min_width = 512;
    hints.min_height = 512;

    gtk_window_set_geometry_hints(GTK_WINDOW(mainWindow), mainWindow, &hints, GDK_HINT_MIN_SIZE);

    g_signal_connect(mainWindow, "destroy", G_CALLBACK(destroyMainWindowCB), this);
}

void Gui::createLayoutContainer() {
    layoutContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), layoutContainer);
}

void Gui::createHomeScreen() {
    GtkWidget *box, *image, *optionsBox, *colorOptionsBox, *gridSizeOptionsBox, *numberOfCellsInRowToWinOptionsBox, *scale, *label, *button, *buttonBox, *vbox;
    GSList *group;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(box, true);
    gtk_widget_set_margin_start(box, 30);
    gtk_widget_set_margin_end(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_hexpand(box, true);

    // IMAGE
    image = gtk_image_new_from_file("assets/piskvorky.png");

    // OPTIONS
    optionsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_widget_set_halign(optionsBox, GTK_ALIGN_CENTER);

    // GRID SIZE
    gridSizeOptionsBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(optionsBox), gridSizeOptionsBox, false, false, 0);

    label = gtk_label_new("Grid size");
    gtk_box_pack_start(GTK_BOX(gridSizeOptionsBox), label, false, false, 0);

    scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, MIN_GRID_SIZE, MAX_GRID_SIZE, 1);
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
    colorOptionsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    gtk_widget_set_halign(colorOptionsBox, GTK_ALIGN_CENTER);
    GdkRGBA color{};

    // PLAYER 1 COLORS
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    label = gtk_label_new("Player 1 color");
    gdk_rgba_parse(&color, PLAYER1_DEFAULT_COLOR);
    button = gtk_color_button_new_with_rgba(&color);
    player1color = PLAYER1_DEFAULT_COLOR;
    g_signal_connect(button, "color-set", G_CALLBACK(colorButtonColorSetCB), &player1color);
    gtk_box_pack_start(GTK_BOX(vbox), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
    gtk_box_pack_start(GTK_BOX(colorOptionsBox), vbox, false, false, 0);

    // PLAYER 2 COLORS
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    label = gtk_label_new("Player 2 color");
    gdk_rgba_parse(&color, PLAYER2_DEFAULT_COLOR);
    button = gtk_color_button_new_with_rgba(&color);
    g_signal_connect(button, "color-set", G_CALLBACK(colorButtonColorSetCB), &player2color);
    player2color = PLAYER2_DEFAULT_COLOR;
    gtk_box_pack_start(GTK_BOX(vbox), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
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
    GtkWidget *gameMenu, *helpMenu, *gameMenuItem, *helpMenuItem, *newGameMenuItem, *restartGameMenuItem, *quitMenuItem, *menuBar, *aboutMenuItem;

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

    menuBarBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_box_pack_start(GTK_BOX(menuBarBox), menuBar, true, true, 0);
}

void Gui::createPlayGrid() {
    GtkWidget *button, *overlay, *label;

    playGrid = gtk_grid_new();

    gtk_grid_set_column_homogeneous(GTK_GRID(playGrid), true);
    gtk_grid_set_row_homogeneous(GTK_GRID(playGrid), true);

    for (int row = 0; row < gameLogic->getGridSize(); row++) {
        for (int col = 0; col < gameLogic->getGridSize(); col++) {
            button = gtk_button_new();
            label = gtk_label_new(" ");
            overlay = gtk_overlay_new();

            gtk_container_add(GTK_CONTAINER(button), label);
            gtk_overlay_add_overlay(GTK_OVERLAY(overlay), button);

            gtk_widget_set_hexpand(button, true);
            gtk_widget_set_vexpand(button, true);

            gtk_widget_set_hexpand(overlay, true);
            gtk_widget_set_vexpand(overlay, true);

            g_signal_connect(button, "clicked", G_CALLBACK(playGridButtonClickedCB), this);
            g_signal_connect(button, "size-allocate", G_CALLBACK(playGridButtonSizeAllocateCB), this);

            gtk_grid_attach(GTK_GRID(playGrid), overlay, row, col, 1, 1);
        }
    }
}

void Gui::createStatusBar() {
    GtkWidget *label, *box;

    statusBar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_pack_start(GTK_BOX(statusBar), box, false, false, 20);

    label = gtk_label_new("On the move: ");
    activePlayerLabel = gtk_label_new("");
    gtk_widget_set_size_request(activePlayerLabel, 50, 10);

    gtk_box_pack_start(GTK_BOX(box), label, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), activePlayerLabel, false, false, 0);
}

void Gui::removeAllChildrenFromLayoutContainer() {
    gtk_container_foreach(GTK_CONTAINER(layoutContainer), (GtkCallback) removeWidget, this);
}

void Gui::showHomeScreen() {
    removeAllChildrenFromLayoutContainer();

    createHomeScreen();
    gtk_box_pack_start(GTK_BOX(layoutContainer), homeScreen, false, false, 0);

    gtk_widget_show_all(mainWindow);
}

void Gui::showGameScreen() {
    GtkWidget *aspectFrame;

    removeAllChildrenFromLayoutContainer();

    createMenuBar();
    createPlayGrid();
    createStatusBar();

    aspectFrame = gtk_aspect_frame_new(nullptr, 0.5, 0.5, 1, false);
    gtk_container_add(GTK_CONTAINER(aspectFrame), playGrid);

    gtk_widget_set_vexpand(statusBar, false);
    gtk_widget_set_hexpand(statusBar, false);

    gtk_box_pack_start(GTK_BOX(layoutContainer), menuBarBox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(layoutContainer), aspectFrame, true, true, 0);
    gtk_box_pack_start(GTK_BOX(layoutContainer), statusBar, false, false, 0);

    gtk_widget_set_halign(statusBar, GTK_ALIGN_CENTER);

    gtk_widget_show_all(mainWindow);
}

void Gui::showGameOverWindow(Player *winner) {
    std::string message = winner->getName() + " wins! Congratulations!\n";
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
            "version", "1.0",
            NULL
    );
}

void Gui::getButtonIndices(GtkWidget *widget, unsigned long *row, unsigned long *col) {
    gint left, top;

    gtk_container_child_get(GTK_CONTAINER(playGrid), gtk_widget_get_parent(widget),
                            "top-attach", &top,
                            "left-attach", &left,
                            NULL);
    *row = static_cast<unsigned long>(top);
    *col = static_cast<unsigned long>(left);
}

void Gui::redrawButton(GtkWidget *widget, std::string color) {
    unsigned long row, col;

    getButtonIndices(widget, &row, &col);

    Cell *cell = gameLogic->getCell(row, col);

    gtk_button_set_label(GTK_BUTTON(widget), cell->getStringValue().c_str());

    changeFontColorOfWidget(gtk_bin_get_child(GTK_BIN(widget)), color);
}

void Gui::removeWidget(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gtk_container_remove(GTK_CONTAINER(gui->layoutContainer), widget);
}

void Gui::changeFontSizeOfWidget(GtkWidget *widget, unsigned int fontSize) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, ("* {font-size: " + std::to_string(fontSize) + "px}").c_str(), -1,
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

void Gui::startNewGame() {
    delete gameLogic;
    gameLogic = new GameLogic(gridSizeScaleValue, numberOfCellsInRowToWin, player1color, player2color);

    showGameScreen();
    updateActivePlayerLabel();
}

// CALLBACKS

void Gui::destroyMainWindowCB(GtkWidget *widget, gpointer data) {
    g_application_quit(G_APPLICATION(((Gui *) data)->gtkApplication));
}

void Gui::activateApplicationCB(GtkApplication *app, gpointer data) {
    ((Gui *) data)->initGui();
    gtk_widget_show_all(((Gui *) data)->mainWindow);
}

void Gui::playGridButtonClickedCB(GtkWidget *widget, gpointer data) {
    unsigned long row, col;
    bool result;
    auto *gui = (Gui *) data;
    std::string color = gui->gameLogic->getActivePlayer()->getColor();

    gui->getButtonIndices(widget, &row, &col);

    result = gui->gameLogic->updateGrid(row, col);

    if (result) {
        gui->redrawButton(widget, color);

        Player *winner = gui->gameLogic->getWinner();
        if (winner) {
            gui->showGameOverWindow(winner);

            for (auto cell : gui->gameLogic->getWinningCells()) {
                GtkWidget *winningCell = gtk_grid_get_child_at(GTK_GRID(gui->playGrid),
                                                               static_cast<gint>(cell->getCol()),
                                                               static_cast<gint>(cell->getRow()));
                GtkWidget *drawingArea = gtk_drawing_area_new();

                g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(drawWinStroke), cell);

                gtk_overlay_add_overlay(GTK_OVERLAY(winningCell), drawingArea);
                gtk_widget_show_all(winningCell);
            }
        }

        gtk_widget_set_sensitive(widget, false);

        gui->updateActivePlayerLabel();
    }
}

void Gui::playGridButtonSizeAllocateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    auto fontSize = static_cast<unsigned int>(gtk_widget_get_allocated_width(widget) / 2.0);

    gui->changeFontSizeOfWidget(widget, fontSize);
}

void Gui::updateActivePlayerLabel() {
    std::string name = gameLogic->getActivePlayer()->getName();
    std::string symbol = Cell::convertValueToString(gameLogic->getActivePlayer()->getSymbol());

    changeFontColorOfWidget(activePlayerLabel, gameLogic->getActivePlayer()->getColor());
    changeFontSizeOfWidget(activePlayerLabel, 20);

    gtk_label_set_text(GTK_LABEL(activePlayerLabel), symbol.c_str());
}

gboolean Gui::drawWinStroke(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width, height;
    GdkRGBA color{};
    GtkStyleContext *context;
    auto *cell = (Cell *) (data);

    context = gtk_widget_get_style_context(widget);

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    gtk_render_background(context, cr, 0, 0, width, height);

    int buttonWidth = gtk_widget_get_allocated_width(widget);

    cairo_set_line_width(cr, (int)(buttonWidth / 10.0));

    switch (cell->getWinningCellSequenceDirection()) {
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

    gtk_style_context_get_color(context,
                                gtk_style_context_get_state(context),
                                &color);
    gdk_cairo_set_source_rgba(cr, &color);

    cairo_stroke(cr);
    return false;
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
