#include "dataPage.h"

DataPage::DataPage(QWidget *parent) : QWidget(parent) {
   QWidget *container = new QWidget(); // permet le scroll area

   QVBoxLayout *layout = new QVBoxLayout();
   container->setLayout(layout);

   QScrollArea *scrollArea = new QScrollArea(this);
   scrollArea->setWidgetResizable(true);
   scrollArea->setWidget(container);


   std::string path = "../../../data";
   for (const auto &file : std::filesystem::directory_iterator(path)) {
      std::string filePath = file.path().string();
      std::string displayName = filePath.substr(14, filePath.length() - 14);
      QRadioButton* choice = new QRadioButton(QString::fromStdString(displayName), this);
      
      layout->addWidget(choice);
   }

   QRadioButton* choice = new QRadioButton("Generate new dataset", this);
   choice->setAccessibleName("generateButton");
   layout->addWidget(choice);

   QPushButton *backToMenuButton = new QPushButton("Back to menu");
   QPushButton *confirmButton = new QPushButton("Confirm choice");

   layout->addWidget(backToMenuButton);
   layout->addWidget(confirmButton);





   setStyleSheet(R"(
    QWidget {
        background-color: #2D1A66;
        width: 100%;
    }

    QRadioButton {
        /* l'attribut width simple n'existe pas ??? */
        max-width: 75%;
        min-width: 400px;

        height: 4em;

        border-width: 3px;
        border-color: #7D60D2;
        border-radius: 5px;
        border-style: solid;

        color: white;
    }


    QRadioButton[accessibleName="generateButton"] {
    background-color: blue;
    }

    )");
}
