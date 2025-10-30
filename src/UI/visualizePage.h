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

private:
    GraphDisplay *mainCanvas;

signals:
    void switchToPage(UiPages p);
    
};