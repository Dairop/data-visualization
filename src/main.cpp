#include <QApplication>
#include "AppController/AppController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppController controller;
    controller.run();

    return app.exec();
}
