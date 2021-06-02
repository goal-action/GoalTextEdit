#include "gtmainmenu.h"

GTMainMenu::GTMainMenu(QWidget* parent)
    : QMenuBar(parent)
{
    //Project
    QMenu* projectMenu = new QMenu("Project", this);
    QAction* newProjectAction = projectMenu->addAction(QIcon(":/images/menu/newProject.png"), "&New project", this, SIGNAL(newProjectPressedSignal()), Qt::CTRL | Qt::Key_N);
    QAction* openProjectAction = projectMenu->addAction(QIcon(":/images/menu/openProject.png"), "&Open project", this, SIGNAL(openProjectPressedSignal()), Qt::CTRL | Qt::Key_O);
    QAction* closeProjectAction = projectMenu->addAction(QIcon(":/images/menu/close.png"), "&Close project", this, SIGNAL(closeProjectPressedSignal()));
    QAction* newFileAction = projectMenu->addAction(QIcon(":/images/menu/newFile.png"), "&New file", this, SIGNAL(newFilePressedSignal()));
    QAction* saveProjectAction = projectMenu->addAction("&Save project", this, SIGNAL(saveProjectPressedSignal()));
    QAction* saveFileAction = projectMenu->addAction("&Save file", this, SIGNAL(savefilePressedSignal()), Qt::CTRL | Qt::Key_S);
    QAction* deleteFileAction = projectMenu->addAction("&Delete file", this, SIGNAL(deleteFilePressedSignal()));
    projectMenu->addSeparator();
    QAction* exitAction = projectMenu->addAction("&Exit", this, SIGNAL(exitPressedSignal()), Qt::CTRL | Qt::Key_E);
    newProjectAction->setShortcutVisibleInContextMenu(true);
    openProjectAction->setShortcutVisibleInContextMenu(true);
    newFileAction->setShortcutVisibleInContextMenu(true);
    saveProjectAction->setShortcutVisibleInContextMenu(true);
    saveFileAction->setShortcutVisibleInContextMenu(true);
    deleteFileAction->setShortcutVisibleInContextMenu(true);
    exitAction->setShortcutVisibleInContextMenu(true);


    //Run
    QMenu* runMenu = new QMenu("Run", this);
    QAction* runNowAction = runMenu->addAction("&Run now", this, SIGNAL(runNowPressedSignal()));

    //Help
    QMenu* helpMenu = new QMenu("Help", this);
    QAction* aboutAction = helpMenu->addAction("&About", this, SIGNAL(aboutPressedSignal()));

    //View
    QMenu* viewMenu = new QMenu("View", this);

    QMenu* subMenuSideBar = new QMenu("Side bar", this);
    QAction* leftSideBarAction = subMenuSideBar->addAction("Left", this, SIGNAL(leftSideBarPressedSignal()));
    QAction* rightSideBarAction = subMenuSideBar->addAction("Right", this, SIGNAL(rightSideBarPressedSignal()));
    leftSideBarAction->setCheckable(true);
    rightSideBarAction->setCheckable(true);
    rightSideBarAction->setChecked(true);
    connect(leftSideBarAction, SIGNAL(triggered(bool)), rightSideBarAction, SLOT(toggle()));
    connect(rightSideBarAction, SIGNAL(triggered(bool)), leftSideBarAction, SLOT(toggle()));

    QMenu* subMenuTheme = new QMenu("Theme", this);
    QAction* lightThemeAction = subMenuTheme->addAction("Light", this, SIGNAL(lightThemePressedSignal()));
    QAction* darkThemeAction = subMenuTheme->addAction("Dark", this, SIGNAL(darkThemePressedSignal()));
    lightThemeAction->setChecked(true);
    lightThemeAction->setCheckable(true);
    darkThemeAction->setCheckable(true);
    connect(lightThemeAction, SIGNAL(triggered(bool)), darkThemeAction, SLOT(toggle()));
    connect(darkThemeAction, SIGNAL(triggered(bool)), lightThemeAction, SLOT(toggle()));

    viewMenu->addMenu(subMenuSideBar);
    viewMenu->addMenu(subMenuTheme);

    addMenu(projectMenu);
    addMenu(runMenu);
    addMenu(helpMenu);
    addMenu(viewMenu);

    m_pvAvailableActionsWithProjectAndFiles = new QVector<QAction*>;
    m_pvAvailableActionsWithProjectAndFiles->push_back(saveFileAction);
    m_pvAvailableActionsWithProjectAndFiles->push_back(deleteFileAction);
    m_pvAvailableActionsWithProjectAndFiles->push_back(saveProjectAction);
    m_pvAvailableActionsWithProjectAndFiles->push_back(runNowAction);

    m_pvAvailableActionsWithProject = new QVector<QAction*>;
    m_pvAvailableActionsWithProject->push_back(closeProjectAction);
    m_pvAvailableActionsWithProject->push_back(newFileAction);

    for(int i = 0;i <  m_pvAvailableActionsWithProjectAndFiles->size(); i++){
        m_pvAvailableActionsWithProjectAndFiles->at(i)->setEnabled(false);
    }

    for(int i = 0; i < m_pvAvailableActionsWithProject->size(); i++){
        m_pvAvailableActionsWithProject->at(i)->setEnabled(false);
    }

    //Стили CSS
    m_pvAllMenus = new QVector<QMenu*>;
    m_pvAllMenus->push_back(projectMenu);
    m_pvAllMenus->push_back(runMenu);
    m_pvAllMenus->push_back(helpMenu);
    m_pvAllMenus->push_back(viewMenu);
    m_pvAllMenus->push_back(subMenuSideBar);
    m_pvAllMenus->push_back(subMenuTheme);

}

