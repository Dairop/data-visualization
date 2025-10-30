#pragma once
#include "mainwindow.h"
#include "core.h"        

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

    std::string selectedDataset;


public slots:
    void onRequestPageChange(UiPages p);
    void onConfirmNewDatasetFileSelected(std::string path);
    void onGenerateNewDataset(int nodes, int edges);
};
