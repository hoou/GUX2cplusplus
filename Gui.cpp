#include <iostream>
#include "Gui.h"

Gui::Gui(int argc, char **argv, GameLogic *gameLogic) : argc(argc), argv(argv), gameLogic(gameLogic) {

    gtkApplication = gtk_application_new("cz.vutbr.fit.stud.xmikit01.piskvorky", G_APPLICATION_FLAGS_NONE);

    // When g_application_run() called, function activate() will be called also

    g_signal_connect(gtkApplication, "activate", G_CALLBACK(activateApplicationCB), this);
}

void Gui::initGui() {
    createMainWindow();
    createPlayGrid();
}

int Gui::run() {
    return g_application_run(G_APPLICATION(gtkApplication), argc, argv);
}

void Gui::createMainWindow() {
    mainWindow = gtk_application_window_new(gtkApplication);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Piskvorky");

    int minWindowSize = MIN_BUTTON_SIZE * gameLogic->getGridSize();

    gtk_window_set_default_size(GTK_WINDOW(mainWindow), minWindowSize, minWindowSize);

    // Set minimum size of the main window and aspect ratio to 1:1
    GdkGeometry hints{};
    hints.min_width = hints.min_height = minWindowSize;
    hints.min_aspect = hints.max_aspect = 1;

    gtk_window_set_geometry_hints(GTK_WINDOW(mainWindow), mainWindow, &hints, GDK_HINT_ASPECT);

    g_signal_connect(mainWindow, "destroy", G_CALLBACK(destroyMainWindowCB), this);
}

void Gui::createPlayGrid() {
    GtkWidget *button, *overlay, *label;

    playGrid = gtk_grid_new();

    gtk_grid_set_column_homogeneous(GTK_GRID(playGrid), true);
    gtk_grid_set_row_homogeneous(GTK_GRID(playGrid), true);

    gtk_container_add(GTK_CONTAINER(mainWindow), playGrid);

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