void GTMainMenu::setProjectExistActionsAvailable(){
    for(int i = 0; i < m_pvAvailableActionsWithProject->size(); ++i)
        m_pvAvailableActionsWithProject->at(i)->setEnabled(true);
}

void GTMainMenu::setProjectAndFileExistActionsAvailable(){
    for(int i = 0; i < m_pvAvailableActionsWithProjectAndFiles->size(); ++i)
        m_pvAvailableActionsWithProjectAndFiles->at(i)->setEnabled(true);
}

void GTMainMenu::setProjectExistActionsUnAvailable(){
    for(int i = 0; i < m_pvAvailableActionsWithProject->size(); ++i)
        m_pvAvailableActionsWithProject->at(i)->setEnabled(false);
}

void GTMainMenu::setProjectAndFileExistActionsUnAvailable(){
    for(int i = 0; i < m_pvAvailableActionsWithProjectAndFiles->size(); ++i)
        m_pvAvailableActionsWithProjectAndFiles->at(i)->setEnabled(false);
}

//методы для смены темы
void GTMainMenu::setLightTheme(){
    QFile fileMenuLightCss(":/styles/mainMenuStyle/lightTheme/menu.css");
    fileMenuLightCss.open(QIODevice::ReadOnly);
    for(int i = 0; i < m_pvAllMenus->size(); i++){
        m_pvAllMenus->at(i)->setStyleSheet(fileMenuLightCss.readAll());
        fileMenuLightCss.seek(0);
    }
    fileMenuLightCss.close();

    QFile fileMenuBarLightCss(":/styles/mainMenuStyle/lightTheme/menuBar.css");
    fileMenuBarLightCss.open(QIODevice::ReadOnly);
    setStyleSheet(fileMenuBarLightCss.readAll());
    fileMenuBarLightCss.close();
}

void GTMainMenu::setDarkTheme(){
    QFile fileMenuDarkCss(":/styles/mainMenuStyle/darkTheme/menu.css");
    fileMenuDarkCss.open(QIODevice::ReadOnly);
    for(int i = 0; i < m_pvAllMenus->size(); i++){
        m_pvAllMenus->at(i)->setStyleSheet(fileMenuDarkCss.readAll());
        fileMenuDarkCss.seek(0);
    }
    fileMenuDarkCss.close();

    QFile fileMenuBarDarkCss(":/styles/mainMenuStyle/darkTheme/menuBar.css");
    fileMenuBarDarkCss.open(QIODevice::ReadOnly);
    setStyleSheet(fileMenuBarDarkCss.readAll());
    fileMenuBarDarkCss.close();
}

//Деструктор
GTMainMenu::~GTMainMenu(){
    delete m_pvAvailableActionsWithProjectAndFiles;
    delete m_pvAvailableActionsWithProject;
}
