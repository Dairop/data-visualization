#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPointF>
#include <QWheelEvent>

#include <unordered_map>

#include "dataset.h"
#include "graph.h"



class GraphDisplay : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
private:
    Graph *myGraph;

    double zoom;
    double x_offset;
    double y_offset;

    //taille canvas
    int glSizeX;
    int glSizeY;

    QPointF lastMousePos;

public:
    GraphDisplay(QWidget *parent = nullptr);
    
    void loadGraph(Graph* d);

protected:    
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updateCameraPos();


};
