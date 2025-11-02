#include "graphDisplay.h"


GraphDisplay::GraphDisplay(QWidget *parent) : 
    QOpenGLWidget(parent), 
    zoom(1.0), 
    x_offset(0.0), y_offset(0.0) {  
        setFocusPolicy(Qt::StrongFocus); //permet de recuperer les evenements clavier et souris

        setMouseTracking(true);

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

    //grille
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_LINES);
    for (int x = -1000; x <= 1000; x += 50) {
        glVertex2f(x, -1000);
        glVertex2f(x, 1000);
    }
    for (int y = -1000; y <= 1000; y += 50) {
        glVertex2f(-1000, y);
        glVertex2f(1000, y);
    }
    glEnd();

    //carre a l'origine
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-25.0f, -25.0f);
        glVertex2f(25.0f, -25.0f);
        glVertex2f(25.0f, 25.0f);
        glVertex2f(-25.0f, 25.0f);
    glEnd();
}