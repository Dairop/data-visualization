#include "appController.h"

AppController::AppController(QObject *parent): QObject(parent) {
    mainWindow = new MainWindow();
    core = new Core();

    connect(mainWindow, &MainWindow::requestPageChange, this, &AppController::onRequestPageChange);
    connect(mainWindow, &MainWindow::confirmNewDatasetFileSelected, this, &AppController::onConfirmNewDatasetFileSelected);
    connect(mainWindow, &MainWindow::generateNewDataset, this, &AppController::onRequestPageChange);
    
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

void AppController::onConfirmNewDatasetFileSelected(std::string path){
    core->setNewDatasetPath(path);
}


void AppController::onGenerateNewDataset(int nodes, int edges){
    core->generateNewDataset(nodes, edges);
}