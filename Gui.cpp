#include <iostream>
#include "Gui.h"

Gui::Gui(int argc, char **argv) : argc(argc), argv(argv) {
    gtkApplication = gtk_application_new("cz.vutbr.fit.stud.xmikit01.piskvorky", G_APPLICATION_FLAGS_NONE);

    // When g_application_run() called, function activate() will be called also

    g_signal_connect(gtkApplication, "activate", G_CALLBACK(activateApplicationCB), this);
}

void Gui::initGui() {
    createMainWindow();
    createLayoutGrid();

    showHomeScreen();
}

int Gui::run() {
    return g_application_run(G_APPLICATION(gtkApplication), argc, argv);
}

void Gui::createMainWindow() {
    mainWindow = gtk_application_window_new(gtkApplication);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Piskvorky");

    g_signal_connect(mainWindow, "destroy", G_CALLBACK(destroyMainWindowCB), this);
}

void Gui::createLayoutGrid() {
    layoutGrid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(mainWindow), layoutGrid);
}

void Gui::createHomeScreen() {
    GtkWidget *box, *image, *optionsBox, *gridSizeOptionsBox, *numberOfCellsInRowToWinOptionsBox, *scale, *label, *button, *buttonBox;
    GSList *group;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
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

    label = gtk_label_new("Number of cells in row to win");
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

    // START BUTTON
    button = gtk_button_new_with_label("START");
    g_signal_connect(button, "clicked", G_CALLBACK(startButtonClickedCB), this);
    buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(buttonBox), button);

    gtk_box_pack_start(GTK_BOX(box), image, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), optionsBox, false, false, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonBox, false, false, 0);

    homeScreen = box;
}

void Gui::createMenuBar() {
    GtkWidget *fileMenu, *fileMenuItem, *newGameMenuItem, *menuBar;

    menuBar = gtk_menu_bar_new();

    fileMenu = gtk_menu_new();
    fileMenuItem = gtk_menu_item_new_with_label("File");
    newGameMenuItem = gtk_menu_item_new_with_label("New game");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuItem), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newGameMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileMenuItem);
    g_signal_connect(newGameMenuItem, "activate", G_CALLBACK(newGameMenuItemActivateCB), this);

    menuBarBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(menuBarBox), menuBar, FALSE, FALSE, 0);
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

            gtk_grid_attach(GTK_GRID(playGrid), overlay, row, col, 1, 1);
        }
    }


}

void Gui::hideAllWidgetsInLayoutGrid() {
    gtk_container_foreach(GTK_CONTAINER(layoutGrid), (GtkCallback) hideWidget, this);
}

void Gui::showHomeScreen() {
    hideAllWidgetsInLayoutGrid();

    createHomeScreen();
    gtk_grid_attach(GTK_GRID(layoutGrid), homeScreen, 0, 0, 1, 1);

    gtk_widget_show_all(mainWindow);
}

void Gui::showPlayGrid() {
    GtkWidget *aspectFrame;

    hideAllWidgetsInLayoutGrid();

    createMenuBar();
    createPlayGrid();

    aspectFrame = gtk_aspect_frame_new(nullptr, 0.5, 0.5, 1, false);
    gtk_container_add(GTK_CONTAINER(aspectFrame), playGrid);

    gtk_grid_attach(GTK_GRID(layoutGrid), menuBarBox, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), aspectFrame, 0, 1, 1, 1);

    gtk_widget_show_all(mainWindow);
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

void Gui::redrawButton(GtkWidget *widget) {
    unsigned long row, col;

    getButtonIndices(widget, &row, &col);

    Cell *cell = gameLogic->getCell(row, col);

    gtk_button_set_label(GTK_BUTTON(widget), cell->getStringValue().c_str());
}

void Gui::hideWidget(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gtk_container_remove(GTK_CONTAINER(gui->layoutGrid), widget);
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

    gui->getButtonIndices(widget, &row, &col);

    result = gui->gameLogic->updateGrid(row, col);

    if (result) {
        gui->redrawButton(widget);

        Player *winner = gui->gameLogic->getWinner();
        if (winner) {
            std::cout << "Congrats " << winner->getName() << std::endl;

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
    }
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

    cairo_set_line_width(cr, 3);

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

    delete gui->gameLogic;
    gui->gameLogic = new GameLogic(gui->gridSizeScaleValue, gui->numberOfCellsInRowToWin);

    gui->showPlayGrid();
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

// Menu items callbacks
void Gui::newGameMenuItemActivateCB(GtkWidget *widget, gpointer data) {
    auto *gui = static_cast<Gui *>(data);

    gui->showHomeScreen();
}
