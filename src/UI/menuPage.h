#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <iostream>

#include "UiPages.h"


class MenuPage: public QWidget {
Q_OBJECT 

public:
    MenuPage(QWidget *parent = nullptr);

private:
    void onVisualizeClicked();
    void onLoadDataClicked();


signals:
    void switchToPage(UiPages p);
};