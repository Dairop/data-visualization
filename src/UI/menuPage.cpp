#include "menuPage.h"

MenuPage::MenuPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    //contenu
    QPushButton *boutonVisualiser = new QPushButton("Visualiser");
    QPushButton *boutonDonnees = new QPushButton("Charger des données");
    QPushButton *boutonQuitter = new QPushButton("Quitter");

    boutonQuitter->setAccessibleName("bQuitter");

    layout->addWidget(boutonVisualiser);
    layout->addWidget(boutonDonnees);
    layout->addWidget(boutonQuitter);


    //variables generees avec https://mywebtools.biagi.org/colors/palette/ , placees
    //a la main dans le code car QSS ne supporte pas les variables de couleurs comme CSS le fait
    /*
    --primary-color: #7D60D2;
    --accentuation-color: #E72653;
    --base-color: #BDB7CC;
    --darker-primary-color: #2D1A66;
    --lighter-primary-color: #EBEAED;
    --light: #F8F6FE;
    --dark: #05020D;
    */


    setStyleSheet(R"(
    QWidget {
        background-color: #2D1A66;
    }

    QPushButton {
        background-color: #7D60D2;
        color: white;
        margin: 5px auto;
        /* l'attribut width simple n'existe pas ??? */
        max-width: 75%;
        min-width: 400px;
        height: 4em;
        border-width: 3px;
        border-color: #7D60D2;
        border-radius: 5px;
        border-style: solid;
        font-size: 16px;
    }

    QPushButton:hover {
        background-color: #F8F6FE;
        color: #E72653;
        border-color: #E72653;
    }


    QPushButton[accessibleName="bQuitter"] {
        background-color: #BDB7CC;
        color: #2D1A66;
        /* max-width: 50px; */
    }

    QPushButton[accessibleName="bQuitter"]:hover {
        background-color: #F8F6FE;
        color: #2D1A66;
        border-color: #2D1A66;
    }

    )");


}