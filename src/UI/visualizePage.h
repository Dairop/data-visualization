#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "UiPages.h"


class VisualizePage: public QWidget {
Q_OBJECT

public:
    VisualizePage(QWidget *parent = nullptr);

signals:
    void switchToPage(UiPages p);
    
};