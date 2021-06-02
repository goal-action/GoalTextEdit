#include "gtstackedtextedits.h"

GTStackedTextEdits::GTStackedTextEdits(QWidget* parent)
    : QStackedWidget(parent)
{

}

//Методы
void GTStackedTextEdits::removeCurrentTextEdit(){
    int numberOfElements = m_vAllFilesOfAddedTextEdits.size();
    for(int i = 0; i < numberOfElements; i++){
        if(m_vAllFilesOfAddedTextEdits.at(i)->getTextEdit() == currentWidget()){
            removeWidget(m_vAllFilesOfAddedTextEdits.at(i)->getTextEdit());
            m_vAllFilesOfAddedTextEdits.removeAt(i);//Удаляем также указатель на файл из вектора
            break;
        }
    }
}


//методы для смены темы
void GTStackedTextEdits::setLightTheme(){
    QFile stacketTextFile(":/styles/stackedTextEdits/lightTheme/stackedTextEdits.css");
    stacketTextFile.open(QIODevice::ReadOnly);
    setStyleSheet(stacketTextFile.readAll());
    stacketTextFile.close();
}

void GTStackedTextEdits::setDarkTheme(){
    QFile stacketTextFile(":/styles/stackedTextEdits/darkTheme/stackedTextEdit.css");
    stacketTextFile.open(QIODevice::ReadOnly);
    setStyleSheet(stacketTextFile.readAll());
    stacketTextFile.close();
}


//Слоты
void GTStackedTextEdits::addTextEditSlot(GTFile* fileOfTextEditToAdd){
    addWidget(fileOfTextEditToAdd->getTextEdit());
    m_vAllFilesOfAddedTextEdits.push_back(fileOfTextEditToAdd);
}

void GTStackedTextEdits::openProjectSlot(const QString& projectName, QVector<GTFile*>* allProjectFiles){
    for(int i = 0; i < allProjectFiles->size(); i++){
        addWidget(allProjectFiles->at(i)->getTextEdit());
        m_vAllFilesOfAddedTextEdits.push_back(allProjectFiles->at(i));
    }
}

void GTStackedTextEdits::newFileSlot(GTFile* newFile){
    addWidget(newFile->getTextEdit());
    m_vAllFilesOfAddedTextEdits.push_back(newFile);
    setCurrentWidget(newFile->getTextEdit());
}

void GTStackedTextEdits::closeProjectSlot(){
    m_vAllFilesOfAddedTextEdits.clear();
    int count = this->count();
    for(int i = count - 1; i >= 0; i--){
        QWidget* w = widget(i);
        removeWidget(w);
        w->deleteLater();
    }
}

void GTStackedTextEdits::setCurrentTextEditSlot(const QString& fileNameOfTextEdit){
    for(int i = 0; i < m_vAllFilesOfAddedTextEdits.size(); i++){
        if(m_vAllFilesOfAddedTextEdits.at(i)->getFileName() == fileNameOfTextEdit){
            setCurrentWidget(m_vAllFilesOfAddedTextEdits.at(i)->getTextEdit());
            break;
        }
    }
}


//Деструктор
GTStackedTextEdits::~GTStackedTextEdits(){

}
