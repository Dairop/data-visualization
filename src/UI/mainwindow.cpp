#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "theme.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
 


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget);

    menuPage = new MenuPage(this);
    //dataPage = new DataPage(this);


    stackedWidget->addWidget(menuPage);
    //stackedWidget->addWidget(dataPage);


    //rajouter signaux (events)



    stackedWidget->setCurrentWidget(menuPage);
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete dataPage;
}
