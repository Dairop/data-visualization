#pragma once

#include <QMainWindow>
#include <qstackedwidget.h>

#include <filesystem>
#include "UiPages.h"
#include "graph.h"

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

    void loadGraph(Graph* d);

private:
    Ui::MainWindow *ui; //permet l'utilisation du fichier .ui
    QStackedWidget *stackedWidget; //permet le passage d'une page à l'autre (chaque page est un widget)

    MenuPage *menuPage;
    DataPage *dataPage;
    VisualizePage *visualizePage;

signals:
    void requestPageChange(UiPages p);
    void confirmNewDatasetFileSelected(std::filesystem::path path);
    void generateNewDataset(int nodes, int edges);
    
public slots:
    void switchToPage(UiPages p);
};
