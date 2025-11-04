#include "visualizePage.h"

VisualizePage::VisualizePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout();

    mainCanvas = new GraphDisplay(this);
    layout->addWidget(mainCanvas, 1);


    QPushButton *backBtn = new QPushButton("Back", this);
    backBtn->setFixedSize(75, 25);
    backBtn->move(15, 15);
    backBtn->raise();
    connect(backBtn, &QPushButton::clicked, [this]() {emit switchToPage(UiPages::menu); });
    

    QPushButton *circularBtn = new QPushButton("Circular representation", this);
    circularBtn->setFixedSize(150, 25);
    circularBtn->move(15, 45);
    circularBtn->raise();
    //connect(circularBtn, &QPushButton::clicked, [this]() {emit switchToPage(UiPages::menu); });


    QPushButton *forceBtn = new QPushButton("Force directed representation", this);
    forceBtn->setFixedSize(175, 25);
    forceBtn->move(15, 75);
    forceBtn->raise();
    //mainCanvas afin de ne pas perdre le focus
    connect(forceBtn, &QPushButton::clicked, [this]() {emit startApplyingForceDirected(); this->mainCanvas->setFocus();});


    //layout->addWidget(backBtn, 0, Qt::AlignLeft | Qt::AlignTop); 

    setLayout(layout);

}

void VisualizePage::loadGraph(Graph* d) {
    mainCanvas->loadGraph(d);
}