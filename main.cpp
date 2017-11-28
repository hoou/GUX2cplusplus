#include "Application.h"

int main(int argc, char *argv[]) {
    int status = 0;
    Application application(argc, argv, 16, 5);

    status = application.run();

    return status;
}