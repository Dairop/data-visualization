 #pragma once 
 
 #include <QOpenGLFunctions>
 #include <QOpenGLWidget>
 #include <QPointF>

 #include <unordered_map>


struct Graph {
    std::unordered_map<int, std::string> nodesNames;
    std::unordered_map<int, int> edges; //oriented
    std::unordered_map<int, QPointF> nodesPosition;
};



class GraphDisplay : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
private:
    Graph myGraph;

    double zoom;
    double x_offset;
    double y_offset;

    QPointF lastMousePos;

public:
    GraphDisplay(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    //void wheelEvent(QWheelEvent *event) override;
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updateCameraPos();
};
