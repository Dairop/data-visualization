#include "dataPage.h"

DataPage::DataPage(QWidget *parent) : QWidget(parent) {
   pathOfLocalSelectedFile = "";
   willGenerateNewDataset = false;
   
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

      //possible de faire sans fonctions lambda ?
      //impossible de passer des parametres avec la version utilisant les signatures de fonctions
      connect(choice, &QRadioButton::clicked, this, [filePath, this]{ this->pathOfLocalSelectedFile = filePath; this->willGenerateNewDataset = false;});      

      layout->addWidget(choice);
   }

   QRadioButton* choice = new QRadioButton("Generate new dataset", this);
   choice->setAccessibleName("generateButton");
   layout->addWidget(choice);
   connect(choice, &QRadioButton::clicked, this, [this]{ this->willGenerateNewDataset = true; });      


   QPushButton *backToMenuButton = new QPushButton("Back to menu");
   QPushButton *confirmButton = new QPushButton("Confirm choice");

   layout->addWidget(backToMenuButton);
   layout->addWidget(confirmButton);


   connect(backToMenuButton, &QPushButton::clicked, this, &DataPage::onClickBackToMenuButton);
   connect(confirmButton, &QPushButton::clicked, this, &DataPage::onClickConfirmButton);




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



void DataPage::onClickBackToMenuButton(){
   emit switchToPage(UiPages::menu);
}


void DataPage::onClickConfirmButton(){
   if (willGenerateNewDataset) {
      emit generateNewDataset(1000, 6100);
   }

   //send selected file path to core to check
   if (pathOfLocalSelectedFile != ""){
      emit confirmNewDatasetFileSelected(pathOfLocalSelectedFile);
   }

   //if valid
   //emit switchToPage(UiPages::menu);
}
