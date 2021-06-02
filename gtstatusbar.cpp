#include "gtstatusbar.h"

GTStatusBar::GTStatusBar(QWidget *parent)
    : QWidget(parent)
{
    m_pStatus = new QLabel;

    //Layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_pStatus);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

//Методы
void GTStatusBar::setStatus(QString newStatus){
    m_pStatus->setText(newStatus);
}

//методы для смены темы
void GTStatusBar::setLightTheme(){
    QFile file(":/styles/statusBar/lightTheme/statusBar.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
}

void GTStatusBar::setDarkTheme(){
    QFile file(":/styles/statusBar/darkTheme/statusBar.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
}

//Деструктор
GTStatusBar::~GTStatusBar(){

}
