#include "gtproject.h"
#include <QtAlgorithms>

GTProject::GTProject(QString projectPath, int theme, QWidget *parent)
    : QWidget(parent), m_sProjectPath(projectPath)
{
    m_pvProjectFiles = new QVector<GTFile*>;
    QDir projectDir(projectPath);
    QStringList listFiles = projectDir.entryList(QDir::Files);
    foreach(auto oneFile, listFiles){
        m_pvProjectFiles->push_back(new GTFile(projectDir.absoluteFilePath(oneFile), theme));
    }
    findProjectName();
    if(m_pvProjectFiles->count() > 0){
        m_pCurrentFile = m_pvProjectFiles->at(0);
    }
    else
        m_pCurrentFile = nullptr;

}

//Геттеры и сеттеры
QVector<GTFile*>* GTProject::getProjectFiles(){
    return m_pvProjectFiles;
}

GTFile* GTProject::getCurrentFile(){return m_pCurrentFile;}
void GTProject::setCurrentFile(GTFile* file){m_pCurrentFile = file;}

QString GTProject::getProjectPath(){return m_sProjectPath;}
void GTProject::setProjectPath(QString path){m_sProjectPath = path;}

QString GTProject::getProjectName(){return m_sProjectName;}
void GTProject::setProjectName(QString name){m_sProjectName = name;}

QString GTProject::getCompiledProgramPath(){return m_sCompiledProgramPath;}
void GTProject::setCompiledProgramPath(QString path){m_sCompiledProgramPath = path;}

//Методы
void GTProject::findProjectName(){
    int index = m_sProjectPath.lastIndexOf('/');
    m_sProjectName = m_sProjectPath.mid(index + 1);
}

void GTProject::addFile(GTFile* newFile){
     m_pvProjectFiles->push_back(newFile);
     if(m_nTheme == 0)
         m_pvProjectFiles->back()->setLightTheme();
     else if(m_nTheme == 1)
         m_pvProjectFiles->back()->setDarkTheme();
}

void GTProject::deleteCurrentFile(){
    for(int i = 0; i < m_pvProjectFiles->size(); i++){
        if(m_pCurrentFile == m_pvProjectFiles->at(i)){
            m_pCurrentFile->deleteConnectedFile();
            m_pvProjectFiles->removeAt(i);
            delete m_pCurrentFile;
            if(i < m_pvProjectFiles->size()){
                m_pCurrentFile = m_pvProjectFiles->at(i);
            }
            else if(i==0){
                m_pCurrentFile = nullptr;
            }
            else{
                m_pCurrentFile = m_pvProjectFiles->at(i - 1);
            }
            break;
        }
    }
}

int GTProject::countFiles(){
    return m_pvProjectFiles->size();
}

void GTProject::saveProject(){
    for(int i = 0; i < m_pvProjectFiles->size(); i++)
        m_pvProjectFiles->at(i)->saveFile();
}

//методы для смены темы
void GTProject::setLightTheme(){
    for(int i = 0; i < m_pvProjectFiles->size(); i++)
        m_pvProjectFiles->at(i)->setLightTheme();
    m_nTheme = LightTheme;
}

void GTProject::setDarkTheme(){
    for(int i = 0; i < m_pvProjectFiles->size(); i++)
        m_pvProjectFiles->at(i)->setDarkTheme();
    m_nTheme = DarkTheme;
}

void GTProject::setLightHighlighter(){
    for(int i = 0; i < m_pvProjectFiles->size(); ++i)
        m_pvProjectFiles->at(i)->setLightHighlighter();
}

void GTProject::setDarkHighlighter(){
    for(int i = 0; i < m_pvProjectFiles->size(); ++i)
        m_pvProjectFiles->at(i)->setDarkHighlighter();
}

//Деструктор
GTProject::~GTProject(){
    qDeleteAll(*m_pvProjectFiles);
    m_pvProjectFiles->clear();

    delete m_pvProjectFiles;
}

