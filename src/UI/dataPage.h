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

private:
    std::string pathOfLocalSelectedFile;
    bool willGenerateNewDataset;

    void onClickBackToMenuButton();
    void onClickConfirmButton();
    void updateLocalSelectedPath(std::string newPath);

signals:
    void switchToPage(UiPages p);
    void confirmNewDatasetFileSelected(std::string path);
    void generateNewDataset(int nodes, int edges);
    
};