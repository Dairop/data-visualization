#pragma once
#include "mainwindow.h"
#include "core.h"    
#include <filesystem>    

#include <QObject>


class AppController : public QObject {
    Q_OBJECT

public:
    AppController(QObject *parent = nullptr);
    ~AppController();

    void run();

private:
    Core *core;
    MainWindow *mainWindow;

    std::filesystem::path selectedDataset;


public slots:
    void onRequestPageChange(UiPages p);
    void onConfirmNewDatasetFileSelected(std::filesystem::path path);
    void onGenerateNewDataset(int nodes, int edges);
    void onDatasetLoaded();
    void startApplyingForceDirected();
    void onPositionsUpdated(int iteration, int totalIterations);
    void onFinishedApplyingForceDirected();
    void requestStopCurrentTask();
    void resetToCircularRepresentation();
    void requestMoveNode(int nodeId, QPointF& newPosition);

};
