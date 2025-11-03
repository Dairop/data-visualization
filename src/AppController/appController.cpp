#include "appController.h"

AppController::AppController(QObject *parent): QObject(parent) {
    mainWindow = new MainWindow();
    core = new Core();

    connect(mainWindow, &MainWindow::requestPageChange, this, &AppController::onRequestPageChange);
    //connect(mainWindow, &MainWindow::confirmNewDatasetFileSelected, this, &AppController::onConfirmNewDatasetFileSelected);
    connect(mainWindow, &MainWindow::generateNewDataset, this, &AppController::onGenerateNewDataset);
    connect(mainWindow, &MainWindow::startApplyingForceDirected, this, &AppController::startApplyingForceDirected);
    
    connect(core, &Core::datasetGenerated, this, &AppController::onDatasetLoaded);
    connect(core, &Core::datasetLoaded, this, &AppController::onDatasetLoaded);
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

void AppController::onConfirmNewDatasetFileSelected(std::filesystem::path path){
    core->setNewDatasetPath(path);
}


void AppController::onGenerateNewDataset(int nodes, int edges){
    core->generateNewDataset(nodes, edges);
}

void AppController::onDatasetLoaded(){
    mainWindow->switchToPage(UiPages::menu);
    mainWindow->loadGraph(core->getGraph());
}


void AppController::startApplyingForceDirected(){
    core->startApplyingForceDirected();
}