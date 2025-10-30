#pragma once

#include <QMainWindow>
#include <qstackedwidget.h>

#include "UiPages.h"

namespace Ui {
    class MainWindow;
}

class MenuPage;
class DataPage;
class VisualizePage;

class MainWindow : public QMainWindow {
    Q_OBJECT  //objet Qt, permet l'utilisation de slots par exemple

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui; //permet l'utilisation du fichier .ui
    QStackedWidget *stackedWidget; //permet le passage d'une page à l'autre (chaque page est un widget)

    MenuPage *menuPage;
    DataPage *dataPage;
    VisualizePage *visualizePage;

signals:
    void requestPageChange(UiPages p);
    void confirmNewDatasetFileSelected(std::string path);
    void generateNewDataset(int nodes, int edges);

public slots:
    void switchToPage(UiPages p);
};
