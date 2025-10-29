#pragma once

#include <filesystem>

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QScrollArea>

#include "mainWindow.h"


class DataPage: public QWidget {
Q_OBJECT

public:
    DataPage(QWidget *parent = nullptr);


signals:
    void switchToPage(UiPages p);
    
};