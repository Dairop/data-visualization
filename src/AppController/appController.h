#pragma once
#include "mainwindow.h"  // from UI
#include "core.h"         // from Core
#include "theme.h"        // from Theme


class AppController {
public:
    AppController();
    void run();

private:
    MainWindow mainWindow;
};
