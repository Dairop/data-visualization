#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "theme.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
 
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupContainers();
}

void MainWindow::setupContainers()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QFrame *container1 = new QFrame();
    container1->setFrameShape(QFrame::Box);
    container1->setStyleSheet("background: white; padding: 10px;");
    QLabel *label1 = new QLabel(Core::getText1());
    QVBoxLayout *layout1 = new QVBoxLayout(container1);
    layout1->addWidget(label1);

    QFrame *container2 = new QFrame();
    container2->setFrameShape(QFrame::Box);
    container2->setStyleSheet("background: white; padding: 10px;");
    QLabel *label2 = new QLabel(Core::getText2());
    QVBoxLayout *layout2 = new QVBoxLayout(container2);
    layout2->addWidget(label2);

    layout->addWidget(container1);
    layout->addWidget(container2);

    central->setStyleSheet("background-color: " + Theme::backgroundColor() + ";");
    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
    delete ui;
}
