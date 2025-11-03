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
    
    //layout->addWidget(backBtn, 0, Qt::AlignLeft | Qt::AlignTop); 

    setLayout(layout);

}

void VisualizePage::loadGraph(Graph* d) {
    mainCanvas->loadGraph(d);
}