#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "UiPages.h"
#include "Graph/graphDisplay.h"


class VisualizePage: public QWidget {
Q_OBJECT

public:
    VisualizePage(QWidget *parent = nullptr);
    void loadGraph(Graph* d);

private:
    GraphDisplay *mainCanvas;

signals:
    void switchToPage(UiPages p);
    void startApplyingForceDirected();
    
};