#include "visualizePage.h"

VisualizePage::VisualizePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    QPushButton *backBtn = new QPushButton("Back", this);
    connect(backBtn, &QPushButton::clicked, [this]() {emit switchToPage(UiPages::menu); });

    layout->addWidget(backBtn);

    mainCanvas = new GraphDisplay(this);

    layout->addWidget(mainCanvas);
}

