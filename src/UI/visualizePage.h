#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>

#include "UiPages.h"
#include "Graph/graphDisplay.h"


class VisualizePage: public QWidget {
Q_OBJECT

public:
    VisualizePage(QWidget *parent = nullptr);
    void loadGraph(Graph* d);

private:
    GraphDisplay *mainCanvas;

    QProgressBar *progressBar;
    QPushButton *forceBtn;
    QPushButton *circularBtn;
    QPushButton *backBtn;

signals:
    void switchToPage(UiPages p);
    void startApplyingForceDirected();

public slots: 
    void onPositionsUpdated(int iteration, int totalIterations);

};