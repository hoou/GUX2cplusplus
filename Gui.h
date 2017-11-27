//
// Created by hoou on 25.11.17.
//

#ifndef GUX2CPLUSPLUS_GUI_H
#define GUX2CPLUSPLUS_GUI_H

#include <gtk/gtk.h>

class Gui {
private:
    int argc;
    char **argv;

    GtkApplication *application;
    GtkWidget *mainWindow;

    void createMainWindow();

    void initGui();

    // Callbacks
    static void activateApplicationCB(GtkApplication *app, gpointer data);
    static void destroyMainWindowCB(GtkWidget *widget, gpointer data);
public:
    Gui(int argc, char *argv[]);

    int run();
};


#endif //GUX2CPLUSPLUS_GUI_H
