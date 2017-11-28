#include "Application.h"

int main(int argc, char *argv[]) {
    int status = 0;
    Application application(argc, argv);

    status = application.run();

    return status;
}