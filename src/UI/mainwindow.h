#pragma once

#include <QMainWindow>
#include <qstackedwidget.h>
#include "menuPage.h"

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui; //permet l'utilisation du fichier .ui
    QStackedWidget *stackedWidget; //permet le passage d'une page à l'autre (chaque page est un widget)

    MenuPage *menuPage;
    //DataPage *dataPaage;
};
