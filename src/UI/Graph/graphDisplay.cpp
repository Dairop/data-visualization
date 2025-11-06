#include "graphDisplay.h"


GraphDisplay::GraphDisplay(QWidget *parent) : 
        QOpenGLWidget(parent), 
        zoom(1.0), 
        x_offset(0.0), y_offset(0.0) {  
    setFocusPolicy(Qt::StrongFocus); //permet de recuperer les evenements clavier et souris

    setMouseTracking(true);

    myGraph = nullptr;
}

void GraphDisplay::loadGraph(Graph* g){
    myGraph = g;
}


void GraphDisplay::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //regles de melanges des couleurs

    //get current viewport size
    GLint* params = nullptr;
    glGetIntegerv(GL_VIEWPORT, params);
    if (params != nullptr){
        glSizeX = params[2];
        glSizeY = params[3];
    }
}

void GraphDisplay::resizeGL(int w, int h) {
    //(0,0) en haut a gauche
    glViewport(0, 0, w, h);
    glSizeX = w;
    glSizeY = h;

    //indique que l'on va modifier la projection
    glMatrixMode(GL_PROJECTION);
    //reinitialise la matrice
    glLoadIdentity();
    //projection orthographique = sans perspective
    glOrtho(0, w, h, 0, -1, 1);

    //indique que l'on va modifier la matrice modelisation vue
    // = position et orientation des elements
    glMatrixMode(GL_MODELVIEW);
    //reinitialise la matrice
    glLoadIdentity();
}

void GraphDisplay::updateCameraPos() {
    glScaled(zoom, zoom, 1.0);
    glTranslated(x_offset, y_offset, 0.0);
}

void GraphDisplay::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    updateCameraPos();

    if (myGraph == nullptr) return;

    std::unordered_map<int, QPointF> positionsCopy;
    std::unordered_map<int, std::vector<int>> edgesCopy;
    {
        std::lock_guard<std::mutex> lock(myGraph->positionMutex);
        positionsCopy = myGraph->nodesPosition;
        edgesCopy = myGraph->edges;
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.f);
    glBegin(GL_LINES);
    for (const auto &[startId, edgesFromStart] : myGraph->edges){
        for (const int endId : edgesFromStart){
            glVertex2f(myGraph->nodesPosition.at(startId).x(), myGraph->nodesPosition.at(startId).y());
            glVertex2f(myGraph->nodesPosition.at(endId).x(), myGraph->nodesPosition.at(endId).y());
        }
    }
    glEnd();


    const int nodesHalfSize = 15;

    glColor3f(0.2f, 0.6f, 1.0f);
    for (const auto &[id, pos] : myGraph->nodesPosition){
        glBegin(GL_QUADS);
            glVertex2f(pos.x() - nodesHalfSize, pos.y() - nodesHalfSize);
            glVertex2f(pos.x() + nodesHalfSize, pos.y() - nodesHalfSize);
            glVertex2f(pos.x() + nodesHalfSize, pos.y() + nodesHalfSize);
            glVertex2f(pos.x() - nodesHalfSize, pos.y() + nodesHalfSize);
        glEnd();
    }
    
}


void GraphDisplay::wheelEvent(QWheelEvent *event) {
    const float zoomChangeFactor = 1.15f;
    const int deltaY = event->angleDelta().y();

    const float oldZoom = zoom;

    QPointF mousePos = event->position();

    //used to center the zoom on the mouse, not top left
    float worldX_before = (mousePos.x() / oldZoom) - x_offset;
    float worldY_before = (mousePos.y() / oldZoom) - y_offset;

    if (deltaY > 0 && zoom < 3000.0) {
        zoom *= zoomChangeFactor;
    } else if (deltaY < 0 && zoom > 0.01) {
        zoom /= zoomChangeFactor;
    } else {
        return;
    }


    float worldX_after = (mousePos.x() / zoom) - x_offset;
    float worldY_after = (mousePos.y() / zoom) - y_offset;

    x_offset += (worldX_after - worldX_before);
    y_offset += (worldY_after - worldY_before);

    update();
}



void GraphDisplay::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void GraphDisplay::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF delta = event->pos() - lastMousePos;
        x_offset += delta.x() / zoom;
        y_offset += delta.y() / zoom;
        lastMousePos = event->pos();
        update();
    }
}