#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "theme.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
 
#include "menuPage.h"
#include "visualizePage.h"
#include "dataPage.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget);

    menuPage = new MenuPage(this);
    dataPage = new DataPage(this);
    visualizePage = new VisualizePage(this);


    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(dataPage);
    stackedWidget->addWidget(visualizePage);


    //rajouter signaux (events)

    //les signaux de changement de pages sont interpretes 
    //par AppController afin de mieux separer les responsabilites
    connect(menuPage, &MenuPage::switchToPage, this, &MainWindow::requestPageChange);
    connect(visualizePage, &VisualizePage::switchToPage, this, &MainWindow::requestPageChange);
    connect(dataPage, &DataPage::switchToPage, this, &MainWindow::requestPageChange);

    connect(dataPage, &DataPage::confirmNewDatasetFileSelected, this, &MainWindow::confirmNewDatasetFileSelected);
    connect(dataPage, &DataPage::generateNewDataset, this, &MainWindow::generateNewDataset);


    stackedWidget->setCurrentWidget(menuPage);

}

MainWindow::~MainWindow()
{
    delete ui;
    //delete dataPage;
}



void MainWindow::switchToPage(UiPages p) {
    switch (p) {
    case UiPages::menu:
        stackedWidget->setCurrentWidget(menuPage);
        break;

    case UiPages::data:
        stackedWidget->setCurrentWidget(dataPage);

        break;

    case UiPages::visualize:
        stackedWidget->setCurrentWidget(visualizePage);

        break;

    default:
        break;
    }
}



void MainWindow::loadDataset(Dataset* d){
    visualizePage->loadDataset(d);
}