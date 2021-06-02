#include "gtfile.h"

GTFile::GTFile(const QString path, int theme, QWidget* parent)
    : QWidget(parent), m_sFilePath(path)
{
    m_pFile = new QFile(m_sFilePath ,this);
    m_pTextEdit = new QTextEdit(this);
    m_pTextEdit->setTabStopDistance(40);
    if(theme == LightTheme){
        m_pHighlighter = new GTTextLightHighlighter(m_pTextEdit->document());
        m_nTheme = LightTheme;
    }
    else if(theme == DarkTheme){
        m_pHighlighter = new GTTextDarkHighlighter(m_pTextEdit->document());
        m_nTheme = DarkTheme;
    }
    m_bIsSaved = true;
    if(m_pFile->exists()){
        m_pFile->open(QIODevice::ReadOnly);
        m_pTextEdit->setText(m_pFile->readAll());
        m_pFile->close();
    }else{
        m_pFile->open(QIODevice::WriteOnly);
        m_pFile->close();
    }
    findFileName();

    connect(m_pTextEdit, SIGNAL(textChanged()), this, SLOT(fileIsNotSavedSlot()));
}

//Геттеры и сеттеры
QString GTFile::getFilePath(){return m_sFilePath;}
void GTFile::setFilePath(QString path){m_sFilePath = path;}

QString GTFile::getFileName(){return m_sFileName;}
void GTFile::setFileName(QString name){m_sFileName = name;}

QTextEdit* GTFile::getTextEdit(){return m_pTextEdit;}
void GTFile::setTextEdit(QTextEdit* textEdit){
    if(m_pTextEdit)
        delete m_pTextEdit;
    m_pTextEdit = textEdit;
}

QFile* GTFile::getFile(){return m_pFile;}
void GTFile::setFile(QFile* file){
    if(m_pFile)
        delete m_pFile;
    m_pFile = file;
}

//Методы
void GTFile::findFileName(){
    int index = m_sFilePath.lastIndexOf('/');
    m_sFileName = m_sFilePath.mid(index + 1);
}

bool GTFile::isSaved(){
    return m_bIsSaved;
}

void GTFile::saveFile(){
    m_pFile->open(QIODevice::WriteOnly);
    m_pFile->write(m_pTextEdit->toPlainText().toUtf8());
    m_pFile->close();
    m_bIsSaved = true;
}

void GTFile::deleteConnectedFile(){
    m_pFile->remove();
}


//методы для смены темы
void GTFile::setLightTheme(){
    QFile file(":/styles/textEdit/lightTheme/textEdit.css");
    file.open(QIODevice::ReadOnly);
    m_pTextEdit->setStyleSheet(file.readAll());
    file.close();
}

void GTFile::setDarkTheme(){
    QFile file(":/styles/textEdit/darkTheme/textEdit.css");
    file.open(QIODevice::ReadOnly);
    m_pTextEdit->setStyleSheet(file.readAll());
    file.close();
}

void GTFile::setLightHighlighter(){
    delete m_pHighlighter;
    m_pHighlighter = new GTTextLightHighlighter(m_pTextEdit->document());
}

void GTFile::setDarkHighlighter(){
    delete m_pHighlighter;
    m_pHighlighter = new GTTextDarkHighlighter(m_pTextEdit->document());
}

//Слоты
void GTFile::fileIsNotSavedSlot(){
    m_bIsSaved = false;
    emit fileIsNotSavedSignal();
}

//Деструктор
GTFile::~GTFile(){

}
