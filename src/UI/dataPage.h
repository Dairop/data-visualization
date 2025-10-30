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
    std::filesystem::path pathOfLocalSelectedFile;
    bool willGenerateNewDataset;

    void onClickBackToMenuButton();
    void onClickConfirmButton();
    void updateLocalSelectedPath(std::filesystem::path newPath);

signals:
    void switchToPage(UiPages p);
    void confirmNewDatasetFileSelected(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);
    
};