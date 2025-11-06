#include "appController.h"

AppController::AppController(QObject *parent): QObject(parent) {
    mainWindow = new MainWindow();
    core = new Core();

    connect(mainWindow, &MainWindow::requestPageChange, this, &AppController::onRequestPageChange);
    //connect(mainWindow, &MainWindow::confirmNewDatasetFileSelected, this, &AppController::onConfirmNewDatasetFileSelected);
    connect(mainWindow, &MainWindow::generateNewDataset, this, &AppController::onGenerateNewDataset);
    connect(mainWindow, &MainWindow::startApplyingForceDirected, this, &AppController::startApplyingForceDirected);
    connect(mainWindow, &MainWindow::requestStopCurrentTask, this, &AppController::requestStopCurrentTask);
    connect(mainWindow, &MainWindow::resetToCircularRepresentation, this, &AppController::resetToCircularRepresentation);

    connect(core, &Core::datasetGenerated, this, &AppController::onDatasetLoaded);
    connect(core, &Core::datasetLoaded, this, &AppController::onDatasetLoaded);
    connect(core, &Core::positionsUpdated, this, &AppController::onPositionsUpdated);
    connect(core, &Core::finishedApplyingForceDirected, this, &AppController::onFinishedApplyingForceDirected);
}

AppController::~AppController(){
    delete mainWindow;
}

void AppController::run() {
    mainWindow->show();
}


void AppController::onRequestPageChange(UiPages p){
    if (core->isThreadRunning() && p != UiPages::visualize) {
        core->stopApplyingForceDirected();
    }

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


void AppController::onPositionsUpdated(int iteration, int totalIterations){
    mainWindow->onPositionsUpdated(iteration, totalIterations);
}


void AppController::onFinishedApplyingForceDirected(){
    mainWindow->finishedApplyingForceDirected();
}


void AppController::requestStopCurrentTask(){
    if (core->isThreadRunning()){
        core->stopApplyingForceDirected();
    }
}


void AppController::resetToCircularRepresentation(){
    requestStopCurrentTask();
    core->placePointsInCircle();
}