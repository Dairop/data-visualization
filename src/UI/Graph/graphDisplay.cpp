#include "graphDisplay.h"

Graph::Graph(Dataset* d){
    this->nodesNames = d->nodesNames;
    this->edges = d->edges;

    placePointsInCircle();
}



void Graph::placePointsInCircle() {
    const int n_nodes = this->nodesNames.size();

    nodesPosition.clear();

    float TWO_PI = 3.141592f * 2.f;
    int i = 0;
    float angle = 0.0f;
    float radius = std::sqrt(n_nodes) * 80.f;
    for (auto & [start, _] : nodesNames){
        angle = (float) i / (float) n_nodes * TWO_PI;
        QPointF pos(cosf(angle) * radius, sinf(angle) * radius);
        nodesPosition.insert(std::pair<int, QPointF>(start, pos));
        
        i++;
    }
}

GraphDisplay::GraphDisplay(QWidget *parent) : 
        QOpenGLWidget(parent), 
        zoom(1.0), 
        x_offset(0.0), y_offset(0.0) {  
    setFocusPolicy(Qt::StrongFocus); //permet de recuperer les evenements clavier et souris

    setMouseTracking(true);

}

void GraphDisplay::loadDataset(Dataset* d){
    myGraph = Graph(d);
}


void GraphDisplay::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //regles de melanges des couleurs
}

void GraphDisplay::resizeGL(int w, int h) {
    //(0,0) en haut a gauche
    glViewport(0, 0, w, h);

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

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.f);
    glBegin(GL_LINES);
    for (const auto &[startId, endId] : myGraph.edges){
        glVertex2f(myGraph.nodesPosition.at(startId).x(), myGraph.nodesPosition.at(startId).y());
        glVertex2f(myGraph.nodesPosition.at(endId).x(), myGraph.nodesPosition.at(endId).y());
    }
    glEnd();


    const int nodesHalfSize = 15;

    glColor3f(0.2f, 0.6f, 1.0f);
    for (const auto &[id, pos] : myGraph.nodesPosition){
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

    if (deltaY > 0) {
        zoom *= zoomChangeFactor;
    } else {
        zoom /= zoomChangeFactor;
    }

    update();
}
