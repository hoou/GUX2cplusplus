//
// Created by hoou on 25.11.17.
//

#include "Gui.h"

Gui::Gui(int argc, char *argv[]) : argc(argc), argv(argv) {

    application = gtk_application_new("cz.vutbr.fit.stud.xmikit01.piskvorky", G_APPLICATION_FLAGS_NONE);

    // When g_application_run() called, function activate() will be called also

    g_signal_connect(application, "activate", G_CALLBACK(activateApplicationCB), this);
}

void Gui::initGui() {
    createMainWindow();
}

void Gui::createMainWindow() {
    mainWindow = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "Piskvorky");
    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 200, 200);

    // Set minimum size of the main window
    GdkGeometry hints{};
    hints.min_width = 200;
    hints.min_height = 200;
    gtk_window_set_geometry_hints(GTK_WINDOW(mainWindow), mainWindow, &hints, GDK_HINT_MIN_SIZE);

    g_signal_connect(mainWindow, "destroy", G_CALLBACK(destroyMainWindowCB), this);
}

// CALLBACKS

void Gui::destroyMainWindowCB(GtkWidget *widget, gpointer data) {
    g_application_quit(G_APPLICATION(((Gui *) data)->application));
}

void Gui::activateApplicationCB(GtkApplication *app, gpointer data) {
    ((Gui *) data)->initGui();
    gtk_widget_show_all(((Gui *) data)->mainWindow);
}

int Gui::run() {
    return g_application_run(G_APPLICATION(application), argc, argv);
}
