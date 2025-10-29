#pragma once
#include "mainwindow.h"  // from UI
#include "core.h"         // from Core
#include "theme.h"        // from Theme

#include <QObject>


class AppController : public QObject {
    Q_OBJECT

public:
    AppController(QObject *parent = nullptr);
    ~AppController();

    void run();

private:
    MainWindow *mainWindow;

    std::string selectedDataset;


public slots:
    void onRequestPageChange(UiPages p);
};
