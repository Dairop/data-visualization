 #pragma once 
 
 #include <QOpenGLFunctions>
 #include <QOpenGLWidget>

class GraphDisplay : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    GraphDisplay(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
