#include "visualizePage.h"

VisualizePage::VisualizePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout();

    mainCanvas = new GraphDisplay(this);
    layout->addWidget(mainCanvas, 1);


    backBtn = new QPushButton("Back", this);
    backBtn->setFixedSize(75, 25);
    backBtn->move(15, 15);
    backBtn->raise();
    connect(backBtn, &QPushButton::clicked, [this]() {emit switchToPage(UiPages::menu); });
    

    circularBtn = new QPushButton("Circular representation", this);
    circularBtn->setFixedSize(150, 25);
    circularBtn->move(15, 45);
    circularBtn->raise();
    //connect(circularBtn, &QPushButton::clicked, [this]() {emit switchToPage(UiPages::menu); });


    forceBtn = new QPushButton("Force directed representation", this);
    forceBtn->setFixedSize(175, 25);
    forceBtn->move(15, 75);
    forceBtn->raise();
    //mainCanvas afin de ne pas perdre le focus
    connect(forceBtn, &QPushButton::clicked, [this]() {
        progressBar->setVisible(true);
        progressBar->setValue(0);
        emit startApplyingForceDirected(); 
        this->mainCanvas->setFocus();
    });
    

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setVisible(false);
    layout->addWidget(progressBar);


    //layout->addWidget(backBtn, 0, Qt::AlignLeft | Qt::AlignTop); 

    setLayout(layout);

}

void VisualizePage::loadGraph(Graph* d) {
    mainCanvas->loadGraph(d);
}


void VisualizePage::onPositionsUpdated(int iteration, int totalIterations) {
    if (totalIterations > 0) {
        int progress = (iteration * 100) / totalIterations;
        progressBar->setValue(progress);
        progressBar->setVisible(progress < 100);
    }
    mainCanvas->update();
}