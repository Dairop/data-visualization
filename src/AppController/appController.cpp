#include "appController.h"

AppController::AppController(QObject *parent): QObject(parent) {
    mainWindow = new MainWindow();

    connect(mainWindow, &MainWindow::requestPageChange, this, &AppController::onRequestPageChange);
}

AppController::~AppController(){
    delete mainWindow;
}

void AppController::run() {
    mainWindow->show();
}


void AppController::onRequestPageChange(UiPages p){
    mainWindow->switchToPage(p);
}