#include "gtsidebar.h"

GTSideBar::GTSideBar(QWidget *parent)
    : QWidget(parent)
{
    QLabel* lblProject = new QLabel("Opened project:");
    m_plblProjectName = new QLabel("");
    m_pListNameOfFileWidget = new QListWidget(this);

    //Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lblProject);
    layout->addWidget(m_plblProjectName);
    layout->addWidget(m_pListNameOfFileWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    connect(m_pListNameOfFileWidget, SIGNAL(itemSelectionChanged()), this, SLOT(changedCurrentItemSlot()));
}

//Методы
void GTSideBar::setProjectName(const QString& newName){
    m_plblProjectName->setText(newName);
}

void GTSideBar::deleteCurrentFile(){
    int index = m_pListNameOfFileWidget->currentRow();
    if(index == 0 && m_pListNameOfFileWidget->count() == 1)
        m_pListNameOfFileWidget->clear();
    else
        m_pListNameOfFileWidget->takeItem(index);
}


//Методы для смены темы
void GTSideBar::setLightTheme(){
    QFile file(":/styles/sideBar/lightTheme/sideBar.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
}

void GTSideBar::setDarkTheme(){
    QFile file(":/styles/sideBar/darkTheme/sideBar.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
}

//Слоты
void GTSideBar::changedCurrentItemSlot(){
    QListWidgetItem* currentListWidjetItem = m_pListNameOfFileWidget->currentItem();
    if(m_pListNameOfFileWidget->selectionMode() != QAbstractItemView::NoSelection)
        emit changedCurrentItemSignal(currentListWidjetItem->text());
}

void GTSideBar::addItemSlot(const QString& itemName){
    if(itemName.endsWith(".cpp")){
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(QIcon(":/images/fileTypes/cppFile.png"), itemName));
    }else if(itemName.endsWith(".h")){
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(QIcon(":/images/fileTypes/headerFile.png"), itemName));
    }else{
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(itemName));
    }
}

void GTSideBar::openProjectSlot(const QString& projectName, QVector<GTFile*>* allProjectFiles){
    m_pListNameOfFileWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_plblProjectName->setText(projectName);
    for(int i = 0; i < allProjectFiles->size(); i++){
        addItemSlot(allProjectFiles->at(i)->getFileName());
    }
    if(m_pListNameOfFileWidget->count() > 0)
        m_pListNameOfFileWidget->setCurrentRow(0);
}

void GTSideBar::newFileSlot(GTFile* newFile){
    if(newFile->getFileName().endsWith(".cpp")){
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(QIcon(":/images/fileTypes/cppFile.png"), newFile->getFileName()));
    }else if(newFile->getFileName().endsWith(".h")){
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(QIcon(":/images/fileTypes/headerFile.png"), newFile->getFileName()));
    }else{
        m_pListNameOfFileWidget->addItem(new QListWidgetItem(newFile->getFileName()));
    }
    int newFileRow = m_pListNameOfFileWidget->count() - 1;
    m_pListNameOfFileWidget->setCurrentRow(newFileRow);
}

void GTSideBar::closeProjectSlot(){
    m_pListNameOfFileWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_plblProjectName->clear();
    int count = m_pListNameOfFileWidget->count();
    for(int i = count; i >= 0; i--){
        delete m_pListNameOfFileWidget->takeItem(i);
    }
}

//Деструктор
GTSideBar::~GTSideBar(){

}

